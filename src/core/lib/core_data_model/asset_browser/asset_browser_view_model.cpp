//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//  asset_browser_view_model.cpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Copyright (c) Wargaming.net. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "asset_browser_view_model.hpp"
#include "i_asset_browser_model.hpp"
#include "i_asset_browser_event_model.hpp"
#include "i_asset_browser_context_menu_model.hpp"
#include "i_asset_object_model.hpp"

#include "core_data_model/variant_list.hpp"
#include "core_data_model/value_change_notifier.hpp"
#include "core_data_model/i_tree_model.hpp"
#include "core_data_model/i_item_role.hpp"
#include "core_data_model/selection_handler.hpp"

#include "core_serialization/interfaces/i_file_system.hpp"

#include <sstream>

static const size_t NO_SELECTION = SIZE_MAX;

struct AssetBrowserViewModel::AssetBrowserViewModelImplementation
{
	AssetBrowserViewModelImplementation(
		ObjectHandleT<IAssetBrowserModel> data,
		ObjectHandleT<IAssetBrowserContextMenuModel> contextMenu,
		ObjectHandleT<IAssetBrowserEventModel> events)
		: currentSelectedAssetIndex_( -1 )
		, currentFolderHistoryIndex_( NO_SELECTION )
		, breadCrumbItemIndex_( 0 )
		, folderSelectionHistoryIndex_( NO_SELECTION )
		, breadcrumbItemIndexNotifier_( NO_SELECTION )
		, selectedTreeItem_(nullptr)
		, ignoreFolderHistory_( false )
		, contextMenu_( std::move(contextMenu) )
		, data_( std::move(data) )
		, events_( std::move(events) )
	{
		folderSelectionHandler_.onPostSelectionChanged().add< AssetBrowserViewModel::AssetBrowserViewModelImplementation,
		&AssetBrowserViewModel::AssetBrowserViewModelImplementation::onPostFolderDataChanged >( this );
		folderContentSelectionHandler_.onPostSelectionChanged().add< AssetBrowserViewModel::AssetBrowserViewModelImplementation,
			&AssetBrowserViewModel::AssetBrowserViewModelImplementation::onPostFolderContentDataChanged >( this );
	}

	~AssetBrowserViewModelImplementation()
	{
		folderSelectionHandler_.onPostSelectionChanged().remove< AssetBrowserViewModel::AssetBrowserViewModelImplementation,
			&AssetBrowserViewModel::AssetBrowserViewModelImplementation::onPostFolderDataChanged >( this );
		folderContentSelectionHandler_.onPostSelectionChanged().remove< AssetBrowserViewModel::AssetBrowserViewModelImplementation,
			&AssetBrowserViewModel::AssetBrowserViewModelImplementation::onPostFolderContentDataChanged >( this );
	}

	void addRecentFileHistory( const char* file )
	{
		recentFileHistory_.push_back( file );
	}

	/// Rebuild the breadcrumb from fullpath
	void rebuildBreadcrumb( const char* value )
	{
		breadcrumbs_.clear();

		std::string	tmpPath		= value;
		std::string	folderName	= "";
		bool altConvertedRoot = false;

		std::string::size_type	firstIndex	= 0;

		std::string rootPath = "";
		auto assetPaths = data_->assetPaths();
		for (auto& path : assetPaths)
		{
			// NOTE: Using the last path for now since we have one path added to the our file system
			rootPath = path;
		}

		// Convert the root path to use the alt directory seperator to make this compatible with non-Windows systems.
		std::replace( rootPath.begin(), rootPath.end(), FilePath::kAltDirectorySeparator,
			FilePath::kDirectorySeparator );

		// Workaround of the file system returning the root path without kAltDirectorySeparator
		std::string::size_type directorySeperatorIndex = tmpPath.find( FilePath::kDirectorySeparator );
		if (std::string::npos == directorySeperatorIndex)
		{
			// Replace the directory separator, '/', with the alt directory separator, '\\'
			std::replace( rootPath.begin(), rootPath.end(), FilePath::kDirectorySeparator,
				FilePath::kAltDirectorySeparator );
			altConvertedRoot = true;
		}

		// Find and remove the root path and normalize the directory seperator to use the accepted tokenizer format.
		// This is done, because some root paths may have extended pathing such as "../../res/game/", and they must be
		// treated as a single string for breadcrumbs and cannot be tokenized along with the rest of the path.
		firstIndex = tmpPath.find( rootPath.c_str() );
		if (std::string::npos == firstIndex)
		{
			// Malformed path in the FileInfo!
			return;
		}

		tmpPath.erase( firstIndex, rootPath.length() );
		std::replace( tmpPath.begin(), tmpPath.end(), FilePath::kDirectorySeparator, FilePath::kAltDirectorySeparator );

		// Put the root path back to its original state (if needed) and set it as our first breadcrumb so the user may
		// navigate back to the root via breadcrumbs
		if (!altConvertedRoot)
		{
			std::replace( rootPath.begin(), rootPath.end(), FilePath::kDirectorySeparator,
				FilePath::kAltDirectorySeparator );
		}

		breadcrumbs_.push_back( rootPath );

		// Tokenize the remaining portion of the path and create presentable breadcrumb strings that
		// will correspond to navigation history
		std::istringstream stream( tmpPath );
		std::string token;
		while (std::getline( stream, token, (char)FilePath::kAltDirectorySeparator ))
		{
			if (token.length() > 0)
			{
				folderName = token + " " + (char)FilePath::kAltDirectorySeparator;
				breadcrumbs_.push_back( folderName );
			}
		}

		// Update breadcrumb index information to notify the QML
		breadCrumbItemIndex_ = (breadcrumbs_.size() - 1);
		breadcrumbItemIndexNotifier_.value( breadCrumbItemIndex_ );
	}

