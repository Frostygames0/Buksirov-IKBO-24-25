#pragma once

#include <string>
#include <string_view>

#include <QProcessEnvironment>

#include "core/envprovider.h"
#include "core/useridentity.h"
#include "core/vfs/path.h"

namespace EmulatorMirea::Core
{

class SystemEnvProvider : public IEnvProvider
{
public:
    explicit SystemEnvProvider(const UserIdentity& identity);

    [[nodiscard]] std::string getEnv(std::string_view name,
                                     const std::string& defaultValue = {}) const override;

private:
    QProcessEnvironment m_env;
    VfsPath m_cwd;
};

} // namespace EmulatorMirea::Core
