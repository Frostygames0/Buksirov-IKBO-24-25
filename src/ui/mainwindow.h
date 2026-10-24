#pragma once

#include <QMainWindow>

namespace EmulatorMirea::Ui
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

    TerminalWidget& terminalWidget();

private:
    void setupUi();
    TerminalWidget* m_terminalWidget = nullptr;
    void setupTerminal();
};

} // namespace EmulatorMirea::Ui