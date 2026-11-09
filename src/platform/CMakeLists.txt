qt_add_library(emulator_platform STATIC
    scriptreader.h       scriptreader.cpp
    xmlvfsreader.h       xmlvfsreader.cpp
    userutil.h           userutil.cpp
    systemenvprovider.h  systemenvprovider.cpp
)

target_link_libraries(emulator_platform
    PUBLIC
        Qt6::Core
        emulator_interface
)