#pragma once

#include <string_view>
#include <string>
#include <vector>

#include "core/envprovider.h"

namespace EmulatorMirea::Core
{

class IEnvProvider;

namespace LineParser
{

[[nodiscard]] std::vector<std::string> parse(std::string_view in, const IEnvProvider& env);

} // namespace LineParser
} // namespace EmulatorMirea::Core