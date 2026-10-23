#include "command.h"

#include <QString>
#include <QStringLiteral>

#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/output.h"
#include "core/vfs/path.h"
#include "core/vfs/types.h"
#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core
{

QString ExitCommand::name() const
{
    return QStringLiteral("exit");
}

ExecutionResult ExitCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.out.writeLine(name() + u": too many arguments");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    bool ok = false;
    const int exitCode = args.value(0, QStringLiteral("0")).toInt(&ok);

    if (!ok)
    {
        ctx.out.writeLine(name() + u": " + args.value(0) + u": numeric argument required");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    return ExecutionResult::exit(exitCode);
}

QString ListCommand::name() const
{
    return QStringLiteral("ls");
}

ExecutionResult ListCommand::execute(const QStringList&, const ExecutionContext& ctx) const
{
    VfsPath path = ctx.cwd;

    auto entries = ctx.vfs.list(path);
    if (!entries)
    {
        ctx.out.writeLine(QStringLiteral("Fail!"));
        return ExecutionResult::withStatus(1);
    }

    for (const VfsEntry& entry : *entries)
    {
        ctx.out.writeLine(entry.name);
    }

    return ExecutionResult::withStatus(0);
}

} // namespace EmulatorMirea::Core
