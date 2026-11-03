#include "registry.h"

#include <string>
#include <utility>

#include "core/commands_v2/command.h"

namespace EmulatorMirea::Core
{

void CommandRegistry::registerCommand(Command&& command)
{
    std::string name = command.name();
    m_commands.try_emplace(std::move(name), std::move(command));
}

const Command* CommandRegistry::getCommand(const std::string& name) const
{
    const auto it = m_commands.find(name);
    return it == m_commands.end() ? nullptr : &it->second;
}

std::vector<CommandRegistry::Info> CommandRegistry::allCommandsInfo() const
{
    std::vector<Info> commands;
    commands.reserve(m_commands.size());

    for (const auto& [name, cmd] : m_commands)
    {
        commands.emplace_back(name, cmd.description());
    }

    return commands;
}

} // namespace EmulatorMirea::Core
