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
#include "core/vfs/path.h"
#include "core/vfs/vfs.h"

namespace EmulatorMirea::Core
{

class Shell
{
public:
    explicit Shell(UserIdentity identity,
                   std::unique_ptr<IEnvProvider> envProvider,
                   CommandRegistry commandRegistry,
                   std::unique_ptr<IVfs> vfs);
    enum class Source
    {
        Interactive,
        Script
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
    CommandRegistry m_registry;
    std::unique_ptr<IVfs> m_vfs;

    QStringList m_history;

    VfsPath m_cwd;

    class PwdEnvProvider final : public IEnvProvider
    {
    public:
        PwdEnvProvider(const IEnvProvider& base, const VfsPath& cwd) : m_base(base), m_cwd(cwd) {}

        [[nodiscard]] QString getEnv(const QString& name, const QString& defaultValue = {}) const override
        {
            if (name == u"PWD")
            {
                return m_cwd.toString();
            }

            return m_base.getEnv(name, defaultValue);
        }

    private:
        const IEnvProvider& m_base;
        const VfsPath& m_cwd;
    };
};

} // namespace EmulatorMirea::Core
