#include "systemenvprovider.h"

#include <qobject.h>
#include <string>
#include <string_view>

#include <QProcessEnvironment>

#include "core/useridentity.h"

namespace EmulatorMirea::Core
{

SystemEnvProvider::SystemEnvProvider(const UserIdentity& identity)
    : m_env(QProcessEnvironment::systemEnvironment())
{
    m_env.insert(QStringLiteral("USER"), QString::fromStdString(identity.userName));
    m_env.insert(QStringLiteral("USERNAME"), QString::fromStdString(identity.userName));
    m_env.insert(QStringLiteral("HOSTNAME"), QString::fromStdString(identity.machineName));
}

std::string SystemEnvProvider::getEnv(std::string_view name, const std::string& defaultValue) const
{
    return m_env.value(QString::fromUtf8(name), QString::fromStdString(defaultValue)).toStdString();
}

} // namespace EmulatorMirea::Core
