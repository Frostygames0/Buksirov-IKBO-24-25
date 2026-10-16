#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/commands/executor.h"
#include "core/executionresult.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core
{

struct ExecutionContext;

class Interpreter
{
public:
    [[nodiscard]] ExecutionResult execute(QStringView cmd, const ExecutionContext& ctx) const;

private:
    LineParser m_lineParser;
    CommandExecutor m_commandExecutor;
};

} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
