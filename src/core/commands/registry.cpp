#include "registry.h"

#include <memory>
#include <utility>

#include <QString>

#include "core/commands/command.h"

namespace EmulatorMirea::Core
{

void CommandRegistry::registerCommand(std::unique_ptr<Command> command)
{
    m_commands.try_emplace(command->name(), std::move(command));
}

Command* CommandRegistry::getCommand(const QString& name) const
{
    const auto it = m_commands.find(name);
    return it == m_commands.end() ? nullptr : it->second.get();
}

QList<CommandRegistry::Info> CommandRegistry::allCommandsInfo() const
{
    QList<Info> commands;
    for (const auto& [name, cmd] : m_commands)
    {
        commands.append({.name = name, .desciption = cmd->description()});
    }

    return commands;
}

} // namespace EmulatorMirea::Core
