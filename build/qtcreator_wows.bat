REM When start qtcreator for the first time, it requires run cmake, use the following arguments for cmake command:
REM
REM -Wno-dev -G"Visual Studio 11 Win64" -DBW_CMAKE_TARGET=generic_app_test -DBW_VERIFY_LINUX=OFF
REM
REM Also choose "NMake Generator (Qt 5.3.1)"

SET BW_CMAKE_TARGE=DBW_CMAKE_TARGET=generic_app_test_wows
SET BW_VERIFY_LINUX=OFF
SET QML_IMPORT_PATH=%CD%\..\src\core\plugins\plg_qt_common\resources
START /B qtcreator.exe %CD%\..\src\CMakeLists.txt
