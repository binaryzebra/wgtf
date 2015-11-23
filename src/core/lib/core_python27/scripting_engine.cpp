#include "pch.hpp"
#include "definition_details.hpp"

#include "core_logging/logging.hpp"

#include "scripting_engine.hpp"
#include "defined_instance.hpp"
#include "script_object_definition_registry.hpp"
#include "type_converters/python_meta_type.hpp"
#include "core_script/type_converter_queue.hpp"
#include "type_converters/long_converter.hpp"
#include "type_converters/string_converter.hpp"
#include "type_converters/list_converter.hpp"
#include "type_converters/tuple_converter.hpp"
#include "type_converters/type_converter.hpp"

#include "core_variant/interfaces/i_meta_type_manager.hpp"
#include "core_variant/variant.hpp"
#include "core_generic_plugin/interfaces/i_component_context.hpp"
#include "core_reflection/object_handle.hpp"

#include "wg_pyscript/py_script_object.hpp"
#include "wg_pyscript/py_script_output_writer.hpp"


#include <array>
#include <vector>


struct Python27ScriptingEngine::Implementation
{
	Implementation( Python27ScriptingEngine& self, IComponentContext& context );

	void initialiseDefinitionRegistry();
	void finaliseDefinitionRegistry();
	void initialisePython();
	void finalisePython();
	void initialiseTypeConverters();
	void finaliseTypeConverters();

	Python27ScriptingEngine& self_;
	IComponentContext& context_;

	std::vector<std::unique_ptr<MetaType>> defaultMetaTypes_;

	PythonTypeConverters typeConverters_;
	PythonType::StringConverter defaultTypeConverter_;
	PythonType::ListConverter listTypeConverter_;
	PythonType::TupleConverter tupleTypeConverter_;
	PythonType::TypeConverter typeTypeConverter_;
	PythonType::LongConverter longTypeConverter_;
	IInterface* pTypeConvertersInterface_;
	IInterface* scriptObjectDefinitionRegistryInterface_;
};


Python27ScriptingEngine::Implementation::Implementation( Python27ScriptingEngine& self, IComponentContext& context )
	: self_( self )
	, context_( context )
	, listTypeConverter_( typeConverters_ )
	, tupleTypeConverter_( typeConverters_ )
	, pTypeConvertersInterface_( nullptr )
{
}


void Python27ScriptingEngine::Implementation::initialiseDefinitionRegistry()
{
	IScriptObjectDefinitionRegistry* scriptObjectDefinitionRegistry = new ScriptObjectDefinitionRegistry( context_ );
	const bool transferOwnership = true;
	scriptObjectDefinitionRegistryInterface_ =
		context_.registerInterface( scriptObjectDefinitionRegistry, transferOwnership, IComponentContext::Reg_Local );
}


void Python27ScriptingEngine::Implementation::finaliseDefinitionRegistry()
{
	context_.deregisterInterface( scriptObjectDefinitionRegistryInterface_ );
}


void Python27ScriptingEngine::Implementation::initialisePython()
{
	// Warn if tab and spaces are mixed in indentation.
	Py_TabcheckFlag = 1;
	// Disable importing Lib/site.py on startup.
	Py_NoSiteFlag = 1;
	// Enable debug output
	// Requires the scriptoutputwriter output hook from stdout/stderr
	//Py_VerboseFlag = 2;
	// Use environment variables
	Py_IgnoreEnvironmentFlag = 0;

	// Initialize logging as a standard module
	// Must be before Py_Initialize()
	PyImport_AppendInittab( "scriptoutputwriter",
		PyScript::PyInit_ScriptOutputWriter );

	Py_Initialize();
	
	// Import the logging module
	// Must be after Py_Initialize()
	PyImport_ImportModule( "scriptoutputwriter" );
}


void Python27ScriptingEngine::Implementation::finalisePython()
{
	// Must not use any PyObjects after this point
	Py_Finalize();
}


