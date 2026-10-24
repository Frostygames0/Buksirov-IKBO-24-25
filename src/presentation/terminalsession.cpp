#include "terminalsession.h"

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "core/executionresult.h"
#include "core/shell.h"
#include "presentation/historynavigator.h"

namespace EmulatorMirea::Presentation
{

TerminalSession::TerminalSession(Core::Shell shell) : m_shell(std::move(shell)) {}

void TerminalSession::start(const QString& scriptPath)
{
    if (!scriptPath.isEmpty())
    {
        Core::ExecutionResult r = runScript(scriptPath);
        if (r.exitRequested())
        {
            emit exitRequested(*r.exitCode);
            return;
        }
    }

    m_navigator.reset();
    showPrompt();
}

void TerminalSession::onEnter(const QString& line)
{
    Core::ExecutionResult r = m_shell.submit(line, m_out);

    m_navigator.reset();
    if (r.exitRequested())
    {
        emit exitRequested(*r.exitCode);
        return;
    }

    showPrompt();
}

void TerminalSession::onHistoryUp(const QString& currentInput)
{
    emit inputChanged(m_navigator.up(currentInput));
}

void TerminalSession::onHistoryDown()
{
    emit inputChanged(m_navigator.down());
}

QString TerminalSession::title() const
{
    return m_shell.title();
}

void TerminalSession::showPrompt()
{
    emit promptChanged(m_shell.prompt());
    emit promptShown();
}

Core::ExecutionResult TerminalSession::runScript(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit outputLine(QStringLiteral("error: cannot open script: %1 (%2)").arg(path, file.errorString()));
        return Core::ExecutionResult::withStatus(Core::ExitCode::UnableToRun);
    }

    QTextStream in(&file);
    Core::ExecutionResult result;

    while (!in.atEnd())
    {
        const QString line = in.readLine();
        const QString trimmed = line.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith(u'#'))
        {
            continue;
        }

        showPrompt();
        emit outputLine(line);

        result = m_shell.submit(line, m_out, Core::Shell::Source::Script);
        if (result.exitRequested())
        {
            break;
        }
    }

    return result;
}

} // namespace EmulatorMirea::Presentation