SET( CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_MODULE_PATH}
)

INCLUDE_DIRECTORIES( ${WG_TOOLS_SOURCE_DIR}/core )
INCLUDE_DIRECTORIES( ${WG_TOOLS_SOURCE_DIR}/core/lib )

LIST( APPEND BW_LIBRARY_PROJECTS
	#WG Systems
	wg_types			core/lib/wg_types
	wg_memory			core/lib/wg_memory

	#NGT Systems
	core_common		        core/lib/core_common
	core_variant		    core/lib/core_variant
    core_dependency_system	core/lib/core_dependency_system
    core_command_system		core/lib/core_command_system
    core_control_system		core/lib/core_control_system
    core_reflection			core/lib/core_reflection
    core_reflection_utils	core/lib/core_reflection_utils
    core_serialization		core/lib/core_serialization
    core_string_utils		core/lib/core_string_utils
    core_copy_paste			core/lib/core_copy_paste

	#Tools Common
	core_logging				core/lib/core_logging
	core_logging_system		    core/lib/core_logging_system
	core_generic_plugin		    core/lib/core_generic_plugin
	core_generic_plugin_manager	core/lib/core_generic_plugin_manager
	core_qt_common			    core/lib/core_qt_common
	core_qt_script			    core/lib/core_qt_script
	core_data_model			    core/lib/core_data_model
	core_ui_framework		    core/lib/core_ui_framework

)

LIST( APPEND BW_BINARY_PROJECTS
	# Apps
	generic_app			core/app/generic_app
)

IF ( {BW_PLATFORM} STREQUAL "win64" )
    LIST( APPEND BW_BINARY_PROJECTS
        maya_plugin		    core/app/maya_plugin
    )
ENDIF()

LIST( APPEND BW_PLUGIN_PROJECTS
	# Plugins
	plg_automation				core/plugins/plg_automation
	plg_reflection				core/plugins/plg_reflection
	plg_command_system			core/plugins/plg_command_system
	plg_variant				    core/plugins/plg_variant
	plg_editor_interaction		core/plugins/plg_editor_interaction
	plg_control_system			core/plugins/plg_control_system
	plg_history_ui				core/plugins/plg_history_ui
	plg_macros_ui				core/plugins/plg_macros_ui
	plg_qt_app					core/plugins/plg_qt_app
	plg_qt_common				core/plugins/plg_qt_common
	plg_serialization			core/plugins/plg_serialization
	plg_logging_system			core/plugins/plg_logging_system
	plg_idedebug_logger			core/plugins/plg_idedebug_logger
	plg_alert_ui				core/plugins/plg_alert_ui
	plg_file_system				core/plugins/plg_file_system
	plg_perforce				core/plugins/plg_perforce
	plg_panel_manager			core/plugins/plg_panel_manager

	plg_copy_paste				core/plugins/plg_copy_paste
	
)
