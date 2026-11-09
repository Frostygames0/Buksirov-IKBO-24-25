#include "interpreter.h"

#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/commands_v2/command.h"
#include "core/commands_v2/registry.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core::Interpreter
{

ExecutionResult execute(CommandRegistry& registry, std::string_view input, const ExecutionContext& ctx)
{
    const std::vector<std::string> tokens = LineParser::parse(input, ctx.envProvider);

    if (tokens.empty())
    {
        return ExecutionResult{};
    }

    const std::string& name = tokens.front();

    const Command* cmd = registry.getCommand(name);
    if (cmd == nullptr)
    {
        ctx.writeLine(name + ": Command not found.");
        return ExecutionResult::withStatus(ExitCode::CommandNotFound);
    }

    return cmd->execute(std::span<const std::string>{tokens}.subspan(1), ctx);
}

} // namespace EmulatorMirea::Core::Interpreter