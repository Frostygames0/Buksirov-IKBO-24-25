add_library(emulator_core STATIC
    commands/command.h   commands/command.cpp
    commands/registry.h  commands/registry.cpp
    
    vfs/types.h
    vfs/path.h           vfs/path.cpp
    vfs/vfs.h            vfs/vfs.cpp
    vfs/xmlvfsreader.h   vfs/xmlvfsreader.cpp

    envprovider.h
    executioncontext.h
    executionresult.h
    useridentity.h
    interpreter.h        interpreter.cpp
    lineparser.h         lineparser.cpp
    shell.h              shell.cpp
    systemenvprovider.h  systemenvprovider.cpp

    historynavigator.h   historynavigator.cpp
    terminalsession.h    terminalsession.cpp
    output.h
)

target_include_directories(emulator_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(emulator_core
    PUBLIC
        Qt6::Core
)