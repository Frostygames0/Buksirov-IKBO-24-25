#include "executor.h"

#include <memory>

#include <QString>
#include <QStringList>

#include "core/commands/command.h"
#include "core/executioncontext.h"
#include "core/stream.h"

namespace EmulatorMirea::Core
{

CommandExecutor::CommandExecutor()
{
    m_registry.registerCommand(std::make_unique<StubCommand>("ls"));
    m_registry.registerCommand(std::make_unique<StubCommand>("cd"));
    m_registry.registerCommand(std::make_unique<ExitCommand>());
}

ExecutionResult CommandExecutor::execute(const QStringList& tokens, const ExecutionContext& ctx) const
{
    if (tokens.isEmpty())
        // TODO Handle it
        return ExecutionResult::withStatus(0);

    const QString& name = tokens.first();

    const Command* cmd = m_registry.getCommand(name);
    if (cmd == nullptr)
    {
        ctx.out.write(name + ": Command not found.");
        return ExecutionResult::withStatus(ExitCode::CommandNotFound);
    }

    return cmd->execute(tokens.mid(1), ctx);
}

} // namespace EmulatorMirea::Core
