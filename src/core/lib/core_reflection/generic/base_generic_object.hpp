#ifndef BASE_GENERIC_OBJECT_HPP
#define BASE_GENERIC_OBJECT_HPP


#include "core_reflection/reflected_object.hpp"
#include "core_reflection/utilities/reflection_utilities.hpp"


class ReflectedMethodParameters;
class Variant;
class PropertyAccessor;

/**
 *	GenericObject is an object that has a "generic type".
 *	
 *	Generic types are not based on a static class definition, like a C++ class.
 *	Generic types are more like script classes which may share the same
 *	definition or be per instance because they can dynamically
 *	add and remove members.
 */
class BaseGenericObject : public DefinitionProvider
{
public:

	/**
	 *	Get a typed property from the Python object.
	 *	@param name name of property.
	 *	@param outValue value of property is stored here.
	 *	@return true on success.
	 */
	template< typename T >
	bool get( const char * name, T & value ) const;


	/**
	 *	Set a typed property on the Python object.
	 *	@param name name of property.
	 *	@param value value of property to set.
	 *	@return true on success.
	 */
	template< typename T >
	bool set( const char * name, const T & value );


	/**
	 *	Set a variant property on the Python object.
	 *	@param name name of property.
	 *	@param value value of property to set.
	 *	@return true on success.
	 */
	bool set( const char * name, const Variant & value );


	/**
	 *	Call a function which is part of this instance.
	 *	
	 *	@pre the given name must be a callable function on this instance.
	 *	@pre the given parameters must be passable to the function.
	 *	
	 *	@param name the name of the function.
	 *	@param parameters the arguments to the function.
	 *	@return the return value of the function.
	 */
	Variant invoke( const char * name,
		const ReflectedMethodParameters& parameters );


	/**
	 *	Default implementation can be overridden by derived classes.
	 */
	virtual PropertyAccessor findProperty( const char * name ) const;


protected:
	/**
	 *	Default implementation can be overridden by derived classes.
	 */
	virtual Variant invokeProperty( const char * name,
		const ReflectedMethodParameters& parameters );


	/**
	 *	Default implementation can be overridden by derived classes.
	 */
	virtual Variant getProperty( const char * name ) const;


	/**
	 *	Default implementation can be overridden by derived classes.
	 */
	virtual bool setProperty( const char * name,
		const TypeId & typeId,
		Variant & value );


	/**
	 *	Construct a property and add it to the definition.
	 *	
	 *	@param name name of property to be added.
	 *	@param typeId type of property to be added.
	 *	@param pMetaBase metadata about property to be added.
	 *	@return the newly added property or null on failure.
	 */
	virtual IBaseProperty * addProperty( const char * name,
		const TypeId & typeId,
		const MetaBase * pMetaBase ) = 0;


	/**
	 *	Must be implemented in all types that derive from BaseGenericObject.
	 *	
	 *	ObjectHandle must be constructed from the most derived type.
	 *	If and ObjectHandleT< BaseGenericObject > is constructed, then
	 *	it will slice the class.
	 *	
	 *	@return an ObjectHandle containing a pointer to the most derived type
	 *		from BaseGenericObject.
	 */
	virtual ObjectHandle getDerivedType() const = 0;
	virtual ObjectHandle getDerivedType() = 0;
};


template< typename T >
bool BaseGenericObject::get( const char * name, T & value ) const
{
	auto pDefinitionManager = this->getDefinition().getDefinitionManager();
	assert( pDefinitionManager != nullptr );
	auto variant = this->getProperty( name );
	return ReflectionUtilities::extract( variant,
		value,
		(*pDefinitionManager) );
}


template< typename T >
bool BaseGenericObject::set( const char * name, const T & value )
{
	const TypeId typeId = TypeId::getType< T >();
	auto variantValue = ReflectionUtilities::reference( value );
	return this->setProperty( name, typeId, variantValue );
}


#endif // BASE_GENERIC_OBJECT_HPP