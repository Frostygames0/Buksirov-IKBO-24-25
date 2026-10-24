cmake_minimum_required(VERSION 3.19)
project(EmulatorMirea LANGUAGES CXX)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src/core)
add_subdirectory(src/presentation)
add_subdirectory(src/ui)

qt_add_executable(EmulatorMirea
    WIN32

    src/main.cpp
    src/userutil.h src/userutil.cpp

    resources/resources.qrc
)

target_include_directories(EmulatorMirea PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(EmulatorMirea
    PRIVATE
        Qt::Core
        Qt::Widgets
        emulator_core
        emulator_ui
)

add_compile_definitions(QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY)

install(TARGETS EmulatorMirea
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET EmulatorMirea
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
    DEPLOY_TOOL_OPTIONS --no-opengl-sw --no-system-d3d-compiler
)

install(SCRIPT ${deploy_script})
