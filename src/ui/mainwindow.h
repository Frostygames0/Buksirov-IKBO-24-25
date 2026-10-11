#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/session.h"
#include "core/interpreter.h"

class QAction;
class QMenu;

namespace EmulatorMirea
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

private:
    Session m_session;
    Interpreter m_interpreter;

    void setupUi();

    TerminalWidget* m_terminalBox = nullptr;
    QMenu* m_menuHelp = nullptr;
    QAction *m_actionAbout = nullptr;

    void setupTerminal();
};
#endif // MAINWINDOW_H
}
