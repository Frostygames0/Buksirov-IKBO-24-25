#include "commands.h"

#include <format>
#include <string>

#include "core/commands_v2/arguments.h"
#include "core/commands_v2/builder.h"
#include "core/commands_v2/command.h"
#include "core/commands_v2/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/vfs/path.h"
#include "core/vfs/types.h"
#include "core/vfs/vfs.h"
#include "core/workingdirectory.h"

namespace EmulatorMirea::Core::Commands
{

namespace
{

Command createExit()
{
    return CommandBuilder{}
        .name("exit")
        .description("exits the terminal with the provided exit code")
        .args(ArgumentBuilder{}.positional<int>("exit_code", "Exit code", 0).buildSet())
        .build([](const ExecutionContext&, int code) { return ExecutionResult::exit(code); });
}

Command createList()
{
    return CommandBuilder{}
        .name("ls")
        .description("lists all files/directories at the specified path")
        .args(ArgumentBuilder{}.positional<std::string>("path", "a path to list", {}).buildSet())
        .build([](const ExecutionContext& ctx, const std::string& pathStr) {
            auto resolved = ctx.cwd.resolve(pathStr);
            if (!resolved)
            {
                ctx.writeLine("path " + resolved.error().toString(pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            auto stat = ctx.vfs.stat(*resolved);
            if (!stat)
            {
                ctx.writeLine("path " + stat.error().toString(pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            ctx.writeLine(resolved->toString());
            if (stat->isFile())
            {
                ctx.writeLine(stat->toString());
                return ExecutionResult::withStatus(ExitCode::Success);
            }

            auto entries = ctx.vfs.list(*resolved);
            for (const VfsEntry& entry : *entries)
            {
                ctx.writeLine(entry.toString());
            }

            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

Command createChangeDirectory()
{
    return CommandBuilder{}
        .name("cd")
        .description("changes the current working directory")
        .args(ArgumentBuilder{}.positional<std::string>("dir", "a path to the directory", {}).buildSet())
        .build([](const ExecutionContext& ctx, const std::string& pathStr) {
            auto resolved = ctx.cwd.resolve(pathStr);
            if (!resolved)
            {
                ctx.writeLine("path " + resolved.error().toString(pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            auto res = ctx.cwd.set(*resolved);
            if (!res)
            {
                ctx.writeLine("path " + res.error().toString(pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

Command createClear()
{
    return CommandBuilder{}
        .name("clear")
        .description("clears the terminal screen")
        .build([](const ExecutionContext& ctx) {
            ctx.out.clear();
            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

Command createEcho()
{
    return CommandBuilder{}
        .name("echo")
        .description("echoes the input")
        .args(ArgumentBuilder{}.positional<std::string>("input", "input to echo", {}).buildSet())
        .build([](const ExecutionContext& ctx, const std::string& input) {
            ctx.out.writeLine(input);
            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

Command createHelp()
{
    return CommandBuilder{}
        .name("help")
        .description("outputs the list of all available commands")
        .build([](const ExecutionContext& ctx) {
            ctx.writeLine("All available commands:");
            for (const CommandRegistry::Info& info : ctx.commandRegistry.allCommandsInfo())
            {
                ctx.writeLine(info.toString());
            }

            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

Command createRemove()
{
    return CommandBuilder{}
        .name("rm")
        .description("removes files/directory at the specified path")
        .args(ArgumentBuilder{}
                  .flag('r', "recursive", "removes everything at the path recursively")
                  .positional<std::string>("path", "a path to the file/directory to remove")
                  .buildSet())
        .build([](const ExecutionContext& ctx, bool recurive, const std::string& pathStr) {
            auto path = ctx.cwd.resolve(pathStr);
            if (!path)
            {
                ctx.writeLine(std::format("path \"{}\" is not valid", pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            auto stat = ctx.vfs.stat(*path);
            if (!stat)
            {
                ctx.writeLine(std::format("\"{}\" doesn't exist", pathStr));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }
            auto res = ctx.vfs.remove(*path, recurive ? VfsRemoveMode::Recursive : VfsRemoveMode::Single);
            if (!res)
            {
                ctx.writeLine(std::format(
                    "failed to remove at \"{}\": {}", pathStr, vfsErrorCodeToStr(res.error().code)));
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            return ExecutionResult::withStatus(ExitCode::Success);
        });
}
} // namespace

CommandRegistry createDefaultRegistry()
{
    CommandRegistry registry;
    registry.registerCommand(createExit());
    registry.registerCommand(createList());
    registry.registerCommand(createChangeDirectory());
    registry.registerCommand(createClear());
    registry.registerCommand(createEcho());
    registry.registerCommand(createHelp());
    registry.registerCommand(createRemove());

    return registry;
}

} // namespace EmulatorMirea::Core::Commands