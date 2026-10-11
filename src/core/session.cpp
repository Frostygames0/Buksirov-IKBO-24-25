#include "core/session.h"

#include <QSysInfo>
#include <QtEnvironmentVariables>
<<<<<<< HEAD
#include <QString>
#include <QStringList>
=======
>>>>>>> 23863db427f3cf9dd7473738be7dd43f66dd8e16

#include "utils/user.h"

namespace EmulatorMirea
{

Session::Session() : m_currentUserName(UserUtils::getCurrentUsername()), m_currentMachineName(QSysInfo::machineHostName()) {}

QString Session::userName() const
{
    return m_currentUserName;
};

QString Session::machineName() const
{
    return m_currentMachineName;
};

QString Session::fancyName() const
{
    return m_currentUserName + "@" + m_currentMachineName;
}

void Session::addHistory(const QString& line)
{
    if (line.trimmed().isEmpty())
        return;

    if (m_history.isEmpty() || m_history.last() != line)
        m_history.append(line);

    resetHistoryIndex();
}

void Session::resetHistoryIndex()
{
    m_historyIndex = m_history.size();
}

QString Session::previousHistoryItem()
{
    if (m_history.isEmpty())
        return {};

    if (m_historyIndex > 0)
        --m_historyIndex;

    return m_history[m_historyIndex];
}

QString Session::nextHistoryItem()
{
    if (m_history.isEmpty())
        return {};

    if (m_historyIndex < m_history.size() - 1)
    {
        ++m_historyIndex;
        return m_history[m_historyIndex];
    }


    // Reached past the latest item, return empty for current input line
    m_historyIndex = m_history.size();
    return {};

}

QString Session::getEnv(const QString& name, const QString& defaultValue) const
{
    if (name == "USER" || name == "USERNAME")
        return userName();
    if (name == "HOSTNAME")
        return machineName();
    if (name == "PWD")
    {
        // TODO: Implement VFS
        return "/";
    }

    return qEnvironmentVariable(name.toUtf8().constData(), defaultValue);
}

} // namespace EmulatorMirea