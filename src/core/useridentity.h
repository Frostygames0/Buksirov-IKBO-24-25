#pragma once

#include <QString>

namespace EmulatorMirea::Core
{

struct UserIdentity
{
    QString userName;
    QString machineName;

    [[nodiscard]] QString fancyName() const { return QStringLiteral("%1$%2").arg(userName, machineName); }
};

}; // namespace EmulatorMirea::Core