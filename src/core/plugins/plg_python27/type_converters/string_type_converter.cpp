#include "pch.hpp"

#include "string_type_converter.hpp"

#include "core_variant/variant.hpp"
#include "wg_pyscript/py_script_object.hpp"


bool StringTypeConverter::toVariant( const PyScript::ScriptObject & inObject,
	Variant & outVariant )
{
	// Get attribute as a string
	PyScript::ScriptErrorPrint errorHandler;
	PyScript::ScriptString str = inObject.str( errorHandler );
	const char * value = str.c_str();

	// Let variant convert string to type
	// Variant will create storage so don't worry about str going out of scope
	outVariant = Variant( value );

	return true;
}


bool StringTypeConverter::toScriptType( const Variant & inVariant,
	PyScript::ScriptObject & outObject )
{
	const std::string str = inVariant.value< std::string >();

	outObject = PyScript::ScriptString::create( str );
	return true;
}

