#include "python_panel.hpp"
#include "core_variant/default_meta_type_manager.hpp"
#include "core_reflection/reflected_object.hpp"
#include "core_reflection/i_definition_manager.hpp"
#include "core_reflection/reflection_macros.hpp"
#include "core_reflection/metadata/meta_types.hpp"
#include "core_reflection/function_property.hpp"
#include "core_reflection/utilities/reflection_function_utilities.hpp"
#include "core_reflection/property_accessor_listener.hpp"
#include "core_logging/logging.hpp"
#include "core_ui_framework/i_ui_framework.hpp"
#include "core_ui_framework/i_ui_application.hpp"
#include "core_data_model/reflection/reflected_tree_model.hpp"


PythonPanel::PythonPanel( const char * panelName,
	IComponentContext & context,
	ObjectHandle & rootPythonObject )
	: Depends( context )
	, context_( context )
{
	this->createContextObject( panelName, rootPythonObject );
	this->addPanel();
}


PythonPanel::~PythonPanel()
{
	this->removePanel();
}


bool PythonPanel::createContextObject( const char * panelName,
	ObjectHandle & pythonObject )
{
	auto pDefinitionManager = this->get< IDefinitionManager >();
	if (pDefinitionManager == nullptr)
	{
		NGT_ERROR_MSG( "Failed to find IDefinitionManager\n" );
		return false;
	}
	auto & definitionManager = (*pDefinitionManager);

	auto controller = this->get< IReflectionController >();
	if (controller == nullptr)
	{
		NGT_ERROR_MSG( "Failed to find IReflectionController\n" );
		return false;
	}

	const bool managed = true;
	contextObject_ = pDefinitionManager->create< PanelContext >( managed );
	contextObject_->panelName_ = panelName;
	contextObject_->pythonObject_ = pythonObject;
	contextObject_->treeModel_.reset(
		new ReflectedTreeModel( pythonObject, definitionManager, controller ) );

	return true;
}


bool PythonPanel::addPanel()
{
	auto uiFramework = get< IUIFramework >();
	auto uiApplication = get< IUIApplication >();
	
	if (uiFramework == nullptr || uiApplication == nullptr)
	{
		NGT_ERROR_MSG( "Failed to find IUIFramework or IUIApplication\n" );
		return false;
	}

	pythonView_ = uiFramework->createView(
		"Python27UITest/PythonObjectTestPanel.qml", IUIFramework::ResourceType::Url, contextObject_ );

	uiApplication->addView( *pythonView_ );
	return true;
}


void PythonPanel::removePanel()
{
	auto uiApplication = get< IUIApplication >();
	
	if (uiApplication == nullptr)
	{
		NGT_ERROR_MSG( "Failed to find IUIApplication\n" );
		return;
	}

	uiApplication->removeView( *pythonView_ );
}

