#include "command.h"

#include <expected>
#include <utility>

#include <QString>
#include <QStringList>

#include "core/commands/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/vfs/types.h"
#include "core/vfs/vfs.h"
#include "core/workingdirectory.h"

namespace EmulatorMirea::Core
{

QString ExitCommand::name() const
{
    return QStringLiteral("exit");
}

QString ExitCommand::description() const
{
    return QStringLiteral("terminates terminal with an exit code");
};

ExecutionResult ExitCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    bool ok = false;
    const int exitCode = args.value(0, QStringLiteral("0")).toInt(&ok);

    if (!ok)
    {
        ctx.writeLine(formatOutput(u"numeric argument required"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    return ExecutionResult::exit(exitCode);
}

QString ListCommand::name() const
{
    return QStringLiteral("ls");
}

QString ListCommand::description() const
{
    return QStringLiteral("lists files of a directory");
};

ExecutionResult ListCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    QString pathStr = args.empty() ? QString{} : args.at(0);

    auto resolved = ctx.cwd.resolve(pathStr);
    if (!resolved)
    {
        ctx.writeLine(formatOutput(u"path " + resolved.error().toString(pathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    auto stat = ctx.vfs.stat(*resolved);
    if (!stat)
    {
        ctx.writeLine(formatOutput(u"path " + stat.error().toString(pathStr)));
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
}

VfsInitCommand::VfsInitCommand(QString vfsLoadPath) : m_vfsLoadPath(std::move(vfsLoadPath)) {}

QString VfsInitCommand::name() const
{
    return QStringLiteral("vfs-init");
}

QString VfsInitCommand::description() const
{
    return QStringLiteral("initializes (resets) VFS");
};

ExecutionResult VfsInitCommand::execute(const QStringList&, const ExecutionContext& ctx) const
{
    ctx.vfs.clear();
    return ExecutionResult::withStatus(ExitCode::Success);
}

QString ChangeDirectoryCommand::name() const
{
    return QStringLiteral("cd");
}

QString ChangeDirectoryCommand::description() const
{
    return QStringLiteral("changes current directory");
};

ExecutionResult ChangeDirectoryCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    QString pathStr = args.empty() ? QString{} : args.at(0);
    auto resolved = ctx.cwd.resolve(pathStr);
    if (!resolved)
    {
        ctx.writeLine(formatOutput(u"path " + resolved.error().toString(pathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    auto res = ctx.cwd.set(*resolved);
    if (!res)
    {
        ctx.writeLine(formatOutput(u"path " + res.error().toString(pathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    return ExecutionResult::withStatus(ExitCode::Success);
}

QString ClearCommand::name() const
{
    return QStringLiteral("clear");
}

QString ClearCommand::description() const
{
    return QStringLiteral("clears the screen of the terminal");
};

ExecutionResult ClearCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (!args.empty())
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    ctx.out.clear();
    return ExecutionResult::withStatus(ExitCode::Success);
}

QString EchoCommand::name() const
{
    return QStringLiteral("echo");
}

QString EchoCommand::description() const
{
    return QStringLiteral("prints its input");
};

ExecutionResult EchoCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.isEmpty())
    {
        ctx.writeLine();
        return ExecutionResult::withStatus(ExitCode::Success);
    }

    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    ctx.writeLine(args.at(0));
    return ExecutionResult::withStatus(ExitCode::Success);
}

QString HelpCommand::name() const
{
    return QStringLiteral("help");
}

QString HelpCommand::description() const
{
    return QStringLiteral("displays a list of all available commands");
};

ExecutionResult HelpCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    ctx.writeLine(QStringLiteral("List of available commands:"));
    for (const CommandRegistry::Info& info : ctx.commandRegistry.allCommandsInfo())
    {
        ctx.writeLine(info.toString());
    }

    return ExecutionResult::withStatus(ExitCode::Success);
}

} // namespace EmulatorMirea::Core
