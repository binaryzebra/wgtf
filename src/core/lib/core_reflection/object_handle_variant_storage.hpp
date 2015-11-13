#ifndef OBJECT_HANDLE_STORAGE_VARIANT_HPP_INCLUDED
#define OBJECT_HANDLE_STORAGE_VARIANT_HPP_INCLUDED


#include "object_handle_storage.hpp"
#include "core_variant/variant.hpp"


class ObjectHandleVariantStorage
	: public IObjectHandleStorage
{
public:
	ObjectHandleVariantStorage( const Variant& variant, const IClassDefinition* definition );
	ObjectHandleVariantStorage( Variant* variant, const IClassDefinition* definition );

	void* data() const override;
	TypeId type() const override;
	bool getId( RefObjectId& o_Id ) const override;
	const IClassDefinition* getDefinition( const IDefinitionManager& definitionManager ) const override;
	void throwBase() const override;

private:
	mutable Variant variant_;
	Variant* variantPtr_;
	const IClassDefinition* definition_;

};


#endif // OBJECT_HANDLE_STORAGE_VARIANT_HPP_INCLUDED