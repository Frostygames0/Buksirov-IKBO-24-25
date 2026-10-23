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

Shell::Shell(UserIdentity identity,
             std::unique_ptr<IEnvProvider> envProvider,
             CommandRegistry registry,
             std::unique_ptr<IVfs> vfs)
    : m_identity(std::move(identity)), m_envProvider(std::move(envProvider)), m_registry(std::move(registry)),
      m_vfs(std::move(vfs))
{
}

ExecutionResult Shell::submit(const QString& line, IOutput& out, Source source)
{
    if (source == Source::Interactive)
    {
        addHistory(line);
    }

    if (line.trimmed().isEmpty())
    {
        return ExecutionResult::withStatus(0);
    }

    PwdEnvProvider decoratedProvider{*m_envProvider, m_cwd};

    const ExecutionContext ctx{
        .identity = m_identity, .envProvider = decoratedProvider, .out = out, .vfs = *m_vfs, .cwd = m_cwd};

    return Interpreter::execute(m_registry, line, ctx);
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