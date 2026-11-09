#pragma once

#include <format>
#include <string>

namespace EmulatorMirea::Core
{

struct UserIdentity
{
    std::string userName;
    std::string machineName;

    [[nodiscard]] std::string fancyName() const { return std::format("{}${}", userName, machineName); }

    bool operator==(const UserIdentity&) const = default;
};

}; // namespace EmulatorMirea::Core