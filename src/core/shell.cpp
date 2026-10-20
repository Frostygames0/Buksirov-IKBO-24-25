#include "shell.h"

#include <utility>

#include <QCoreApplication>
#include <QString>
#include <QStringLiteral>
#include <QSysInfo>

#include "core/envprovider.h"
#include "core/executioncontext.h"
#include "core/executionresult.h"
#include "core/interpreter.h"

namespace EmulatorMirea::Core
{

Shell::Shell(UserIdentity identity, IEnvProvider& envProvider, Interpreter& interpreter, IVfs& vfs)
    : m_identity(std::move(identity)), m_envProvider(envProvider), m_interpreter(interpreter), m_vfs(vfs)
{
}

ExecutionResult Shell::submit(const QString& line, IOutputStream& out)
{
    addHistory(line);

    if (line.trimmed().isEmpty())
    {
        return ExecutionResult::withStatus(0);
    }

    const ExecutionContext ctx{
        .envProvider = m_envProvider,
        .out = out,
        .identity = m_identity,
    };

    return m_interpreter.execute(line, ctx);
}

void Shell::addHistory(const QString& line)
{
    if (line.trimmed().isEmpty())
    {
        return;
    }

    if (m_history.isEmpty() || m_history.last() != line)
    {
        m_history.append(line);
    }
}

qsizetype Shell::historySize() const
{
    return m_history.size();
}

QString Shell::historyAt(qsizetype index) const
{
    return m_history.value(index);
}

QString Shell::prompt() const
{
    return QStringLiteral("[%1]$ ").arg(m_identity.fancyName());
}

QString Shell::title() const
{
    return m_identity.fancyName();
}

} // namespace EmulatorMirea::Core