#pragma once

#include <QString>

namespace EmulatorMirea::Core
{

class VfsPath;

class IEnvProvider
{
public:
    virtual ~IEnvProvider() = default;

    [[nodiscard]] virtual QString getEnv(const QString& name, const QString& defaultValue = {}) const = 0;
};

} // namespace EmulatorMirea::Core
