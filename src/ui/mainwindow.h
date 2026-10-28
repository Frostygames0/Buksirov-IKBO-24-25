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
    TerminalWidget* m_terminalWidget = nullptr;

    void setupUi();
    void setupTerminal();
};

} // namespace EmulatorMirea::Ui