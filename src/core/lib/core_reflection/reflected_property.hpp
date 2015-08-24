#ifndef REFLECTED_PROPERTY_HPP
#define REFLECTED_PROPERTY_HPP

#include "base_property.hpp"
#include "utilities/reflection_utilities.hpp"

#include "core_variant/collection.hpp"
#include "core_variant/variant.hpp"

template< typename TargetType, typename BaseType >
class ReflectedProperty
	: public BaseProperty
{
public:	
	typedef ReflectedProperty< TargetType, BaseType > SelfType;
	typedef TargetType BaseType::* member_ptr;

	explicit ReflectedProperty(
		const char * name, member_ptr memPtr,
		const TypeId & type )
		: BaseProperty( name, type )
		, memberPtr_( memPtr )
	{
		TypeId typeId( "" );
		if (ReflectionUtilities::PropertyTypeHelper< TargetType >::getType( typeId ))
		{
			setType( typeId );
		}
	}


	//==========================================================================
	Variant get( const ObjectHandle & pBase, const IDefinitionManager & definitionManager ) const override
	{
		auto pObject = pBase.reflectedCast< BaseType >( definitionManager );
		if (pObject && memberPtr_)
		{
			return pObject->*memberPtr_;
		}
		else
		{
			return Variant();
		}
	}


	//==========================================================================
	bool set( const ObjectHandle & pBase, const Variant & value, const IDefinitionManager & definitionManager ) const override
	{
		return set_Value< std::is_same<TargetType, Variant>::value >::set(
					pBase, memberPtr_, value, definitionManager );
	}

	
private:
	member_ptr memberPtr_;

	template<bool is_Variant, typename _dummy = void>
	struct set_Value
	{
		static bool set(
			const ObjectHandle & pBase,
			member_ptr memberPtr,
			const Variant & value,
			const IDefinitionManager & definitionManager )
		{
			auto pObject = pBase.reflectedCast< BaseType >( definitionManager );
			if (pObject && memberPtr)
			{
				pObject->*memberPtr = value;
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<typename _dummy>
	struct set_Value<false, _dummy>
	{
		static bool set(
			const ObjectHandle & pBase,
			member_ptr memberPtr,
			const Variant & value,
			const IDefinitionManager & definitionManager )
		{
			return set_impl< Variant::traits< TargetType >::can_downcast >::set(
						pBase, memberPtr, value, definitionManager );
		}
	};


	template<bool can_set, typename _dummy = void>
	struct set_impl
	{
		static bool set(
			const ObjectHandle & pBase,
			member_ptr memberPtr,
			const Variant & value,
			const IDefinitionManager & definitionManager )
		{
			bool br = false;
			auto pObject = pBase.reflectedCast< BaseType >( definitionManager );
			if (pObject && memberPtr)
			{
				br = value.tryCast(pObject->*memberPtr);
			}
			return br;
		}
	};

	template<typename _dummy>
	struct set_impl<false, _dummy>
	{
		static bool set(
			const ObjectHandle & pBase,
			member_ptr,
			const Variant &,
			const IDefinitionManager & )
		{
			// nop
			return false;
		}
	};

};

#endif // REFLECTED_PROPERTY_HPP
