#include "interpreter.h"

#include <QStringList>
#include <QStringView>

#include "core/commands/command.h"
#include "core/commands/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/lineparser.h"
#include "core/output.h"

namespace EmulatorMirea::Core::Interpreter
{

ExecutionResult execute(CommandRegistry& registry, QStringView input, const ExecutionContext& ctx)
{
    const QStringList tokens = LineParser::parse(input, ctx.envProvider);

    if (tokens.isEmpty())
    {
        return ExecutionResult::withStatus(ExitCode::None);
    }

    const QString& name = tokens.first();

    const Command* cmd = registry.getCommand(name);
    if (cmd == nullptr)
    {
        ctx.out.writeLine(name + u": Command not found.");
        return ExecutionResult::withStatus(ExitCode::CommandNotFound);
    }

    return cmd->execute(tokens.mid(1), ctx);
}

} // namespace EmulatorMirea::Core::Interpreter