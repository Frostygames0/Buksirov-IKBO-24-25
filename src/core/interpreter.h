#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/commands/executor.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core
{

struct CommandContext;

class Interpreter
{
public:
    [[nodiscard]] int execute(QStringView cmd, const CommandContext& ctx) const;

private:
    LineParser m_lineParser;
    CommandExecutor m_commandExecutor;
};

} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