void Python27ScriptingEngine::Implementation::initialiseTypeConverters()
{
	// Register Python types to be usable by Variant
	auto pMetaTypeManager = context_.queryInterface< IMetaTypeManager >();
	assert( pMetaTypeManager != nullptr );

	if (pMetaTypeManager != nullptr)
	{
		defaultMetaTypes_.emplace_back( new MetaTypeImpl< PythonMetaType >() );
		for (const auto & type : defaultMetaTypes_)
		{
			const auto success = pMetaTypeManager->registerType( type.get() );
			assert( success );
		}
	}

	// Register type converters for converting between PyObjects and Variant
	typeConverters_.registerTypeConverter( defaultTypeConverter_ );
	typeConverters_.registerTypeConverter( listTypeConverter_ );
	typeConverters_.registerTypeConverter( tupleTypeConverter_ );
	typeConverters_.registerTypeConverter( typeTypeConverter_ );
	typeConverters_.registerTypeConverter( longTypeConverter_ );

	const bool transferOwnership = false;
	pTypeConvertersInterface_ = context_.registerInterface(
		&typeConverters_, transferOwnership, IComponentContext::Reg_Local );
}


void Python27ScriptingEngine::Implementation::finaliseTypeConverters()
{
	// Deregister type converters for converting between PyObjects and Variant
	typeConverters_.deregisterTypeConverter( longTypeConverter_ );
	typeConverters_.deregisterTypeConverter( typeTypeConverter_ );
	typeConverters_.deregisterTypeConverter( tupleTypeConverter_ );
	typeConverters_.deregisterTypeConverter( listTypeConverter_ );
	typeConverters_.deregisterTypeConverter( defaultTypeConverter_ );
	context_.deregisterInterface( pTypeConvertersInterface_ );

	// Register Python types to be usable by Variant
	auto pMetaTypeManager = context_.queryInterface< IMetaTypeManager >();
	assert( pMetaTypeManager != nullptr );

	if (pMetaTypeManager != nullptr)
	{
		for (const auto & type : defaultMetaTypes_)
		{
			const auto success = pMetaTypeManager->deregisterType( type.get() );
			assert( success );
		}
	}

	defaultMetaTypes_.clear();
}


Python27ScriptingEngine::Python27ScriptingEngine( IComponentContext& context )
	: impl_( new Implementation( *this, context ) )
{
}


Python27ScriptingEngine::~Python27ScriptingEngine()
{
}


bool Python27ScriptingEngine::init()
{
	impl_->initialisePython();
	impl_->initialiseTypeConverters();
	impl_->initialiseDefinitionRegistry();
	return true;
}


void Python27ScriptingEngine::fini()
{
	impl_->finaliseDefinitionRegistry();
	impl_->finaliseTypeConverters();
	impl_->finalisePython();
}


bool Python27ScriptingEngine::appendPath( const wchar_t* path )
{
	PyScript::ScriptObject testPathObject =
		PyScript::ScriptObject::createFrom( path );

	PyObject* pySysPaths = PySys_GetObject( "path" );
	if (pySysPaths == nullptr)
	{
		NGT_ERROR_MSG( "Unable to get sys.path\n" );
		return false;
	}

	PyScript::ScriptList sysPaths( pySysPaths );
	sysPaths.append( testPathObject );

	const int result = PySys_SetObject( "path", pySysPaths );
	if (result != 0)
	{
		NGT_ERROR_MSG( "Unable to assign sys.path\n" );
		return false;
	}

	return true;
}


ObjectHandle Python27ScriptingEngine::import( const char* name )
{
	if (!Py_IsInitialized())
	{
		return nullptr;
	}

	PyScript::ScriptModule module = PyScript::ScriptModule::import( name,
		PyScript::ScriptErrorPrint( "Unable to import\n" ) );

	if (!module.exists())
	{
		return nullptr;
	}

	std::unique_ptr<ReflectedPython::DefinedInstance> pointer(
		new ReflectedPython::DefinedInstance( impl_->context_, module ) );
	ObjectHandleT<ReflectedPython::DefinedInstance> handle( std::move( pointer ), &pointer->getDefinition() );
	return handle;
}


bool Python27ScriptingEngine::checkErrors()
{
	if (PyScript::Script::hasError())
	{
		PyScript::Script::printError();
		PyScript::Script::clearError();
		return true;
	}

	return false;
}
