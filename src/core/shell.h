#ifndef SHELL_H
#define SHELL_H

#include <memory>

#include <QString>
#include <QStringList>
#include <QtTypes>

#include "core/commands/registry.h"
#include "core/envprovider.h"
#include "core/executionresult.h"
#include "core/iterminalview.h"
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
                   std::unique_ptr<CommandRegistry> commandRegistry,
                   std::unique_ptr<IVfs> vfs);

    Shell(const Shell&) = delete;
    Shell& operator=(const Shell&) = delete;

    enum class HistoryMode
    {
        Save,
        Ignore
    };

    ExecutionResult submit(const QString& line, ITerminalView& view, HistoryMode historyMode = HistoryMode::Save);

    [[nodiscard]] qsizetype historySize() const;
    [[nodiscard]] QString historyAt(qsizetype index) const;
    [[nodiscard]] QString prompt() const;
    [[nodiscard]] QString title() const;

private:
    void addHistory(const QString& line);

    UserIdentity m_identity;

    std::unique_ptr<IEnvProvider> m_envProvider;
    std::unique_ptr<CommandRegistry> m_registry;
    std::unique_ptr<IVfs> m_vfs;

    QStringList m_history;

    VfsPath cwd;
};

} // namespace EmulatorMirea::Core

#endif // SHELL_H
