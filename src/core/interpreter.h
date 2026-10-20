#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>
#include "core/executionresult.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core
{

class CommandExecutor;
struct ExecutionContext;

class Interpreter
{
public:
    explicit Interpreter(CommandExecutor& commandExecutor);

    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;

    Interpreter(Interpreter&&) = delete;
    Interpreter& operator=(Interpreter&&) = delete;

    [[nodiscard]] ExecutionResult execute(QStringView cmd, const ExecutionContext& ctx) const;

private:
    LineParser m_lineParser;
    CommandExecutor& m_commandExecutor;
};

} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
