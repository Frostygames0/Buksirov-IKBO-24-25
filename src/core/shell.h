#pragma once

#include <memory>

#include <QString>
#include <QStringList>
#include <QtTypes>

#include "core/commands/registry.h"
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
                   CommandRegistry commandRegistry);
    enum class Source
    {
        Interactive,
        Script,
    };

    ExecutionResult submit(const QString& line, IOutput& out, Source source = Source::Interactive);

    [[nodiscard]] qsizetype historySize() const;
    [[nodiscard]] QString historyAt(qsizetype index) const;
    [[nodiscard]] QString prompt() const;
    [[nodiscard]] QString title() const;

private:
    void addHistory(const QString& line);

    UserIdentity m_identity;

    std::unique_ptr<IEnvProvider> m_envProvider;
    std::unique_ptr<IVfs> m_vfs;
    WorkingDirectory m_cwd{*m_vfs};

    CommandRegistry m_registry;

    QStringList m_history;

    int m_lastStatus;

    class ShellAwareProvider final : public IEnvProvider
    {
    public:
        ShellAwareProvider(const IEnvProvider& base, const Shell& shell) : m_base(base), m_shell(shell) {}

        [[nodiscard]] QString getEnv(const QString& name, const QString& defaultValue = {}) const override
        {
            if (name == u"PWD")
            {
                return m_shell.m_cwd.currentDir().toString();
            }

            if (name == u"status")
            {
                return QString::number(m_shell.m_lastStatus);
            }

            return m_base.getEnv(name, defaultValue);
        }

    private:
        const IEnvProvider& m_base;
        const Shell& m_shell;
    };
};

} // namespace EmulatorMirea::Core
