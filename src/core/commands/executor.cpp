#include "core/commands/executor.h"

#include <QCoreApplication>
#include <QString>

#include "core/commands/command.h"

namespace EmulatorMirea::Commands
{

Executor::Executor()
{
    m_registry.registerCommand(std::make_unique<StubCommand>("ls"));
    m_registry.registerCommand(std::make_unique<StubCommand>("cd"));
    m_registry.registerCommand(std::make_unique<ExitCommand>());
}

QString Executor::execute(const QStringList& tokens, Session& session) const
{
    if (tokens.isEmpty())
        return {};

    const QString& name = tokens.first();

    Command* cmd = m_registry.getCommand(name);
    if (!cmd)
        return name + ": command not found";

    return cmd->execute(tokens.mid(1), session);
}

}
