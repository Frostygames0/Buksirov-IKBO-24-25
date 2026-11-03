#include "shell.h"

#include <utility>
#include <string_view>
#include <memory>

#include "core/envprovider.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/interpreter.h"

namespace EmulatorMirea::Core
{

Shell::Shell(UserIdentity identity,
             std::unique_ptr<IEnvProvider> envProvider,
             std::unique_ptr<IVfs> vfs,
             CommandRegistry registry)
    : m_identity(std::move(identity)),
      m_envProvider(std::move(envProvider)),
      m_vfs(std::move(vfs)),
      m_registry(std::move(registry))
{
}

ExecutionResult Shell::submit(const std::string& line, IOutput& out, Source source)
{
    if (source == Source::Interactive)
    {
        addHistory(line);
    }

    ShellAwareProvider decoratedProvider{*m_envProvider, *this};

    const ExecutionContext ctx{
        .identity = m_identity,
        .envProvider = decoratedProvider,
        .out = out,
        .vfs = *m_vfs,
        .cwd = m_cwd,
        .commandRegistry = m_registry,
    };

    auto res = Interpreter::execute(m_registry, line, ctx);
    if (res.status != ExitCode::None)
    {
        m_lastStatus = res.status;
    }

    return res;
}

namespace
{
std::string_view trim(std::string_view sv)
{
    const char* spaces = " \t\n\r\f\v";

    size_t start = sv.find_first_not_of(spaces);
    if (start == std::string_view::npos)
    {
        return "";
    }

    size_t end = sv.find_last_not_of(spaces);

    return sv.substr(start, end - start + 1);
}
} // namespace

void Shell::addHistory(std::string_view line)
{
    if (trim(line).empty())
    {
        return;
    }

    if (m_history.empty() || m_history.back() != line)
    {
        m_history.emplace_back(line);
    }
}

} // namespace EmulatorMirea::Core