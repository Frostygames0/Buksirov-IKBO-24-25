qt_add_library(emulator_presentation STATIC 
    terminalpresenter.h  terminalpresenter.cpp
    historynavigator.h historynavigator.cpp
)

target_link_libraries(emulator_presentation   
    PUBLIC
        Qt6::Core
        emulator_core
        emulator_qt_interface
)