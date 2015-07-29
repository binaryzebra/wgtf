#include "custom_model_interface_test.hpp"

#include "generic_plugin/interfaces/i_component_context.hpp"
#include "reflection/reflected_object.hpp"
#include "reflection/reflection_macros.hpp"
#include "reflection/metadata/meta_types.hpp"
#include "reflection/function_property.hpp"
#include "reflection/utilities/reflection_function_utilities.hpp"
#include "qt_common/i_qt_framework.hpp"
#include "ui_framework/i_ui_application.hpp"
#include "ui_framework/i_view.hpp"

namespace CustomModelInterfaceTest_Locals
{
	class ICustomModelInterface
	{
		DECLARE_REFLECTED

	public:
		ICustomModelInterface()
		{

		}

	protected:
		ICustomModelInterface( int numeric, std::string string )
			: numeric_( numeric )
			, string_( string )
		{

		}

	private:
		int numeric_;
		std::string string_;
	};

	class CustomModelImplementation1 : public ICustomModelInterface
	{
	public:
		CustomModelImplementation1()
			: ICustomModelInterface( 1, "Implementation 1" )
		{

		}
	};

	class CustomModelImplementation2 : public ICustomModelInterface
	{
	public:
		CustomModelImplementation2()
			: ICustomModelInterface( 2, "Implementation 2" )
		{

		}
	};

	class CustomModelImplementation3 : public ICustomModelInterface
	{
	public:
		CustomModelImplementation3()
			: ICustomModelInterface( 3, "Implementation 3" )
		{

		}
	};

	class TestFixture
	{
		DECLARE_REFLECTED

	public:
		void init( IDefinitionManager * defManager )
		{
			auto def = defManager->getDefinition< ICustomModelInterface >();
			auto impl1 = std::unique_ptr< ICustomModelInterface >(
				new CustomModelImplementation1 );
			auto impl2 = std::unique_ptr< ICustomModelInterface >(
				new CustomModelImplementation2 );
			auto impl3 = std::unique_ptr< ICustomModelInterface >(
				new CustomModelImplementation3 );

			implementation1_ = ObjectHandle( std::move( impl1 ), def );
			implementation2_ = ObjectHandle( std::move( impl2 ), def );
			implementation3_ = ObjectHandle( std::move( impl3 ), def );
		}

		ObjectHandle implementation1() const
		{
			return implementation1_;
		}

		ObjectHandle implementation2() const
		{
			return implementation2_;
		}

		ObjectHandle implementation3() const
		{
			return implementation3_;
		}

	private:
		ObjectHandle implementation1_;
		ObjectHandle implementation2_;
		ObjectHandle implementation3_;
	};
}

BEGIN_EXPOSE( CustomModelInterfaceTest_Locals::ICustomModelInterface, MetaNone() )
	EXPOSE( "Numeric", numeric_, MetaNone() )
	EXPOSE( "String", string_, MetaNone() )
END_EXPOSE()

BEGIN_EXPOSE( CustomModelInterfaceTest_Locals::TestFixture, MetaNone() )
	EXPOSE( "Implementation1", implementation1, MetaNone() )
	EXPOSE( "Implementation2", implementation2, MetaNone() )
	EXPOSE( "Implementation3", implementation3, MetaNone() )
END_EXPOSE()

CustomModelInterfaceTest::CustomModelInterfaceTest()
{

}

CustomModelInterfaceTest::~CustomModelInterfaceTest()
{

}

void CustomModelInterfaceTest::initialise( IComponentContext & contextManager )
{
	auto defManager = contextManager.queryInterface< IDefinitionManager >();
	if (defManager == nullptr)
	{
		return;
	}

	defManager->registerDefinition( new TypeClassDefinition< 
		CustomModelInterfaceTest_Locals::ICustomModelInterface >() );
	defManager->registerDefinition( new TypeClassDefinition< 
		CustomModelInterfaceTest_Locals::TestFixture >() );

	auto testFixture = defManager->createT< 
		CustomModelInterfaceTest_Locals::TestFixture >();
	testFixture->init( defManager );

	auto qtFramework = contextManager.queryInterface< IQtFramework >();
	if (qtFramework == nullptr)
	{
		return;
	}

	testView_ = qtFramework->createView( 
		"qrc:///testing/custom_model_interface_test_panel.qml", 
		IUIFramework::ResourceType::Url, testFixture );

	auto uiApplication = contextManager.queryInterface< IUIApplication >();
	if (uiApplication == nullptr)
	{
		return;
	}

	uiApplication->addView( *testView_ );
}