add_library(emulator_presentation STATIC 
    terminalpresenter.h  terminalpresenter.cpp
    historynavigator.h historynavigator.cpp
)

target_include_directories(emulator_presentation
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(emulator_presentation   
    PUBLIC
        emulator_core
        Qt6::Core
)