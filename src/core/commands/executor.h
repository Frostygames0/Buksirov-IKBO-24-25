#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QString>
#include <QStringList>

#include "core/commands/registry.h"

namespace EmulatorMirea::Core
{

class CommandExecutor
{
public:
    explicit CommandExecutor();
    int execute(const QStringList& tokens, const CommandContext& ctx) const;

private:
    CommandRegistry m_registry;
};

} // namespace EmulatorMirea::Core

#endif // COMMANDEXECUTOR_H
