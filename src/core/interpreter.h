#pragma once

#include <QString>
#include <QStringView>

#include "core/commands/registry.h"
#include "core/executioncontext.h"
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
