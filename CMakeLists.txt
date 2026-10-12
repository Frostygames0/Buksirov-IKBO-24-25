cmake_minimum_required(VERSION 3.19)
project(EmulatorMirea LANGUAGES CXX)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTORCC ON)

qt_add_executable(EmulatorMirea
    WIN32

    src/main.cpp


    # core: shell logic, no widgets
    src/core/interpreter.h       src/core/interpreter.cpp
    src/core/envprovider.h
    src/core/session.h           src/core/session.cpp
    src/core/lineparser.h        src/core/lineparser.cpp
    src/core/shell.h             src/core/shell.cpp

    # core/commands
    src/core/commands/executor.h src/core/commands/executor.cpp
    src/core/commands/registry.h src/core/commands/registry.cpp
    src/core/commands/command.h  src/core/commands/command.cpp

    # ui: widgets
    src/ui/mainwindow.h          src/ui/mainwindow.cpp
    src/ui/terminalwidget.h      src/ui/terminalwidget.cpp

    # utils
    src/utils/user.h             src/utils/user.cpp

    resources/resources.qrc
)

target_include_directories(EmulatorMirea PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(EmulatorMirea
    PRIVATE
        Qt::Core
        Qt::Widgets
)

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
