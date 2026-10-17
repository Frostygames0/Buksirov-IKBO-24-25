#ifndef COMMANDREGISTRY_H
#define COMMANDREGISTRY_H

#include <memory>
#include <unordered_map>

#include <QString>

#include "core/commands/command.h"

namespace EmulatorMirea::Core
{

class CommandRegistry
{
public:
    void registerCommand(std::unique_ptr<Command> command);
    Command* getCommand(const QString& name) const;

private:
    std::unordered_map<QString, std::unique_ptr<Command>> m_commands;
};

} // namespace EmulatorMirea::Core
#endif // COMMANDREGISTRY_H
