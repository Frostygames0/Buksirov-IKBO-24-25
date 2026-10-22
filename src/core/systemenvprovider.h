#ifndef SYSTEMENVPROVIDER_H
#define SYSTEMENVPROVIDER_H

#include <QProcessEnvironment>
#include <QString>

#include "core/envprovider.h"
#include "core/useridentity.h"
#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{

class SystemEnvProvider : public IEnvProvider
{
public:
    explicit SystemEnvProvider(const UserIdentity& identity);

    [[nodiscard]] QString getEnv(const QString& name, const QString& defaultValue = {}) const override;

    void provideCwd(const VfsPath& path) override;

private:
    QProcessEnvironment m_env;
    VfsPath m_cwd;
};

} // namespace EmulatorMirea::Core

#endif // SYSTEMENVPROVIDER_H
