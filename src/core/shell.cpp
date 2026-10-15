#include "shell.h"

#include <functional>
#include <utility>

#include <QString>
#include <QStringLiteral>
#include <QSysInfo>

#include "core/commands/command.h"
#include "utils/user.h"

namespace EmulatorMirea::Core
{

Shell::Shell(std::function<void(int)> exitHandler)
    : m_userName(UserUtils::getCurrentUsername()), m_machineName(QSysInfo::machineHostName()),
      m_env(m_userName, m_machineName), m_exitHandler(std::move(exitHandler))
{
}

int Shell::submit(const QString& line, IOutputStream& out)
{
    addHistory(line);

    if (line.trimmed().isEmpty())
        return 0;

    const CommandContext ctx{
        .envProvider = m_env,
        .out = out,
        .userName = m_userName,
        .machineName = m_machineName,
        .requestExit = m_exitHandler,
    };

    return m_interpreter.execute(line, ctx);
}

void Shell::addHistory(const QString& line)
{
    if (line.trimmed().isEmpty())
        return;

    if (m_history.isEmpty() || m_history.last() != line)
        m_history.append(line);
}

qsizetype Shell::historySize() const
{
    return m_history.size();
}

QString Shell::historyAt(qsizetype index) const
{
    return m_history.value(index);
}

QString Shell::fancyName() const
{
    return m_userName + QStringLiteral("@") + m_machineName;
}

QString Shell::prompt() const
{
    return QStringLiteral("[%1]$ ").arg(fancyName());
}

QString Shell::title() const
{
    return fancyName();
}

} // namespace EmulatorMirea::Core