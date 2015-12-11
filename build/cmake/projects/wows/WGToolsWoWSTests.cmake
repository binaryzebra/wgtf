SET( CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_MODULE_PATH}
)

INCLUDE_DIRECTORIES( ${WG_TOOLS_SOURCE_DIR}/wows )
INCLUDE_DIRECTORIES( ${WG_TOOLS_SOURCE_DIR}/wows/lib )

SET( NGT_PLUGIN_CONFIGURATION_FILES
	config/testing/*.txt
	../../../wows/app/generic_app/config/testing/*.txt
)

