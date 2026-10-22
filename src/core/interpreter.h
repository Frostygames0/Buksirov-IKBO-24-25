#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>
#include "core/commands/registry.h"
#include "core/executionresult.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core
{

class CommandExecutor;
struct ExecutionContext;

class Interpreter
{
public:
    explicit Interpreter(CommandRegistry& commandExecutor);

    [[nodiscard]] ExecutionResult execute(QStringView input, const ExecutionContext& ctx) const;

private:
    LineParser m_lineParser;
    CommandRegistry* m_registry;
};

} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
