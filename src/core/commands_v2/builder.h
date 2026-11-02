#pragma once

#include <functional>
#include <span>
#include <string_view>

#include "core/commands_v2/arguments.h"
#include "core/commands_v2/command.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{
template <class... Args>
class CommandBuilder
{
public:
    CommandBuilder()
        requires(sizeof...(Args) == 0)
    = default;

    [[nodiscard]] CommandBuilder name(std::string n) &&
    {
        m_name = std::move(n);
        return std::move(*this);
    }

    [[nodiscard]] CommandBuilder description(std::string d) &&
    {
        m_description = std::move(d);
        return std::move(*this);
    }

    template <class... Us>
    [[nodiscard]] CommandBuilder<Us...> args(ArgumentSet<Us...> a) &&
    {
        return CommandBuilder<Us...>(std::move(m_name), std::move(m_description), std::move(a));
    }

    template <class F>
        requires std::copy_constructible<std::decay_t<F>> &&
                 std::invocable<std::decay_t<F>&, const ExecutionContext&, Args&&...> &&
                 std::convertible_to<
                     std::invoke_result_t<std::decay_t<F>&, const ExecutionContext&, Args&&...>,
                     ExecutionResult>
    [[nodiscard]] Command build(F&& func) &&
    {
        Command::Impl handler = [argSet = std::move(m_args), f = std::forward<F>(func)](
                                    std::span<const std::string_view> unparsedArgs,
                                    const ExecutionContext& ctx) mutable -> ExecutionResult {
            auto parsed = argSet.parse(unparsedArgs);
            if (!parsed)
            {
                ctx.writeLine(QString::fromStdString(parsed.error()));
                return ExecutionResult::withStatus(ExitCode::Misuse);
            }

            if constexpr (sizeof...(Args) == 0)
            {
                return std::invoke(f, ctx);
            }
            else
            {
                return std::apply(
                    [&](Args&&... values) -> ExecutionResult {
                        return std::invoke(f, ctx, std::forward<Args>(values)...);
                    },
                    std::move(*parsed));
            }
        };

        return Command{std::move(m_name), std::move(m_description), std::move(handler)};
    }

private:
    std::string m_name;
    std::string m_description;
    ArgumentSet<Args...> m_args;

    CommandBuilder(std::string n, std::string d, ArgumentSet<Args...> a)
        : m_name(std::move(n)),
          m_description(std::move(d)),
          m_args(std::move(a))
    {
    }

    template <class... Us>
    friend class CommandBuilder;
};
} // namespace EmulatorMirea::Core