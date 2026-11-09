#include "shell.h"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "core/envprovider.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/interpreter.h"
#include "core/useridentity.h"

namespace EmulatorMirea::Core
{
namespace
{
class ShellAwareProvider final : public IEnvProvider
{
public:
    ShellAwareProvider(const IEnvProvider& base, const Shell& shell) : m_base(base), m_shell(shell) {}

    [[nodiscard]] std::string getEnv(std::string_view name, const std::string& defaultValue = {}) const override
    {
        if (name == "PWD")
        {
            return m_shell.cwd().currentDir().toString();
        }

        if (name == "status")
        {
            return std::to_string(m_shell.lastStatus());
        }

        return m_base.getEnv(name, defaultValue);
    }

private:
    const IEnvProvider& m_base;
    const Shell& m_shell;
};
} // namespace

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
        m_history.add(line);
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

    const auto res = Interpreter::execute(m_registry, line, ctx);
    if (res.executionSucceeded())
    {
        m_lastStatus = *res.status;
    }

    return res;
}

} // namespace EmulatorMirea::Core