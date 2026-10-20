#pragma once

namespace EmulatorMirea::Core
{
struct UserIdentity
{
    QString userName;
    QString machineName;

    [[nodiscard]] QString fancyName() const { return userName + u'@' + machineName; }
};
}; // namespace EmulatorMirea::Core