#pragma once

#include <QObject>
#include <QString>

#include "core/historynavigator.h"
#include "core/output.h"
#include "core/shell.h"

namespace EmulatorMirea::Core
{

class TerminalSession : public QObject
{
    Q_OBJECT
public:
    explicit TerminalSession(Shell shell);

    void start(const QString& scriptPath);

    [[nodiscard]] QString title() const;

public slots:
    void onEnter(const QString& line);
    void onHistoryUp(const QString& currentInput);
    void onHistoryDown();

signals:
    void outputLine(const QString& text);
    void promptChanged(const QString& prompt);
    void promptShown();
    void inputChanged(const QString& text);
    void exitRequested(int code);

private:
    Shell m_shell;
    HistoryNavigator m_navigator{m_shell};

    void showPrompt();

    // TODO: Extract
    ExecutionResult runScript(const QString& path, IOutput& out);

    class SignalWrappedOut : public IOutput
    {
    public:
        SignalWrappedOut(TerminalSession& session) : m_session(session) {}

        void writeLine(const QString& line) override { emit m_session.outputLine(line); }

    private:
        TerminalSession& m_session;
    };

    SignalWrappedOut m_out{*this};
};

}; // namespace EmulatorMirea::Core