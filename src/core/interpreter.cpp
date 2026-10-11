#include "core/interpreter.h"

#include <QString>
#include <QStringList>
#include <QStringView>

#include "core/session.h"

namespace EmulatorMirea
{

QString Interpreter::execute(QStringView cmd, Session& session) const
{
    QStringList tokens = m_tokenizer.Tokenize(cmd);

    for (int i = 0; i < tokens.size(); ++i)
        tokens[i] = m_envExpander.Expand(tokens[i], session);

    tokens.removeAll({});

    return m_commandExecutor.execute(tokens, session);
}

}