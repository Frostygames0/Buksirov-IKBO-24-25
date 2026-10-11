#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "core/envexpander.h"
#include "core/tokenizer.h"
#include "core/commandexecutor.h"

class QString;
class QStringView;

namespace EmulatorMirea
{

class Session;

class Interpreter
{
public:
    QString Execute(QStringView cmd, Session& session) const;
private:
    EnvExpander m_envExpander;
    Tokenizer m_tokenizer;
    CommandExecutor m_commandExecutor;
};

}

#endif // INTERPRETER_H
