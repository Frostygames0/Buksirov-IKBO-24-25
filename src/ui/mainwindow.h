#pragma once

#include <QMainWindow>
#include <QtTypes>

#include "config.h"
#include "core/executionresult.h"

namespace EmulatorMirea::Core
{
class Shell;
}

namespace EmulatorMirea::Ui
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Core::Shell& shell);

    void startup(const Config& config);

private:
    Core::Shell& m_shell;
    qsizetype m_historyIndex = 0;

    void setupUi();

    // TODO: Remove it from here, it's not the best place
    Core::ExecutionResult runScript(const QString& path);

    TerminalWidget* m_terminalWidget = nullptr;
    void setupTerminal();

    void onTerminalCommandEntered(const QString& line);
    void onTerminalHistoryUpRequested();
    void onTerminalHistoryDownRequested();
};

} // namespace EmulatorMirea::Ui