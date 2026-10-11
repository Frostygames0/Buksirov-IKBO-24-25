#include "core/commands/executor.h"

#include <QCoreApplication>
#include <QString>
#include <QStringList>

#include "core/commands/command.h"
#include "core/session.h"

namespace EmulatorMirea::Core
{

CommandExecutor::CommandExecutor()
{
    m_registry.registerCommand(std::make_unique<StubCommand>("ls"));
    m_registry.registerCommand(std::make_unique<StubCommand>("cd"));
    m_registry.registerCommand(std::make_unique<ExitCommand>());
}

QString CommandExecutor::execute(const QStringList& tokens, ISession& session) const
{
    if (tokens.isEmpty())
        return {};

    const QString& name = tokens.first();

    const Command* cmd = m_registry.getCommand(name);
    if (cmd == nullptr)
        return name + ": command not found";

    return cmd->execute(tokens.mid(1), session);
}

} // namespace EmulatorMirea::Core
