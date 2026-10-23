#pragma once

#include <qchar.h>

#include <QMainWindow>
#include <QtTypes>

#include "core/terminalsession.h"

namespace EmulatorMirea::Ui
{

class TerminalWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Core::TerminalSession& session);
    void startDelayed(const QString& scriptPath);

private:
    Core::TerminalSession& m_session;

    void setupUi();
    TerminalWidget* m_terminalWidget = nullptr;
    void setupTerminal();
};

} // namespace EmulatorMirea::Ui