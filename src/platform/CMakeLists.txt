add_library(emulator_platform STATIC
    scriptreader.h scriptreader.cpp
    xmlvfsreader.h xmlvfsreader.cpp
    userutil.h     userutil.cpp
)

target_include_directories(emulator_platform
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(emulator_platform
    PUBLIC
        emulator_core
        Qt6::Core
)