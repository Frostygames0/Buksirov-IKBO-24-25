#include "command.h"

#include <expected>
#include <qnamespace.h>
#include <qtypes.h>
#include <utility>

#include <QRegularExpression>
#include <QSet>
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

ExecutionResult VfsInitCommand::execute(const QStringList&, const ExecutionContext& ctx) const
{
    ctx.vfs.clear();
    return ExecutionResult::withStatus(ExitCode::Success);
}

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

ExecutionResult RemoveCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 2)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    VfsRemoveMode mode = VfsRemoveMode::Single;

    QString pathStr;
    for (const QString& arg : args)
    {
        if (arg == u"-r")
        {
            mode = VfsRemoveMode::Recursive;
        }
        else
        {
            if (!pathStr.isEmpty())
            {
                ctx.writeLine(formatOutput(u"redundant path: " + arg));
                return ExecutionResult::withStatus(ExitCode::Misuse);
            }

            pathStr = arg;
        }
    }

    if (pathStr.isEmpty())
    {
        ctx.writeLine(formatOutput(u"removal path was not specified"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    auto resolved = resolveAndStatOrStop(pathStr, ctx);
    if (!resolved)
    {
        return resolved.error();
    }

    auto res = ctx.vfs.remove(*resolved, mode);
    if (!res)
    {
        ctx.writeLine(formatOutput(u"error while removing " + res.error().toString(pathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    return ExecutionResult::withStatus(ExitCode::Success);
}

std::expected<VfsPath, ExecutionResult> Command::resolveAndStatOrStop(const QString& str,
                                                                      const ExecutionContext& ctx) const
{
    auto resolvedSrc = ctx.cwd.resolve(str);
    auto srcRes = resolvedSrc.and_then([&ctx](const VfsPath& path) { return ctx.vfs.stat(path); });
    if (!srcRes)
    {
        ctx.writeLine(formatOutput(u"path " + srcRes.error().toString(str)));
        return std::unexpected(ExecutionResult::withStatus(ExitCode::GeneralError));
    }

    return *resolvedSrc;
}

ExecutionResult MoveCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 2)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    if (args.isEmpty())
    {
        ctx.writeLine(formatOutput(u"source was not specified"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    const QString& srcPathStr = args.at(0);
    auto resolvedSrc = resolveAndStatOrStop(srcPathStr, ctx);
    if (!resolvedSrc)
    {
        return resolvedSrc.error();
    }

    if (args.size() == 1)
    {
        ctx.writeLine(formatOutput(u"destination was not specified"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    const QString& dstPathStr = args.at(1);
    auto resolvedDst = resolveAndStatOrStop(dstPathStr, ctx);
    if (!resolvedDst)
    {
        return resolvedDst.error();
    }

    auto res = ctx.vfs.move(*resolvedSrc, *resolvedDst);
    if (!res)
    {
        ctx.writeLine(res.error().toString());
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    return ExecutionResult::withStatus(ExitCode::Success);
}

ExecutionResult WcCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    if (args.isEmpty())
    {
        ctx.writeLine(formatOutput(u"file was not specified"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    const QString& srcPathStr = args.at(0);
    auto resolvedSrc = resolveAndStatOrStop(srcPathStr, ctx);
    if (!resolvedSrc)
    {
        return resolvedSrc.error();
    }

    auto file = ctx.vfs.readFile(*resolvedSrc);
    if (!file)
    {
        ctx.writeLine(formatOutput(file.error().toString(srcPathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    QString parsed = QString::fromUtf8(*file);
    qsizetype count = parsed.split(QRegularExpression(QStringLiteral("\\s+")), Qt::SkipEmptyParts).size();
    ctx.writeLine(QStringLiteral("%1 words").arg(count));

    return ExecutionResult::withStatus(ExitCode::Success);
}

ExecutionResult UniqCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.writeLine(formatOutput(u"too many arguments"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    if (args.isEmpty())
    {
        ctx.writeLine(formatOutput(u"file was not specified"));
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    const QString& srcPathStr = args.at(0);
    auto resolvedSrc = resolveAndStatOrStop(srcPathStr, ctx);
    if (!resolvedSrc)
    {
        return resolvedSrc.error();
    }

    auto file = ctx.vfs.readFile(*resolvedSrc);
    if (!file)
    {
        ctx.writeLine(formatOutput(file.error().toString(srcPathStr)));
        return ExecutionResult::withStatus(ExitCode::GeneralError);
    }

    QString parsed = QString::fromUtf8(*file);
    QStringList lines = parsed.split(u'\n', Qt::SkipEmptyParts);
    QSet<QString> uniqueLines(lines.begin(), lines.end());

    ctx.writeLine(QStringLiteral("%1 lines").arg(uniqueLines.size()));
    return ExecutionResult::withStatus(ExitCode::Success);
}

} // namespace EmulatorMirea::Core
