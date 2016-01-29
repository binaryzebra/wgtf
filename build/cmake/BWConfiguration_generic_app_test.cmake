SET( BW_SINGLETON_MANAGER_SUPPORT ON )
SET( NGT_SUPPORT ON )
SET( DEPLOY_QT_SUPPORT ON )

ADD_DEFINITIONS( -DEDITOR_ENABLED )
ADD_DEFINITIONS( -DALLOW_STACK_CONTAINER )

SET( TARGET_PLUGIN_APP_FOLDER_NAME generic_app_test )
SET( BW_BUNDLE_NAME generic_app )

INCLUDE( projects/core/WGToolsCore )
INCLUDE( projects/core/WGToolsCoreTests )

BW_GENERATE_DOC( WGToolsCore ${WG_TOOLS_SOURCE_DIR}/../doc/core/Doxyfile.in ${WG_TOOLS_SOURCE_DIR}/../doc/core )

SET( NGT_PLUGIN_CONFIGURATION_FILES
	config/testing/plugins.txt
	config/testing/plugins_ui.txt
)
