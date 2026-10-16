#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QString>
#include <QStringList>

#include "core/commands/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class CommandExecutor
{
public:
    explicit CommandExecutor();
    ExecutionResult execute(const QStringList& tokens, const ExecutionContext& ctx) const;

private:
    CommandRegistry m_registry;
};

} // namespace EmulatorMirea::Core

#endif // COMMANDEXECUTOR_H
