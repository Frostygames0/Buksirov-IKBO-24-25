#pragma once

#include <concepts>
#include <functional>
#include <span>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

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

    [[nodiscard]] CommandBuilder name(std::string name) &&
    {
        m_name = std::move(name);
        return std::move(*this);
    }

    [[nodiscard]] CommandBuilder description(std::string description) &&
    {
        m_description = std::move(description);
        return std::move(*this);
    }

    template <class... Us>
    [[nodiscard]] CommandBuilder<Us...> args(ArgumentSet<Us...> args) &&
        requires(sizeof...(Args) == 0)
    {
        return CommandBuilder<Us...>(std::move(m_name), std::move(m_description), std::move(args));
    }

    template <class F>
        requires std::copy_constructible<std::decay_t<F>> &&
                 std::invocable<std::decay_t<F>&, const ExecutionContext&, Args&&...> &&
                 std::convertible_to<
                     std::invoke_result_t<std::decay_t<F>&, const ExecutionContext&, Args&&...>,
                     ExecutionResult>
    [[nodiscard]] Command build(F&& func) &&
    {
        Command::Handler handler = [argSet = std::move(m_args), f = std::forward<F>(func)](
                                       std::span<const std::string> unparsedArgs,
                                       const ExecutionContext& ctx) mutable -> ExecutionResult {
            // TODO: Move help into argument parsing
            for (const std::string& arg : unparsedArgs)
            {
                if (arg == "--")
                {
                    break;
                }

                if (arg == "--help" || arg == "-h")
                {
                    // TODO Improve Help
                    ctx.writeLine(argSet.usage());
                    return ExecutionResult::withStatus(ExitCode::Success);
                }
            }

            auto parsed = argSet.parse(unparsedArgs);
            if (!parsed)
            {
                ctx.writeLine(parsed.error());
                return ExecutionResult::withStatus(ExitCode::Misuse);
            }

            return std::apply(
                [&](Args&&... values) -> ExecutionResult {
                    return std::invoke(f, ctx, std::forward<Args>(values)...);
                },
                std::move(*parsed));
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