	void generateBreadcrumbs( const IItem* selectedItem )
	{
		auto folders = data_->getFolderTreeModel();
		if (selectedItem && folders)
		{
			ITreeModel::ItemIndex selectedItemIndex = folders->index( selectedItem );
			auto foundItemIndex = std::find( foldersCrumb_.begin(), foldersCrumb_.end(), selectedItemIndex );
			auto assetObjectModel = selectedItem->getData( 0, ValueRole::roleId_ ).cast<ObjectHandle>();

			// Don't add same ItemIndex twice
			if (!ignoreFolderHistory_ && foldersCrumb_.end() == foundItemIndex)
			{
				// Keep the folder item index history and update current breadcrumb index
				folderItemIndexHistory_.push_back( selectedItemIndex.first );
				currentFolderHistoryIndex_ = (folderItemIndexHistory_.size() - 1);

				foldersCrumb_.push_back( selectedItemIndex );
			}

			// Rebuild the breadcrumb each time to support the breadcrumb click navigation
			rebuildBreadcrumb( assetObjectModel.getBase<IAssetObjectModel>()->getFullPath() );

			// Reset the flag
			ignoreFolderHistory_ = false;
		}
	}

	void onPostFolderDataChanged( const ISelectionHandler* sender,
		const ISelectionHandler::PostSelectionChangedArgs& args )
	{
		std::vector< IItem* > items = folderSelectionHandler_.getSelectedItems();
		if (items.empty())
		{
			return;
		}

		assert( items.size() == 1);

		selectedTreeItem_ = items[0];
		data_.get()->populateFolderContents( selectedTreeItem_ );

		this->generateBreadcrumbs( selectedTreeItem_ );
	}

	void onPostFolderContentDataChanged( const ISelectionHandler* sender,
		const ISelectionHandler::PostSelectionChangedArgs& args )
	{
		std::vector< int > indices = folderSelectionHandler_.getSelectedRows();
		if (indices.empty())
		{
			return;
		}

		assert( indices.size() == 1);

		currentSelectedAssetIndex_ = indices[0];
	}

	VariantList	breadcrumbs_;
	VariantList	recentFileHistory_;
	int			currentSelectedAssetIndex_;
	size_t		currentFolderHistoryIndex_;
	size_t		breadCrumbItemIndex_;

	std::vector<ITreeModel::ItemIndex>	foldersCrumb_;
	ValueChangeNotifier< size_t >		folderSelectionHistoryIndex_;
	ValueChangeNotifier< size_t >		breadcrumbItemIndexNotifier_;
	std::vector<size_t>					folderItemIndexHistory_;
	IItem*								selectedTreeItem_;
	bool								ignoreFolderHistory_;

	ObjectHandleT<IAssetBrowserContextMenuModel>	contextMenu_;
	ObjectHandleT<IAssetBrowserModel>				data_;
	ObjectHandleT<IAssetBrowserEventModel>			events_;

	SelectionHandler folderSelectionHandler_;
	SelectionHandler folderContentSelectionHandler_;
};

