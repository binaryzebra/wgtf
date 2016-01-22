#include "property_accessor.hpp"
#include "interfaces/i_base_property.hpp"
#include "reflected_object.hpp"
#include "reflected_method.hpp"
#include "reflected_method_parameters.hpp"
#include "object_handle.hpp"
#include "property_accessor_listener.hpp"
#include "i_definition_manager.hpp"
#include "utilities/reflection_utilities.hpp"
#include "metadata/meta_base.hpp"

#include <unordered_map>
#include "core_variant/variant.hpp"

//==============================================================================
PropertyAccessor::PropertyAccessor( PropertyAccessor && other )
	: object_( other.object_ )
	, sharedProperty_( std::move( other.sharedProperty_ ) )
	, property_( other.property_ )
	, rootObject_( other.rootObject_ )
	, path_( std::move( other.path_ ) )
	, definitionManager_( other.definitionManager_ )
{
}

//==============================================================================
PropertyAccessor::PropertyAccessor( const PropertyAccessor & other )
: object_( other.object_ )
, sharedProperty_( other.sharedProperty_ )
, property_( other.property_ )
, rootObject_( other.rootObject_ )
, path_( other.path_ )
, definitionManager_( other.definitionManager_ )
{
}

//==============================================================================
PropertyAccessor::PropertyAccessor()
	: property_( nullptr )
	, definitionManager_( nullptr )
{
}

//==============================================================================
PropertyAccessor& PropertyAccessor::operator = (PropertyAccessor&& other)
{
	object_ = other.object_;
	sharedProperty_ = std::move( other.sharedProperty_ );
	property_ = other.property_;
	rootObject_ = other.rootObject_;
	path_ = std::move( other.path_ );
	definitionManager_ = other.definitionManager_;
	return *this;
}


//==============================================================================
PropertyAccessor::PropertyAccessor(
	const IDefinitionManager * definitionManager,
	const ObjectHandle & rootObject, const char * path )
	:property_( nullptr )
	, rootObject_( rootObject )
	, path_( path )
	, definitionManager_( definitionManager )
{
}


//==============================================================================
bool PropertyAccessor::isValid() const
{
	return object_ != ObjectHandle() && getProperty() != nullptr;
}


//==============================================================================
const TypeId & PropertyAccessor::getType() const
{
	if (!isValid())
	{
		static auto s_voidType = TypeId::getType< void >();
		return s_voidType;
	}
	return getProperty()->getType();
}


//==============================================================================
PropertyAccessor PropertyAccessor::getParent() const
{
	if (path_.length() == 0||
		definitionManager_ == nullptr)
	{
		return PropertyAccessor();
	}
	auto definition = rootObject_.getDefinition( *definitionManager_ );
	if (definition == nullptr)
	{
		return PropertyAccessor();
	}
	// This is the member operator '.', the arrow operator '->', and array operator '['
	size_t parentMarkerPos = path_.find_last_of( ".[" );
	if (parentMarkerPos == std::string::npos)
	{
		return definition->bindProperty( "", rootObject_ );
	}
	std::string parentPath;
	parentPath.append( path_.c_str(), parentMarkerPos );
	return definition->bindProperty( parentPath.c_str(), rootObject_ );
}


bool PropertyAccessor::canSetValue() const
{
	if (!this->isValid())
	{
		return false;
	}

	if (definitionManager_ == nullptr)
	{
		return false;
	}

	return !this->getProperty()->readOnly();
}


//==============================================================================
bool PropertyAccessor::setValue( const Variant & value ) const 
{
	if (!this->canSetValue())
	{
		return false;
	}

	// Since "listeners" is a MutableVector, these iterators are safe to use
	// while other listeners are registered/deregistered
	auto& listeners = definitionManager_->getPropertyAccessorListeners();
	auto itBegin = listeners.cbegin();
	auto itEnd = listeners.cend();
	for( auto it = itBegin; it != itEnd; ++it )
	{
		(*it).get()->preSetValue( *this, value );
	}
	bool ret = getProperty()->set( object_, value, *definitionManager_ );
	for( auto it = itBegin; it != itEnd; ++it )
	{
		(*it).get()->postSetValue( *this, value );
	}
	return ret;
}


