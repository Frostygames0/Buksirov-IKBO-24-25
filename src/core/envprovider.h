#pragma once

#include <string>
#include <string_view>

namespace EmulatorMirea::Core
{

class VfsPath;

class IEnvProvider
{
public:
    virtual ~IEnvProvider() = default;

    [[nodiscard]] virtual std::string getEnv(std::string_view name, const std::string& defaultValue = {}) const = 0;
};

} // namespace EmulatorMirea::Core
