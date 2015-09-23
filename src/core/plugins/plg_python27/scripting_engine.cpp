#include "pch.hpp"

#include "core_logging/logging.hpp"

#include "scripting_engine.hpp"
#include "wg_pyscript/py_script_object.hpp"
#include "wg_pyscript/py_script_output_writer.hpp"
#include "wg_pyscript/type_converter.hpp"


bool Python27ScriptingEngine::init()
{
	// Warn if tab and spaces are mixed in indentation.
	Py_TabcheckFlag = 1;
	// Disable importing Lib/site.py on startup.
	Py_NoSiteFlag = 1;
	// Enable debug output
	// Requires the ScriptOutputWriter output hook from stdout/stderr
	Py_VerboseFlag = 2;
	// Use environment variables
	Py_IgnoreEnvironmentFlag = 0;

	// Initialize logging as a standard module
	// Must be before Py_Initialize()
	PyImport_AppendInittab( "ScriptOutputWriter",
		PyScript::PyInit_ScriptOutputWriter );

	Py_Initialize();
	
	// Import the logging module
	// Must be after Py_Initialize()
	PyImport_ImportModule( "ScriptOutputWriter" );

	return true;
}


void Python27ScriptingEngine::fini()
{
	Py_Finalize();
}


bool Python27ScriptingEngine::appendPath( const wchar_t* path )
{
	PyObject * pyTestPath = TypeConverter::getData( path );
	PyScript::ScriptObject testPathObject( pyTestPath );

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


bool Python27ScriptingEngine::import( const char* name )
{
	if (!Py_IsInitialized())
	{
		return false;
	}
	PyScript::ScriptModule module = PyScript::ScriptModule::import( name,
		PyScript::ScriptErrorPrint( "Unable to import\n" ) );

	// TESTING
	if ((strcmp( name, "python27_test" ) == 0) && (module.get() != nullptr))
	{
		module.callMethod( "run",
			PyScript::ScriptErrorPrint( "Unable to run\n" ) );
	}

	return (module.get() != nullptr);
}

