#pragma once

#include <functional>
#include <span>
#include <string>
#include <string_view>

#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{
class Command
{
public:
    using Impl = std::function<ExecutionResult(std::span<const std::string_view>, const ExecutionContext&)>;

    [[nodiscard]] ExecutionResult execute(std::span<const std::string_view> args,
                                          const ExecutionContext& ctx) const
    {
        return m_impl(args, ctx);
    }

    [[nodiscard]] const std::string& name() const { return m_name; }

    [[nodiscard]] const std::string& description() const { return m_description; }

private:
    std::string m_name;
    std::string m_description;
    Impl m_impl;

    Command(std::string name, std::string description, Impl impl)
        : m_name(std::move(name)),
          m_description(std::move(description)),
          m_impl(std::move(impl))
    {
    }

    template <class... Args>
    friend class CommandBuilder;
};
} // namespace EmulatorMirea::Core