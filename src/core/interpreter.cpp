#include "interpreter.h"

#include <QStringList>
#include <QStringView>

#include "core/commands/command.h"
#include "core/commands/executor.h"
#include "core/commands/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/stream.h"

namespace EmulatorMirea::Core
{

Interpreter::Interpreter(CommandRegistry& registry) : m_registry(&registry) {}

ExecutionResult Interpreter::execute(QStringView input, const ExecutionContext& ctx) const
{
    const QStringList tokens = m_lineParser.parse(input, ctx.envProvider);

    if (tokens.isEmpty())
    {
        // TODO Handle it
        return ExecutionResult::withStatus(0);
    }

    const QString& name = tokens.first();

    const Command* cmd = m_registry->getCommand(name);
    if (cmd == nullptr)
    {
        ctx.out.write(name + u": Command not found.");
        return ExecutionResult::withStatus(ExitCode::CommandNotFound);
    }

    return cmd->execute(tokens.mid(1), ctx);
}

} // namespace EmulatorMirea::Core