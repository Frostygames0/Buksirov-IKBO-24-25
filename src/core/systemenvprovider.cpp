#include "systemenvprovider.h"

#include <QDir>
#include <QProcessEnvironment>
#include <QString>
#include <QStringLiteral>

namespace EmulatorMirea::Core
{

SystemEnvProvider::SystemEnvProvider(const QString& userName, const QString& machineName)
    : m_env(QProcessEnvironment::systemEnvironment())
{
    m_env.insert(QStringLiteral("USER"), userName);
    m_env.insert(QStringLiteral("USERNAME"), userName);
    m_env.insert(QStringLiteral("HOSTNAME"), machineName);
    m_env.insert(QStringLiteral("HOME"), QDir::homePath());
}

QString SystemEnvProvider::getEnv(const QString& name, const QString& defaultValue) const
{
    if (name == QStringLiteral("PWD"))
    {
        // TODO: Implement VFS
        return QStringLiteral("/");
    }

    return m_env.value(name, defaultValue);
}

} // namespace EmulatorMirea::Core
