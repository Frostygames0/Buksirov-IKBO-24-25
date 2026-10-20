#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QString>
#include <QStringList>

#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class CommandRegistry;

class CommandExecutor
{
public:
    explicit CommandExecutor(const CommandRegistry& registry);
    [[nodiscard]] ExecutionResult execute(const QStringList& tokens, const ExecutionContext& ctx) const;

private:
    const CommandRegistry* m_registry;
};

} // namespace EmulatorMirea::Core

#endif // COMMANDEXECUTOR_H
