#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "core/commands_v2/registry.h"
#include "core/envprovider.h"
#include "core/executionresult.h"
#include "core/output.h"
#include "core/useridentity.h"
#include "core/vfs/vfs.h"
#include "core/workingdirectory.h"

namespace EmulatorMirea::Core
{

class Shell
{
public:
    explicit Shell(UserIdentity identity,
                   std::unique_ptr<IEnvProvider> envProvider,
                   std::unique_ptr<IVfs> vfs,
                   CommandRegistry registry);
    enum class Source
    {
        Interactive,
        Script,
    };

    ExecutionResult submit(const std::string& line, IOutput& out, Source source = Source::Interactive);

    [[nodiscard]] std::size_t historySize() const { return m_history.size(); }

    [[nodiscard]] std::string_view historyAt(std::size_t index) const { return m_history.at(index); }

    [[nodiscard]] std::string prompt() const
    {
        return std::format("[{} {}]$ ", m_identity.fancyName(), m_cwd.currentDir().topmostName());
    }

    [[nodiscard]] std::string title() const { return m_identity.fancyName(); }

private:
    void addHistory(std::string_view line);

    UserIdentity m_identity;

    std::unique_ptr<IEnvProvider> m_envProvider;
    std::unique_ptr<IVfs> m_vfs;
    WorkingDirectory m_cwd{*m_vfs};

    CommandRegistry m_registry;

    std::vector<std::string> m_history;

    int m_lastStatus{};

    class ShellAwareProvider final : public IEnvProvider
    {
    public:
        ShellAwareProvider(const IEnvProvider& base, const Shell& shell) : m_base(base), m_shell(shell) {}

        [[nodiscard]] std::string getEnv(std::string_view name,
                                         const std::string& defaultValue = {}) const override
        {
            if (name == "PWD")
            {
                return m_shell.m_cwd.currentDir().toString();
            }

            if (name == "status")
            {
                return std::to_string(m_shell.m_lastStatus);
            }

            return m_base.getEnv(name, defaultValue);
        }

    private:
        const IEnvProvider& m_base;
        const Shell& m_shell;
    };
};

} // namespace EmulatorMirea::Core
