#include "terminalpresenter.h"

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "core/executionresult.h"
#include "core/shell.h"
#include "presentation/historynavigator.h"

namespace EmulatorMirea::Presentation
{

void TerminalPresenter::runStartup(const StartupParams& startParams)
{
    for (const QString& line : startParams.banner)
    {
        emit outputLine(line);
    }

    for (const QString& line : startParams.script)
    {
        emit promptShown(m_shell.prompt());
        emit outputLine(line);

        Core::ExecutionResult result = m_shell.submit(line, m_out, Core::Shell::Source::Script);
        if (result.exitRequested())
        {
            emit exitRequested(*result.exitCode);
            return;
        }
    }

    m_navigator.reset();
    emit promptShown(m_shell.prompt());
}

void TerminalPresenter::onEnter(const QString& line)
{
    Core::ExecutionResult r = m_shell.submit(line, m_out);

    m_navigator.reset();
    if (r.exitRequested())
    {
        emit exitRequested(*r.exitCode);
        return;
    }

    emit promptShown(m_shell.prompt());
}

void TerminalPresenter::onHistoryUp(const QString& currentInput)
{
    emit inputSet(m_navigator.up(currentInput));
}

void TerminalPresenter::onHistoryDown(const QString& currentInput)
{
    emit inputSet(m_navigator.down(currentInput));
}

} // namespace EmulatorMirea::Presentation