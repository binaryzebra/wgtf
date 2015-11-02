#ifndef TEST_FIXTURE_HPP
#define TEST_FIXTURE_HPP

#include "reflection_test_module.hpp"

#include "core_generic_plugin_manager/test_plugin_loader.hpp"
#include "core_python27/scripting_engine.hpp"

#include <stack>
#include <memory>

class ReflectionTestModule;

class TestPythonFixture : public TestPluginLoader
{
public:
	TestPythonFixture( const char * testName,
		TestResult & result );
	virtual ~TestPythonFixture();

	Python27ScriptingEngine scriptingEngine_;

private:
	std::stack<IInterface*> interfaces_;
	std::unique_ptr< ReflectionTestModule > reflectionModule_;
};
#endif
