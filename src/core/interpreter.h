#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{

class ExecutionContext;

namespace Interpreter
{

ExecutionResult execute(QStringView input, const ExecutionContext& ctx);

} // namespace Interpreter
} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
