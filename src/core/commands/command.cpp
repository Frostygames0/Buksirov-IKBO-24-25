#include "core/commands/command.h"

#include <QCoreApplication>
#include <QStringLiteral>
#include <QString>

#include <utility>

#include "core/session.h"

namespace EmulatorMirea::Commands
{

StubCommand::StubCommand(QString name) : m_name(std::move(name)) {}

QString StubCommand::name() const
{
    return m_name;
}

QString StubCommand::execute(const QStringList& args, Session&) const
{
    return m_name + " " + args.join(' ');
}

QString ExitCommand::name() const
{
    return QStringLiteral("exit");
}

QString ExitCommand::execute(const QStringList& args, Session&) const
{
    if (args.size() > 1)
        return name() + ": too many arguments";

    bool ok = false;
    const int exitCode = args.value(0, "0").toInt(&ok);

    if (!ok)
        return name() + ": " + args.value(0) + ": numeric argument required";

    QCoreApplication::exit(exitCode & 0xFF);
    return {};
}

} // namespace EmulatorMirea::Commands
