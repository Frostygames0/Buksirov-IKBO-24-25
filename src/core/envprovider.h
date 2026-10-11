#ifndef IENVPROVIDER_H
#define IENVPROVIDER_H

#include <QString>

namespace EmulatorMirea::Core
{

class IEnvProvider
{
public:
    virtual ~IEnvProvider() = default;

    [[nodiscard]] virtual QString getEnv(const QString& name, const QString& defaultValue) const = 0;
};

} // namespace EmulatorMirea::Core

#endif // IENVPROVIDER_H
