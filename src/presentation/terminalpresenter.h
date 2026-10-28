#pragma once

#include <utility>

#include <QObject>
#include <QString>
#include <QStringList>

#include "core/output.h"
#include "core/shell.h"
#include "presentation/historynavigator.h"

namespace EmulatorMirea::Presentation
{

class TerminalPresenter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TerminalPresenter)
public:
    explicit TerminalPresenter(Core::Shell shell) : m_shell(std::move(shell)) {}

    struct StartupParams
    {
        QStringList banner;
        QStringList script;
    };

    void runStartup(const StartupParams& startParams);

    [[nodiscard]] QString title() const { return m_shell.title(); }

public slots:
    void onEnter(const QString& line);
    void onHistoryUp(const QString& currentInput);
    void onHistoryDown(const QString& currentInput);

signals:
    void cleared();
    void outputLine(const QString& text);
    void output(const QString& text);
    void promptShown(const QString& text);
    void inputSet(const QString& text);
    void exitRequested(int code);

private:
    Core::Shell m_shell;
    HistoryNavigator m_navigator{m_shell};

    class SignalWrappedOut final : public Core::IOutput
    {
    public:
        explicit SignalWrappedOut(TerminalPresenter& session) : m_session(session) {}

        void write(const QString& line) override { emit m_session.output(line); }

        void writeLine(const QString& line) override { emit m_session.outputLine(line); }

        void clear() override { emit m_session.cleared(); };

    private:
        TerminalPresenter& m_session;
    };

    SignalWrappedOut m_out{*this};
};

} // namespace EmulatorMirea::Presentation