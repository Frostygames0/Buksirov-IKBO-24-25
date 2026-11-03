add_library(emulator_core STATIC
    commands_v2/command.h
    commands_v2/builder.h
    commands_v2/arguments.h
    commands_v2/registry.h commands_v2/registry.cpp
    commands_v2/commands.h commands_v2/commands.cpp
    
    vfs/types.h
    vfs/vfs.h
    vfs/path.h             vfs/path.cpp
    vfs/memoryvfs.h        vfs/memoryvfs.cpp
    
    workingdirectory.h     workingdirectory.cpp
    envprovider.h
    executioncontext.h
    executionresult.h
    useridentity.h
    interpreter.h          interpreter.cpp
    lineparser.h           lineparser.cpp
    shell.h                shell.cpp

    output.h
)

target_include_directories(emulator_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)