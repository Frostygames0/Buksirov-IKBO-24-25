#include "core/commands/command.h"

#include <QCoreApplication>

namespace EmulatorMirea::Commands
{

StubCommand::StubCommand(const QString& name) : m_name(name) {}

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
    return "exit";
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

}
