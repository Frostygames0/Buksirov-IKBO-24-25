#include "systemenvprovider.h"

#include <QDir>
#include <QProcessEnvironment>
#include <QString>
#include <QStringLiteral>

#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

SystemEnvProvider::SystemEnvProvider(const UserIdentity& identity) : m_env(QProcessEnvironment::systemEnvironment())
{
    m_env.insert(QStringLiteral("USER"), identity.userName);
    m_env.insert(QStringLiteral("USERNAME"), identity.userName);
    m_env.insert(QStringLiteral("HOSTNAME"), identity.machineName);
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
