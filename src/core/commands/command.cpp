#include "command.h"

#include <QString>
#include <QStringLiteral>

#include "core/executioncontext.h"

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
        ctx.view.write(name() + u": too many arguments");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    bool ok = false;
    const int exitCode = args.value(0, QStringLiteral("0")).toInt(&ok);

    if (!ok)
    {
        ctx.view.write(name() + u": " + args.value(0) + u": numeric argument required");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    return ExecutionResult::exit(exitCode);
}

} // namespace EmulatorMirea::Core
