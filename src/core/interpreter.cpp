#include "core/interpreter.h"

#include <QString>
#include <QStringList>
#include <QStringView>

#include "core/session.h"

namespace EmulatorMirea
{

QString Interpreter::execute(QStringView cmd, Session& session) const
{
    const QStringList tokens = m_tokenizer.tokenize(cmd, session);
    return m_commandExecutor.execute(tokens, session);
}

} // namespace EmulatorMirea