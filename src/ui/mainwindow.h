#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/shell.h"

namespace EmulatorMirea
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

private:
    Shell m_shell;

    void setupUi();

    TerminalWidget* m_terminalBox = nullptr;
    void setupTerminal();
};
#endif // MAINWINDOW_H
}
