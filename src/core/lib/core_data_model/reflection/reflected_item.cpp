#include "reflected_item.hpp"
#include "core_reflection/interfaces/i_class_definition.hpp"
#include "core_reflection/metadata/meta_utilities.hpp"
#include "core_reflection/metadata/meta_impl.hpp"

const IClassDefinition * ReflectedItem::getDefinition() const 
{ 
	auto parent = getParent();
	if (parent == nullptr)
	{
		return nullptr;
	}
	return static_cast< const ReflectedItem * >( parent )->getDefinition();
}

IReflectionController * ReflectedItem::getController() const 
{ 
	if (controller_ != nullptr)
	{
		return controller_;
	}
	if (parent_ != nullptr)
	{
		return parent_->getController();
	}
	return nullptr;
}

void ReflectedItem::setController( IReflectionController * controller )
{
	controller_ = controller;
}

IDefinitionManager * ReflectedItem::getDefinitionManager() const 
{ 
	if (definitionManager_ != nullptr)
	{
		return definitionManager_;
	}
	if (parent_ != nullptr)
	{
		return parent_->getDefinitionManager();
	}
	return nullptr;
}

void ReflectedItem::setDefinitionManager( IDefinitionManager * definitionManager )
{
	definitionManager_ = definitionManager;
}

bool ReflectedItem::EnumerateVisibleProperties(const PropertyCallback& callback) const
{
	auto object = getObject();
	if (object == nullptr)
	{
		return true;
	}

	auto definitionManager = getDefinitionManager();
	if (definitionManager == nullptr)
	{
		return true;
	}

	return EnumerateVisibleProperties(object, *definitionManager, "", callback);
}

bool ReflectedItem::EnumerateVisibleProperties(ObjectHandle object, const IDefinitionManager & definitionManager, const std::string & inplacePath, const PropertyCallback& callback)
{
	auto definition = object.getDefinition( definitionManager );
	if(definition == nullptr)
		return true;

	for ( const auto& property : definition->allProperties() )
	{
		assert(property != nullptr);
		auto inPlace = findFirstMetaData< MetaInPlaceObj >(*property, definitionManager);
		if ( inPlace != nullptr )
		{
			auto propertyAccessor = definition->bindProperty(property->getName(), object);

			if ( !propertyAccessor.canGetValue() )
			{
				continue;
			}

			const Variant & value = propertyAccessor.getValue();
			const bool isCollection = value.typeIs< Collection >();
			if ( isCollection )
			{
				// TODO: Support InPlace collections
				// For now just show the collection
				// Currently Despair's ContainerProperty is pushed up to the collection
				// Eventually we'll need a MetaContainer which we can check here

				if ( !callback(property, inplacePath) )
					return false;
			}

			ObjectHandle handle;
			bool isObjectHandle = value.tryCast(handle);
			if ( isObjectHandle )
			{
				handle = reflectedRoot(handle, definitionManager);
				auto path = inplacePath + property->getName() + ".";
				if ( !EnumerateVisibleProperties(handle, definitionManager, path, callback) )
					return false;
			}
			continue;
		}
		bool isHidden = findFirstMetaData< MetaHiddenObj >(*property, definitionManager) != nullptr;
		if ( isHidden )
		{
			continue;
		}
		if( !callback(property, inplacePath) )
			return false;
	}
	return true;
}