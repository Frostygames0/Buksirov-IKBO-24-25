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

} // namespace EmulatorMirea::Core
