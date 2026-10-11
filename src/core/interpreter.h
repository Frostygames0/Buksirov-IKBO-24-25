#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/commands/executor.h"
#include "core/session.h"
#include "core/tokenizer.h"

namespace EmulatorMirea
{

class Session;

class Interpreter
{
public:
    [[nodiscard]] QString execute(QStringView cmd, Session& session) const;

private:
    Tokenizer m_tokenizer;
    Commands::Executor m_commandExecutor;
};

} // namespace EmulatorMirea

#endif // INTERPRETER_H
