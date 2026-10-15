#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtTypes>

#include "core/shell.h"

namespace EmulatorMirea::Ui
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

private:
    Core::Shell m_shell;
    qsizetype m_historyIndex = 0;

    void setupUi();

    TerminalWidget* m_terminalBox = nullptr;
    void setupTerminal();
};

} // namespace EmulatorMirea::Ui

#endif // MAINWINDOW_H