//==============================================================================
bool PropertyAccessor::setValueWithoutNotification( const Variant & value ) const 
{
	if (!isValid())
	{
		return false;
	}
	if (definitionManager_ == nullptr)
	{
		return false;
	}

	return getProperty()->set( object_, value, *definitionManager_ );
}


bool PropertyAccessor::canInvoke() const
{
	if (!this->isValid())
	{
		return false;
	}

	if (definitionManager_ == nullptr)
	{
		return false;
	}

	return this->getProperty()->isMethod();
}


//==============================================================================
Variant PropertyAccessor::invoke( const ReflectedMethodParameters & parameters, bool undo ) const
{
	Variant result;

	if (!this->canInvoke())
	{
		return result;
	}

	auto& listeners = definitionManager_->getPropertyAccessorListeners();
	auto listenersBegin = listeners.cbegin();
	auto listenersEnd = listeners.cend();

	for (auto itr = listenersBegin; itr != listenersEnd; ++itr)
	{
		itr->get()->preInvoke( *this, parameters, undo );
	}

	if (undo)
	{
		ReflectedMethod* method = static_cast<ReflectedMethod*>( getProperty() );
		method = method->getUndoMethod();
		assert( method != nullptr );
		result = method->invoke( object_, parameters );
	}
	else
	{
		result = getProperty()->invoke( object_, parameters );
	}

	for (auto itr = listenersBegin; itr != listenersEnd; ++itr)
	{
		itr->get()->postInvoke( *this, parameters, undo );
	}

	return result;
}


//==============================================================================
const char * PropertyAccessor::getName() const
{
	if(!isValid())
	{
		return nullptr;
	}
	return getProperty()->getName();
}


//==============================================================================
const IClassDefinition * PropertyAccessor::getStructDefinition() const
{
	if(!isValid())
	{
		return nullptr;
	}
	if (definitionManager_ == nullptr)
	{
		return nullptr;
	}
	Collection collection;
	bool isCollection = getValue().tryCast( collection );
	if (isCollection)
	{
		return 
			definitionManager_->getDefinition(
			collection.valueType().getName()
		);
	}
	return definitionManager_->getDefinition(
		getProperty()->getType().getName() );
}


bool PropertyAccessor::canGetValue() const
{
	if (!this->isValid())
	{
		return false;
	}

	if (definitionManager_ == nullptr)
	{
		return false;
	}

	return this->getProperty()->isValue();
}


//==============================================================================
Variant PropertyAccessor::getValue() const
{
	// TODO NGT-1649 How does it return a value when it's invalid?
	// Does not match behaviour of setValue()
	if (!this->isValid())
	{
		return this->getRootObject();
	}
	if (definitionManager_ == nullptr)
	{
		return this->getRootObject();
	}

	assert( this->canGetValue() );
	return getProperty()->get( object_, *definitionManager_ );
}


//==============================================================================
MetaHandle PropertyAccessor::getMetaData() const
{
	if (getProperty() == nullptr)
	{
		return nullptr;
	}
	return getProperty()->getMetaData();
}


//==============================================================================
void PropertyAccessor::setObject( const ObjectHandle & object )
{
	object_ = object;
}


//==============================================================================
void PropertyAccessor::setBaseProperty( IBaseProperty * property )
{
	sharedProperty_.reset();
	property_ = property;
}


//==============================================================================
void PropertyAccessor::setBaseProperty(
	const std::shared_ptr< IBaseProperty > & property )
{
	sharedProperty_ = property;
	property_ = property.get();
}


//==============================================================================
const ObjectHandle & PropertyAccessor::getRootObject() const
{
	return rootObject_;
}


//==============================================================================
const char * PropertyAccessor::getFullPath() const
{
	return path_.c_str();
}


//==============================================================================
const IDefinitionManager * PropertyAccessor::getDefinitionManager() const
{
	return definitionManager_;
}
