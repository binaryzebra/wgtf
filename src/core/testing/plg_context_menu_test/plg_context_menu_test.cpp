#include "core_dependency_system/i_interface.hpp"
#include "core_generic_plugin/generic_plugin.hpp"
#include "core_qt_common/i_qt_framework.hpp"
#include "core_qt_common/qt_action_manager.hpp"
#include "core_ui_framework/i_action.hpp"
#include "core_ui_framework/i_ui_application.hpp"
#include "core_ui_framework/i_ui_framework.hpp"
#include "core_ui_framework/i_view.hpp"
#include "core_ui_framework/i_window.hpp"
#include "core_variant/variant.hpp"

//==============================================================================
class ContextMenuTest
	: public PluginMain
{
public:
	//==========================================================================
	ContextMenuTest( IComponentContext & contextManager ){}

	//==========================================================================
	bool PostLoad( IComponentContext & contextManager )
	{
		return true;
	}

	//==========================================================================
	void Initialise( IComponentContext & contextManager )
	{
		Variant::setMetaTypeManager( contextManager.queryInterface< IMetaTypeManager >() );

		auto uiFramework = contextManager.queryInterface< IUIFramework >();
		assert( uiFramework != nullptr );

		auto uiApplication = contextManager.queryInterface< IUIApplication >();
		assert( uiApplication != nullptr );

		// Load the action data
		uiFramework->loadActionData( ":/plg_context_menu_test/actions.xml", IUIFramework::ResourceType::File );

		// Create actions and add them to the UI Application
		using namespace std::placeholders;
		cmTestOpen_ = uiFramework->createAction( "CMTestExplorerOpen",
			std::bind( &ContextMenuTest::executeOpen, this, _1 ),
			std::bind( &ContextMenuTest::canExecuteOpen, this, _1 ) );
		uiApplication->addAction( *cmTestOpen_ );

		cmTestCheckOut_ = uiFramework->createAction( "CMTestPerforceCheckOut",
			std::bind( &ContextMenuTest::executeCheckOut, this, _1 ) );
		uiApplication->addAction( *cmTestCheckOut_ );

		// Create the view and present it
		auto pQtFramework = contextManager.queryInterface< IQtFramework >();
		if (pQtFramework != nullptr)
		{
			testView_ = pQtFramework->createView("plg_context_menu_test/test_contextmenu_panel.qml",
				IUIFramework::ResourceType::Url );
		}
		uiApplication->addView( *testView_ );
	}

	//==========================================================================
	bool Finalise( IComponentContext & contextManager )
	{
		auto uiApplication = contextManager.queryInterface< IUIApplication >();
		assert( uiApplication != nullptr );
		
		uiApplication->removeAction( *cmTestOpen_ );
		uiApplication->removeAction( *cmTestCheckOut_ );
		
		cmTestOpen_.reset();
		cmTestCheckOut_.reset();

		uiApplication->removeView( *testView_ );
		testView_ = nullptr;

		return true;
	}
	
	//==========================================================================
	bool canExecuteOpen( const IAction* action )
	{
		return true;
	}

	void executeOpen( IAction * action )
	{
		unsigned int echoValue = 0;

		if (action != nullptr)
		{
			Variant& variant = action->getData();
			if (variant.canCast< unsigned int >())
			{
				echoValue = variant.cast< unsigned int >();
			}
		}

		NGT_DEBUG_MSG( "Open file context menu item clicked: %d !\n", echoValue );
	}
		
	//==========================================================================
	void executeCheckOut( IAction * action )
	{
		NGT_DEBUG_MSG( "Perforce check out context menu item clicked!\n" );
	}

private:
	
	std::unique_ptr<IView> testView_;
	std::unique_ptr< IAction > cmTestCheckOut_;
	std::unique_ptr< IAction > cmTestOpen_;
};

PLG_CALLBACK_FUNC( ContextMenuTest )

