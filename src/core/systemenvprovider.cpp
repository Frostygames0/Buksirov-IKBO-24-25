#include "systemenvprovider.h"

#include <QProcessEnvironment>
#include <QString>

#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

SystemEnvProvider::SystemEnvProvider(const UserIdentity& identity) : m_env(QProcessEnvironment::systemEnvironment())
{
    m_env.insert(QStringLiteral("USER"), identity.userName);
    m_env.insert(QStringLiteral("USERNAME"), identity.userName);
    m_env.insert(QStringLiteral("HOSTNAME"), identity.machineName);
}

QString SystemEnvProvider::getEnv(const QString& name, const QString& defaultValue) const
{
    return m_env.value(name, defaultValue);
}

} // namespace EmulatorMirea::Core
