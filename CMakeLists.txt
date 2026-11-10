cmake_minimum_required(VERSION 3.19)
project(EmulatorMirea LANGUAGES CXX)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)

if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# Flags for all sub-projects
add_library(emulator_interface INTERFACE)
target_include_directories(emulator_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(emulator_interface INTERFACE 
    # General
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive->
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wshadow -Wnon-virtual-dtor>

    # Debug
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Od /Ob0 /Zi /RTC1>
    $<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>,$<CONFIG:Debug>>:-O0 -g>

    # Release
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2 /Oi /Gy>
    $<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>,$<CONFIG:Release>>:-O2>
)

# Flags for sub-projects that use Qt
add_library(emulator_qt_interface INTERFACE)
target_link_libraries(emulator_qt_interface INTERFACE emulator_interface)
target_compile_definitions(emulator_qt_interface INTERFACE QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY)

add_subdirectory(src/core)
add_subdirectory(src/platform)
add_subdirectory(src/presentation)
add_subdirectory(src/ui)

qt_add_executable(emulator_app
    WIN32
    src/main.cpp
)

qt_add_resources(emulator_app "resources"
    PREFIX  "/"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    FILES
        "resources/icon.png"
        "resources/JetBrainsMono-Regular.ttf"
)

target_link_libraries(emulator_app
    PRIVATE
        Qt::Core
        Qt::Widgets
        emulator_core
        emulator_platform
        emulator_presentation
        emulator_ui
    PUBLIC
        emulator_interface
)

install(TARGETS emulator_app
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if (WIN32)
    set(DEPLOY_TOOL_OPTIONS 
        --no-opengl-sw 
        --no-system-d3d-compiler 
        --skip-plugin-types generic,iconengines,networkinformation,tls,styles,imageformats 
        --exclude-plugins qsvg)
endif()

qt_generate_deploy_app_script(
    TARGET emulator_app
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
    DEPLOY_TOOL_OPTIONS ${DEPLOY_TOOL_OPTIONS}
)

install(SCRIPT ${deploy_script})
