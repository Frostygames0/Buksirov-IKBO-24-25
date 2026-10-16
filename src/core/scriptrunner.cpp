#include "scriptrunner.h"

#include <QFile>
#include <QTextStream>

#include "core/iterminalview.h"
#include "core/shell.h"
#include "core/terminalostream.h"

namespace EmulatorMirea::Core
{

ScriptRunner::ScriptRunner(Shell& shell, ITerminalView& view) : m_shell(&shell), m_view(&view) {}

ExecutionResult ScriptRunner::run(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_view->print(QStringLiteral("[error] cannot open script: %1 (%2)").arg(path, file.errorString()));
        return ExecutionResult::withStatus(ExitCode::UnableToRun);
    }

    TerminalOStream out(*m_view);
    QTextStream in(&file);

    ExecutionResult result;

    while (!in.atEnd())
    {
        const QString line = in.readLine();
        const QString trimmed = line.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith('#'))
            continue;

        m_view->setPrompt(m_shell->prompt());
        m_view->showPrompt();
        m_view->print(line);

        result = m_shell->submit(line, out);
        if (result.exitRequested())
            break;
    }

    return result;
}

} // namespace EmulatorMirea::Core