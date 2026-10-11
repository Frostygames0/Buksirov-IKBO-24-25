#include "registry.h"

#include <memory>

#include <QString>

#include "core/commands/command.h"

namespace EmulatorMirea::Commands
{

void Registry::registerCommand(std::unique_ptr<Command> command)
{
    m_commands.try_emplace(command->name(), std::move(command));
}

Command* Registry::getCommand(const QString& name) const
{
    auto it = m_commands.find(name);
    return it == m_commands.end() ? nullptr : it->second.get();
}

} // namespace EmulatorMirea::Commands
