#include "command.h"

#include <utility>

#include <QString>
#include <QStringLiteral>

#include "core/stream.h"

namespace EmulatorMirea::Core
{

StubCommand::StubCommand(QString name) : m_name(std::move(name)) {}

QString StubCommand::name() const
{
    return m_name;
}

int StubCommand::execute(const QStringList& args, const CommandContext& ctx) const
{
    ctx.out.write(m_name + " " + args.join(' '));
    return ExitCode::Success;
}

QString ExitCommand::name() const
{
    return QStringLiteral("exit");
}

int ExitCommand::execute(const QStringList& args, const CommandContext& ctx) const
{
    if (args.size() > 1)
    {
        ctx.out.write(name() + ": too many arguments");
        return ExitCode::Misuse;
    }

    bool ok = false;
    const int exitCode = args.value(0, "0").toInt(&ok);

    if (!ok)
    {
        ctx.out.write(name() + ": " + args.value(0) + ": numeric argument required");
        return ExitCode::Misuse;
    }

    if (ctx.requestExit)
        ctx.requestExit(exitCode);

    return exitCode;
}

} // namespace EmulatorMirea::Core
