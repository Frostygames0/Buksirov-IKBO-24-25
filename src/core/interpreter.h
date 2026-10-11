#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/commands/executor.h"
#include "core/lineparser.h"

namespace EmulatorMirea::Core
{

class Session;

class Interpreter
{
public:
    [[nodiscard]] QString execute(QStringView cmd, ISession& session) const;

private:
    LineParser m_lineParser;
    CommandExecutor m_commandExecutor;
};

} // namespace EmulatorMirea::Core

#endif // INTERPRETER_H
