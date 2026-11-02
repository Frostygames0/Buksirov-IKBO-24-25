#pragma once

#include <concepts>
#include <format>
#include <memory>
#include <unordered_map>
#include <utility>

#include "core/commands_v2/command.h"

namespace EmulatorMirea::Core
{

class CommandRegistry
{
public:
    struct Info
    {
        std::string name;
        std::string desciption;

        [[nodiscard]] std::string toString() const { return std::format("{} - {}", name, desciption); }
    };

    explicit CommandRegistry() = default;

    void registerCommand(Command&& command);

    template <std::derived_from<Command> T, class... Args>
        requires std::constructible_from<T, Args...>
    void registerCommand(Args&&... args)
    {
        registerCommand(std::make_unique<T>(std::forward<Args>(args)...));
    };

    [[nodiscard]] const Command* getCommand(const std::string& name) const;

    [[nodiscard]] std::vector<Info> allCommandsInfo() const;

private:
    std::unordered_map<std::string, Command> m_commands;
};

} // namespace EmulatorMirea::Core