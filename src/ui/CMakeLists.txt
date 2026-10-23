add_library(emulator_ui STATIC
    mainwindow.h      mainwindow.cpp
    terminalwidget.h  terminalwidget.cpp
)
target_include_directories(emulator_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(emulator_ui
    PUBLIC
        emulator_core
        Qt6::Widgets
)