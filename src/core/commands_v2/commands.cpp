#include "commands.h"

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

constexpr Command createExit()
{
    return CommandBuilder{}
        .name("exit")
        .description("exits the terminal with provided exit code")
        .args(ArgumentBuilder{}.positional<int>("exit_code", "Exit code", 0).buildSet())
        .build([](const ExecutionContext&, int code) { return ExecutionResult::exit(code); });
}

constexpr Command createList()
{
    return CommandBuilder{}
        .name("ls")
        .description("lists all files")
        .args(ArgumentBuilder{}.positional("path", "path to list", {}).buildSet())
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

constexpr Command createChangeDirectory()
{
    return CommandBuilder{}
        .name("cd")
        .description("changes the current working directory")
        .args(ArgumentBuilder{}.positional("dir", "directory to change to", {}).buildSet())
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

constexpr Command createClear()
{
    return CommandBuilder{}
        .name("clear")
        .description("clears the terminal screen")
        .build([](const ExecutionContext& ctx) {
            ctx.out.clear();
            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

constexpr Command createEcho()
{
    return CommandBuilder{}
        .name("clear")
        .description("echoes the input")
        .args(ArgumentBuilder{}.positional("input", "the input to echo", {}).buildSet())
        .build([](const ExecutionContext& ctx, const std::string& input) {
            ctx.out.writeLine(input);
            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

constexpr Command createHelp()
{
    return CommandBuilder{}
        .name("help")
        .description("outputs the list of all commands")
        .build([](const ExecutionContext& ctx) {
            ctx.writeLine("List of available commands:");
            for (const CommandRegistry::Info& info : ctx.commandRegistry.allCommandsInfo())
            {
                ctx.writeLine(info.toString());
            }

            return ExecutionResult::withStatus(ExitCode::Success);
        });
}

constexpr Command createRemove()
{
    return CommandBuilder{}
        .name("rm")
        .description("removes")
        .args(ArgumentBuilder{}.flag('r', "recursive", "").positional("path", "to remove").buildSet())
        .build([](const ExecutionContext& ctx, bool recurive, const std::string& pathStr) {
            auto path = ctx.cwd.resolve(pathStr);
            if (!path)
            {
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            auto stat = ctx.vfs.stat(*path);
            if (!stat)
            {
                return ExecutionResult::withStatus(ExitCode::GeneralError);
            }

            auto res = ctx.vfs.remove(*path, recurive ? VfsRemoveMode::Recursive : VfsRemoveMode::Single);
            if (!res)
            {
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