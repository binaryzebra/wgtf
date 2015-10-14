#include "base_asset_object_item.hpp"
#include "core_data_model/i_item_role.hpp"
#include "core_data_model/asset_browser/i_thumbnail_provider.hpp"

struct BaseAssetObjectItem::Implementation
{
	typedef std::vector< BaseAssetObjectItem > BaseAssetObjectItems;

	Implementation( BaseAssetObjectItem & self,  const FileInfo & fileInfo, const IItem * parent, 
		IFileSystem * fileSystem, IThumbnailProvider * thumbnailProvider )
		: self_( self )
		, fileInfo_( fileInfo )
		, parent_( parent )
		, fileSystem_( fileSystem )
		, thumbnailProvider_( thumbnailProvider )
	{
	}

	BaseAssetObjectItems & getChildren() const
	{
		// Not currently thread safe, only one thread can initialize the children
		if (fileSystem_ != nullptr && fileInfo_.isDirectory() && children_.empty())
		{
			fileSystem_->enumerate( fileInfo_.fullPath.c_str(), [&]( FileInfo && info )
			{
				if (info.isDirectory() && !info.isDots() && !(info.attributes & FileAttributes::Hidden))
					children_.emplace_back( info, &self_, fileSystem_, thumbnailProvider_ );
				return true;
			});
		}
		return children_;
	}
	
	BaseAssetObjectItem & self_;
	FileInfo fileInfo_;
	const IItem * parent_;
	IFileSystem * fileSystem_;
	IThumbnailProvider * thumbnailProvider_;
	mutable BaseAssetObjectItems children_;	
};

BaseAssetObjectItem::BaseAssetObjectItem( const BaseAssetObjectItem & rhs )
	: impl_( new Implementation( *this, 
								 rhs.impl_->fileInfo_, 
								 rhs.impl_->parent_, 
								 rhs.impl_->fileSystem_, 
								 rhs.impl_->thumbnailProvider_ ) )
{
}

BaseAssetObjectItem::BaseAssetObjectItem( const FileInfo & fileInfo, const IItem * parent, 
	IFileSystem * fileSystem, IThumbnailProvider * thumbnailProvider )
	: impl_( new Implementation( *this, fileInfo, parent, fileSystem, thumbnailProvider ) )
{
}

BaseAssetObjectItem::~BaseAssetObjectItem()
{
}

BaseAssetObjectItem & BaseAssetObjectItem::operator=( const BaseAssetObjectItem & rhs )
{
	if (this != &rhs)
	{
		impl_.reset( new Implementation( *this, rhs.impl_->fileInfo_, rhs.impl_->parent_, 
			rhs.impl_->fileSystem_, rhs.impl_->thumbnailProvider_ ) );
	}

	return *this;
}

const IItem * BaseAssetObjectItem::getParent() const
{
	return impl_->parent_;
}

IItem* BaseAssetObjectItem::operator[]( size_t index ) const
{
	if (impl_->getChildren().size() > index)
	{
		return &impl_->getChildren()[ index ];
	}

	return nullptr;
}

size_t BaseAssetObjectItem::indexOf( const IItem* item ) const
{
	auto& children = impl_->getChildren();
	return static_cast< const BaseAssetObjectItem * >( item )-&*begin( children );
}

bool BaseAssetObjectItem::empty() const
{
	return impl_->getChildren().empty();
}

size_t BaseAssetObjectItem::size() const
{
	return impl_->getChildren().size();
}

int BaseAssetObjectItem::columnCount() const
{
	return 1;
}

const char * BaseAssetObjectItem::getDisplayText( int column ) const
{
	return getFileName();
}

ThumbnailData BaseAssetObjectItem::getThumbnail( int column ) const
{
	if (impl_->thumbnailProvider_ != nullptr )
	{
		return impl_->thumbnailProvider_->fetch();
	}

	return nullptr;
}

Variant BaseAssetObjectItem::getData( int column, size_t roleId ) const
{
	if (column != 0)
	{
		return Variant();
	}

	if (roleId == ValueRole::roleId_)
	{
		return getFileName();
	}
	else if (roleId == IndexPathRole::roleId_)
	{
		return getFullPath();
	}
	else if (roleId == ThumbnailRole::roleId_)
	{
		return getThumbnail( 0 );
	}
	else if (roleId == SizeRole::roleId_)
	{
		return getSize();
	}
	else if (roleId == CreatedTimeRole::roleId_)
	{
		return getCreatedTime();
	}
	else if (roleId == ModifiedTimeRole::roleId_)
	{
		return getModifiedTime();
	}
	else if (roleId == AccessedTimeRole::roleId_)
	{
		return getAccessedTime();
	}
	else if (roleId == IsDirectoryRole::roleId_)
	{
		return isDirectory();
	}
	else if (roleId == IsReadOnlyRole::roleId_)
	{
		return isReadOnly();
	}
	else if (roleId == IsCompressedRole::roleId_)
	{
		return isCompressed();
	}

	return Variant();
}

bool BaseAssetObjectItem::setData( int column, size_t roleId, const Variant & data )
{
	return false;
}

const FileInfo& BaseAssetObjectItem::getFileInfo() const
{
	return impl_->fileInfo_;
}

const char* BaseAssetObjectItem::getFileName() const
{
	return impl_->fileInfo_.name();
}

const char* BaseAssetObjectItem::getFullPath() const
{
	return impl_->fileInfo_.fullPath.c_str();
}

uint64_t BaseAssetObjectItem::getSize() const
{
	return impl_->fileInfo_.size;
}

uint64_t BaseAssetObjectItem::getCreatedTime() const
{
	return impl_->fileInfo_.created;
}

uint64_t BaseAssetObjectItem::getModifiedTime() const
{
	return impl_->fileInfo_.modified;
}

uint64_t BaseAssetObjectItem::getAccessedTime() const
{
	return impl_->fileInfo_.accessed;
}

bool BaseAssetObjectItem::isDirectory() const
{
	return impl_->fileInfo_.isDirectory();
}

bool BaseAssetObjectItem::isReadOnly() const
{
	return impl_->fileInfo_.isReadOnly();
}

bool BaseAssetObjectItem::isCompressed() const
{
	return (impl_->fileInfo_.attributes & FileAttributes::Compressed) == FileAttributes::Compressed;
}
