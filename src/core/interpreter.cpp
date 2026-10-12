#include "interpreter.h"

#include <QString>
#include <QStringList>
#include <QStringView>

#include "core/session.h"

namespace EmulatorMirea::Core
{

QString Interpreter::execute(QStringView cmd, ISession& session) const
{
    const QStringList args = m_lineParser.parse(cmd, session);
    return m_commandExecutor.execute(args, session);
}

} // namespace EmulatorMirea::Core