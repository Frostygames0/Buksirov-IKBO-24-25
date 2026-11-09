#pragma once

#include <string_view>

#include "core/commands_v2/registry.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

struct ExecutionContext;
class CommandRegistry;

namespace Interpreter
{

ExecutionResult execute(CommandRegistry& registry, std::string_view input, const ExecutionContext& ctx);

} // namespace Interpreter

} // namespace EmulatorMirea::Core
