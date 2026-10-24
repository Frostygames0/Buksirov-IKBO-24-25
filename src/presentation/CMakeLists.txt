add_library(emulator_presentation STATIC 
    terminalsession.h  terminalsession.cpp
    historynavigator.h historynavigator.cpp
)

target_include_directories(emulator_presentation
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(emulator_presentation
    PUBLIC
        Qt6::Core
)