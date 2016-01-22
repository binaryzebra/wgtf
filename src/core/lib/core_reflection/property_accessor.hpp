#ifndef PROPERTY_ACCESSOR_HPP
#define PROPERTY_ACCESSOR_HPP

#include <string>
#include <memory>
#include "i_definition_manager.hpp"
#include "core_variant/collection.hpp"

class TypeId;
class ObjectHandle;
class IBaseProperty;
class IClassDefinition;
class MetaBase;
class Variant;
class ReflectedMethodParameters;

class PropertyAccessor
{
public:
	PropertyAccessor();
	PropertyAccessor( const PropertyAccessor & other );
	PropertyAccessor( PropertyAccessor && other );
	
	PropertyAccessor& operator = ( PropertyAccessor && other );

	bool isValid() const;

	const TypeId & getType() const;
	PropertyAccessor getParent() const;

	MetaHandle getMetaData() const;

	const char * getName() const;
	const IClassDefinition * getStructDefinition() const;

	// TODO: hide these accessors
	bool canGetValue() const;
	Variant getValue() const;

	bool canSetValue() const;
	bool setValue(const Variant & value ) const;
	bool setValueWithoutNotification( const Variant & value ) const;

	bool canInvoke() const;
	Variant invoke( const ReflectedMethodParameters & parameters, bool undo = false ) const;

	IBaseProperty * getProperty() const { return property_; }
	const ObjectHandle & getObject() const { return object_; }

	const ObjectHandle & getRootObject() const;
	const char * getFullPath() const;

	const IDefinitionManager * getDefinitionManager() const;
private:
	ObjectHandle			object_;
	std::shared_ptr< IBaseProperty > sharedProperty_;
	IBaseProperty *			property_;

	ObjectHandle			rootObject_;
	std::string				path_;
	const IDefinitionManager *	definitionManager_;

	friend class ClassDefinition;

	PropertyAccessor(
		const IDefinitionManager * definitionManager,
		const ObjectHandle & rootObject, const char * path );
	void setObject( const ObjectHandle & object );
	void setBaseProperty( IBaseProperty * property );
	void setBaseProperty( const std::shared_ptr< IBaseProperty > & property );
};

#endif // PROPERTY_ACCESSOR_HPP
