#pragma once

#include <format>
#include <memory>
#include <string>

#include "core/commands_v2/registry.h"
#include "core/envprovider.h"
#include "core/executionresult.h"
#include "core/history.h"
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

    [[nodiscard]] std::string prompt() const
    {
        return std::format("[{} {}]$ ", m_identity.fancyName(), m_cwd.currentDir().topmostName());
    }

    [[nodiscard]] std::string title() const { return m_identity.fancyName(); }

    [[nodiscard]] const WorkingDirectory& cwd() const { return m_cwd; }

    [[nodiscard]] const History& history() const { return m_history; }

    [[nodiscard]] int lastStatus() const { return m_lastStatus; }

private:
    UserIdentity m_identity;
    std::unique_ptr<IEnvProvider> m_envProvider;
    std::unique_ptr<IVfs> m_vfs;
    CommandRegistry m_registry;

    WorkingDirectory m_cwd{*m_vfs};
    History m_history;
    int m_lastStatus{};
};

} // namespace EmulatorMirea::Core
