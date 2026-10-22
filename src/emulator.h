#pragma once

#include <QObject>
#include <QString>
#include <QStringLiteral>

#include "core/commands/commandhistory.h"
#include "core/commands/executor.h"
#include "core/commands/registry.h"
#include "core/executionresult.h"
#include "core/interpreter.h"
#include "core/iterminalview.h"
#include "core/shell.h"
#include "core/systemenvprovider.h"
#include "core/useridentity.h"
#include "core/vfs/path.h"
#include "core/vfs/vfs.h"

namespace EmulatorMirea
{

struct Config
{
    QString vfsPath;
    QString scriptPath;
};

class Emulator : public QObject
{
    Q_OBJECT
public:
    explicit Emulator(Core::UserIdentity identity);

    void setView(Core::ITerminalView* view) noexcept;
    [[nodiscard]] QString title() const;

    void startup(const Config& config);
public slots:
    void onCommandEntered(const QString& line);
    void onHistoryUp();
    void onHistoryDown();

signals:
    void exitRequested(int exitCode);

private:
    void registerCommands();
    void showPrompt();
    [[nodiscard]] QString prompt() const;
    Core::ExecutionResult submit(const QString& line);
    Core::ExecutionResult runScript(const QString& path);

    Core::UserIdentity m_identity;

    Core::SystemEnvProvider m_env{m_identity};

    Core::MemoryVfs m_vfs;
    Core::CommandRegistry m_registry;
    Core::Interpreter m_interpreter{m_registry};

    Core::CommandHistory m_history;
    Core::VfsPath m_cwd;

    Core::ITerminalView* m_view = nullptr;
};
} // namespace EmulatorMirea