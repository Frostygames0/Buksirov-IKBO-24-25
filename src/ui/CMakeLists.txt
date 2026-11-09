qt_add_library(emulator_ui STATIC
    mainwindow.h      mainwindow.cpp
    terminalwidget.h  terminalwidget.cpp
)

target_link_libraries(emulator_ui
    PUBLIC
        Qt6::Widgets
        emulator_qt_interface
)