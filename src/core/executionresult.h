#pragma once

#include <optional>

namespace EmulatorMirea::Core
{

namespace ExitCode
{
inline constexpr int None = -1;
inline constexpr int Success = 0;
inline constexpr int GeneralError = 1;
inline constexpr int Misuse = 2;
inline constexpr int UnableToRun = 126;
inline constexpr int CommandNotFound = 127;
} // namespace ExitCode

struct ExecutionResult
{
    int status = ExitCode::Success;
    std::optional<int> exitCode;

    [[nodiscard]] bool exitRequested() const { return exitCode.has_value(); }

    static ExecutionResult withStatus(int s) { return {.status = s, .exitCode = std::nullopt}; }

    static ExecutionResult exit(int code)
    {
        const int c = code & 0xFF;
        return {.status = c, .exitCode = c};
    }
};

} // namespace EmulatorMirea::Core
