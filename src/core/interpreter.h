#pragma once

#include <QString>
#include <QStringView>

#include "core/commands_v2/registry.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class ExecutionContext;
class CommandRegistry;

namespace Interpreter
{

ExecutionResult execute(CommandRegistry& registry, QStringView input, const ExecutionContext& ctx);

} // namespace Interpreter

} // namespace EmulatorMirea::Core
