#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtTypes>

#include "core/launchconfig.h"
#include "core/shell.h"

namespace EmulatorMirea::Ui
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Core::LaunchConfig config);

private:
    Core::LaunchConfig m_config;
    Core::Shell m_shell;
    qsizetype m_historyIndex = 0;

    void setupUi();
    void startup();
    void stepScript();

    TerminalWidget* m_terminalBox = nullptr;
    void setupTerminal();

    void onTerminalCommandEntered(const QString& line);
    void onTerminalHistoryUpRequested();
    void onTerminalHistoryDownRequested();
};

} // namespace EmulatorMirea::Ui

#endif // MAINWINDOW_H
