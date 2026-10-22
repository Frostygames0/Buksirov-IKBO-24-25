#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtTypes>

#include "config.h"
#include "core/executionresult.h"
#include "core/shell.h"


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

    Core::ExecutionResult runScript(const QString& path);

    TerminalWidget* m_terminalWidget = nullptr;
    void setupTerminal();

    void onTerminalCommandEntered(const QString& line);
    void onTerminalHistoryUpRequested();
    void onTerminalHistoryDownRequested();
};

} // namespace EmulatorMirea::Ui

#endif // MAINWINDOW_H