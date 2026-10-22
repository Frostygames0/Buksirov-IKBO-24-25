#ifndef ENVPROVIDER_H
#define ENVPROVIDER_H

#include <QString>

#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{

class VfsPath;

class IEnvProvider
{
public:
    virtual ~IEnvProvider() = default;

    [[nodiscard]] virtual QString getEnv(const QString& name, const QString& defaultValue = {}) const = 0;

    virtual void provideCwd(const VfsPath& cwd) = 0;
};

} // namespace EmulatorMirea::Core

#endif // ENVPROVIDER_H
