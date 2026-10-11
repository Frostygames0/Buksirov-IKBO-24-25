#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QStringView>

#include "core/envexpander.h"
#include "core/tokenizer.h"

#include "core/commands/executor.h"

namespace EmulatorMirea
{

class Session;

class Interpreter
{
public:
    QString execute(QStringView cmd, Session& session) const;
private:
    EnvExpander m_envExpander;
    Tokenizer m_tokenizer;
    Commands::Executor m_commandExecutor;
};

}

#endif // INTERPRETER_H
