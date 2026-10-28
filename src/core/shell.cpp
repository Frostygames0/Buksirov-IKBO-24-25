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
             std::unique_ptr<IVfs> vfs,
             CommandRegistry registry)
    : m_identity(std::move(identity)),
      m_envProvider(std::move(envProvider)),
      m_vfs(std::move(vfs)),
      m_registry(std::move(registry))
{
}

ExecutionResult Shell::submit(const QString& line, IOutput& out, Source source)
{
    if (source == Source::Interactive)
    {
        addHistory(line);
    }

    ShellAwareProvider decoratedProvider{*m_envProvider, *this};

    const ExecutionContext ctx{
        .identity = m_identity,
        .envProvider = decoratedProvider,
        .out = out,
        .vfs = *m_vfs,
        .cwd = m_cwd,
        .commandRegistry = m_registry,
    };

    auto res = Interpreter::execute(m_registry, line, ctx);
    if (res.status != ExitCode::None)
    {
        m_lastStatus = res.status;
    }

    return res;
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

} // namespace EmulatorMirea::Core