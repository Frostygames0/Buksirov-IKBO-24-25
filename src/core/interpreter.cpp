#include "interpreter.h"

#include <QStringList>
#include <QStringView>

#include "core/commands_v2/command.h"
#include "core/commands_v2/registry.h"
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

    const Command* cmd = registry.getCommand(name.toStdString());
    if (cmd == nullptr)
    {
        ctx.out.writeLine(name + u": Command not found.");
        return ExecutionResult::withStatus(ExitCode::CommandNotFound);
    }

    std::vector<std::string_view> stdArgs;
    stdArgs.reserve(tokens.size() - 1);

    std::vector<std::string> stdStrings;
    stdStrings.reserve(tokens.size() - 1);

    for (const QString& arg : tokens.mid(1))
    {
        stdStrings.push_back(arg.toStdString());
        stdArgs.push_back(stdStrings.back());
    }

    return cmd->execute(stdArgs, ctx);
}

} // namespace EmulatorMirea::Core::Interpreter