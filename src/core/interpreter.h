#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/session.h"
#include "core/tokenizer.h"
#include "core/commands/executor.h"

namespace EmulatorMirea
{

class Interpreter
{
public:
    QString execute(QStringView cmd, Session& session) const;
private:
    Tokenizer m_tokenizer;
    Commands::Executor m_commandExecutor;
};

}

#endif // INTERPRETER_H
