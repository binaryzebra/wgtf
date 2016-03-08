#include "core_generic_plugin/generic_plugin.hpp"
#include "core_qt_common/shared_controls.hpp"
#include "core_reflection/type_class_definition.hpp"
#include <QWidget>
#include <QQmlEngine>
#include <QQmlContext>

#include "core_generic_plugin/interfaces/i_plugin_context_manager.hpp"

#include "core_ui_framework/i_ui_application.hpp"
#include "core_ui_framework/i_ui_framework.hpp"
#include "core_ui_framework/i_window.hpp"
#include "core_ui_framework/i_view.hpp"
#include "core_qt_common/i_qt_framework.hpp"
#include "core_qt_common/helpers/qt_helpers.hpp"

#include "core_reflection/reflection_macros.hpp"

#include "models/curve_editor.hpp"
#include "models/bezier_point.hpp"
#include "models/point.hpp"
#include "models/curve.hpp"
#include "metadata/i_curve_editor.mpp"



class CurveEditorPlugin
	: public PluginMain
{
public:
	CurveEditorPlugin(IComponentContext & contextManager)
	{
	}

	bool PostLoad( IComponentContext & contextManager ) override
	{
		Q_INIT_RESOURCE(plg_curve_editor);
		return true;
	}

	void Initialise( IComponentContext & contextManager ) override
	{
		auto metaTypeMgr = contextManager.queryInterface< IMetaTypeManager >();
		assert(metaTypeMgr);
		if (metaTypeMgr == nullptr)
			return;
		Variant::setMetaTypeManager(metaTypeMgr);

		auto definitionManager = contextManager.queryInterface<IDefinitionManager>();
		assert(definitionManager != nullptr);
		if (definitionManager == nullptr)
			return;

		// Setup the models for the view
		definitionManager->registerDefinition(new TypeClassDefinition<Point>);
		definitionManager->registerDefinition(new TypeClassDefinition<BezierPoint>);
		definitionManager->registerDefinition(new TypeClassDefinition<ICurve>);
		definitionManager->registerDefinition(new TypeClassDefinition<ICurveEditor>);

		std::unique_ptr<ICurveEditor> curvesModel = std::unique_ptr<ICurveEditor>( new CurveEditor() );

		auto curve = curvesModel->createCurve(CurveTypes::Linear, true);
		{
			BezierPointData pointData1 = { { 0.0f, 0.00f }, { -0.2f, 0.00f }, { 0.2f, 0.0f } };
			curve->add(pointData1);
			BezierPointData pointData2 = {{ 0.25f, 0.25f }, { -0.2f, 0.00f }, { 0.2f, 0.0f }};
			curve->add(pointData2);
			BezierPointData pointData3 = { { 0.5f, 0.50f }, { -0.1f, -0.2f }, { 0.1f, 0.2f } };
			curve->add(pointData3);
			BezierPointData pointData4 = { { 1.0f, 1.00f }, { -0.2f, 0.00f }, { 0.2f, 0.0f } };
			curve->add(pointData4);
		}

		curve = curvesModel->createCurve(CurveTypes::Linear, true);
		{
			BezierPointData pointData1 = { { 0.0f, 0.0f }, { -0.1f, 0.00f }, { 0.1f, 0.1f } };
			curve->add(pointData1);
            BezierPointData pointData2 = { {1.0f, 0.5f }, { -0.1f, -0.1f }, { 0.1f, 0.1f } };
			curve->add(pointData2);
		}

		curve = curvesModel->createCurve(CurveTypes::CubicBezier, true);
		{
			BezierPointData pointData1 = { { 0.0f, 0.0f }, { -0.1f, 0.00f }, { 0.1f, 0.1f } };
			curve->add(pointData1);
			BezierPointData pointData2 = { { 0.8f, 0.1f }, { -0.1f, -0.1f }, { 0.1f, 0.1f } };
			curve->add(pointData2);
			BezierPointData pointData3 = { { 0.9f, 0.9f }, { -0.1f, -0.1f }, { 0.1f, 0.1f } };
			curve->add(pointData3);
			BezierPointData pointData4 = { { 1.0f, 0.1f }, { -0.1f, -0.1f }, { 0.1f, 0.1f } };
			curve->add(pointData4);
		}

		curve = curvesModel->createCurve(CurveTypes::CubicBezier, true);
		{
			BezierPointData pointData1 = { { 0.0f, 0.75f }, { 0.00f, 0.00f }, { 0.1f, 0.1f } };
			curve->add(pointData1);
			BezierPointData pointData2 = { { 1.0f, 0.25f }, { -0.1f, -0.1f }, { 0.0f, 0.0f } };
			curve->add(pointData2);
		}
		
		auto uiApplication = contextManager.queryInterface< IUIApplication >();
		auto uiFramework = contextManager.queryInterface< IUIFramework >();

		types_.emplace_back( contextManager.registerInterface<ICurveEditor>(curvesModel.get(), false) );
		
		curvePanel_ = uiFramework->createView("plg_curve_editor/CurveEditor.qml", IUIFramework::ResourceType::Url, std::move(curvesModel));
		uiApplication->addView(*curvePanel_);

	}

	bool Finalise( IComponentContext & contextManager ) override
	{
		auto uiApplication = contextManager.queryInterface< IUIApplication >();
		if(uiApplication)
			uiApplication->removeView(*curvePanel_);
		curvePanel_.reset();
		
		return true;
	}

	void Unload( IComponentContext & contextManager ) override
	{
		for ( auto type : types_ )
		{
			contextManager.deregisterInterface(type);
		}
		Q_CLEANUP_RESOURCE(plg_curve_editor);
	}

private:
	std::vector< IInterface * > types_;
	std::unique_ptr< IView > curvePanel_;
};

PLG_CALLBACK_FUNC(CurveEditorPlugin)

