#include "command.h"

#include <QString>
#include <QStringLiteral>

#include "core/executioncontext.h"
#include "core/stream.h"

namespace EmulatorMirea::Core
{

StubCommand::StubCommand(const QString& name) : m_name(name) {}

QString StubCommand::name() const
{
    return m_name;
}

ExecutionResult StubCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    ctx.out.write(m_name + u' ' + args.join(u' '));
    return ExecutionResult::withStatus(ExitCode::Success);
}

QString ExitCommand::name() const
{
    return QStringLiteral("exit");
}

ExecutionResult ExitCommand::execute(const QStringList& args, const ExecutionContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.out.write(name() + u": too many arguments");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    bool ok = false;
    const int exitCode = args.value(0, QStringLiteral("0")).toInt(&ok);

    if (!ok)
    {
        ctx.out.write(name() + u": " + args.value(0) + u": numeric argument required");
        return ExecutionResult::withStatus(ExitCode::Misuse);
    }

    return ExecutionResult::exit(exitCode);
}

} // namespace EmulatorMirea::Core