AssetBrowserViewModel::AssetBrowserViewModel(
	ObjectHandleT<IAssetBrowserModel> data,
	ObjectHandleT<IAssetBrowserContextMenuModel> contextMenu,
	ObjectHandleT<IAssetBrowserEventModel> events ) :
	impl_( new AssetBrowserViewModelImplementation( std::move(data), std::move(contextMenu), std::move(events) ) )
{
	if(impl_->events_.get())
	{
		impl_->events_->connectUseSelectedAsset([&](const IAssetObjectModel& selectedAsset) {
			onUseSelectedAsset(selectedAsset);
		});
		impl_->events_->connectFilterChanged( [&]( const Variant& filter ) { updateFolderContentsFilter( filter ); } );
	}
}

ObjectHandle AssetBrowserViewModel::data() const
{
	return impl_->data_;
}

ObjectHandle AssetBrowserViewModel::events() const
{
	return impl_->events_;
}

ObjectHandle AssetBrowserViewModel::contextMenu() const
{
	return impl_->contextMenu_;
}

IListModel * AssetBrowserViewModel::getBreadcrumbs() const
{
	return &impl_->breadcrumbs_;
}

size_t AssetBrowserViewModel::getFolderTreeItemIndex() const
{
	if ( impl_->folderItemIndexHistory_.size() <= 0 ||
		NO_SELECTION == impl_->currentFolderHistoryIndex_)
	{
		return 0;
	}

	return impl_->folderItemIndexHistory_[impl_->currentFolderHistoryIndex_];
}

IValueChangeNotifier * AssetBrowserViewModel::folderSelectionHistoryIndex() const
{
	return &impl_->folderSelectionHistoryIndex_;
}

const size_t & AssetBrowserViewModel::getFolderHistoryIndex() const
{
	return impl_->currentFolderHistoryIndex_;
}

void AssetBrowserViewModel::setFolderHistoryIndex( const size_t & index )
{
	impl_->currentFolderHistoryIndex_ = index;
	impl_->folderSelectionHistoryIndex_.value( index );
}

IValueChangeNotifier * AssetBrowserViewModel::breadcrumbItemIndexNotifier() const
{
	return &impl_->breadcrumbItemIndexNotifier_;
}

const size_t & AssetBrowserViewModel::getBreadcrumbItemIndex() const
{
	return impl_->breadCrumbItemIndex_;
}

void AssetBrowserViewModel::setBreadcrumbItemIndex( const size_t & index )
{
	// Do not track this navigation
	impl_->ignoreFolderHistory_ = true;

	impl_->breadCrumbItemIndex_ = index;
	impl_->breadcrumbItemIndexNotifier_.value( index );
}

const int & AssetBrowserViewModel::currentSelectedAssetIndex() const
{
	return impl_->currentSelectedAssetIndex_;
}

void AssetBrowserViewModel::currentSelectedAssetIndex( const int & index )
{
	impl_->currentSelectedAssetIndex_ = index;
}

IAssetObjectModel* AssetBrowserViewModel::getSelectedAssetData() const
{
	//TODO: This will need to support multi-selection. Right now it is a single item
	// selection, but the QML is the same way.
	auto dataModel = impl_->data_.get();
	if (dataModel != nullptr)
	{
		return dataModel->getFolderContentsAtIndex( impl_->currentSelectedAssetIndex_ );
	}

	return nullptr;
}

IListModel * AssetBrowserViewModel::getRecentFileHistory() const
{
	return &impl_->recentFileHistory_;
}

void AssetBrowserViewModel::onUseSelectedAsset( const IAssetObjectModel& selectedAsset )
{
	impl_->addRecentFileHistory( selectedAsset.getFullPath() );
}

bool AssetBrowserViewModel::refreshData() const
{
	if (impl_->selectedTreeItem_ != nullptr)
	{
		impl_->data_->populateFolderContents( impl_->selectedTreeItem_ );
	}

	return true;
}

void AssetBrowserViewModel::updateFolderContentsFilter( const Variant& filter )
{
	std::string newFilter = "";
	if (filter.typeIs<std::string>())
	{
		bool isOk = filter.tryCast( newFilter );
		if (isOk)
		{
			// Set the new filter for folder contents
			impl_->data_->setFolderContentsFilter( newFilter );

			// Refresh is required to apply the new filter
			refreshData();
		}
	}
}

ISelectionHandler * AssetBrowserViewModel::getFolderSelectionHandler() const
{
	return &impl_->folderSelectionHandler_;
}

ISelectionHandler * AssetBrowserViewModel::getFolderContentSelectionHandler() const
{
	return &impl_->folderContentSelectionHandler_;
}
