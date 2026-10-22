#include "emulator.h"

#include <memory>
#include <utility>

#include <QFile>
#include <QTextStream>

#include "core/commands/command.h"
#include "core/executioncontext.h"
#include "core/vfs/xmlvfsreader.h"

namespace EmulatorMirea
{

Emulator::Emulator(Core::UserIdentity identity) : m_identity(std::move(identity))
{
    registerCommands();
}

void Emulator::registerCommands()
{
    m_registry.registerCommand(std::make_unique<Core::ExitCommand>());
    m_registry.registerCommand(std::make_unique<Core::StubCommand>(QStringLiteral("ls")));
    m_registry.registerCommand(std::make_unique<Core::StubCommand>(QStringLiteral("cd")));
}

void Emulator::setView(Core::ITerminalView* view) noexcept
{
    m_view = view;
}

QString Emulator::title() const
{
    return m_identity.fancyName();
}

QString Emulator::prompt() const
{
    return QStringLiteral("[%1 %2]$ ").arg(m_identity.fancyName(), m_cwd.toString());
}

void Emulator::showPrompt()
{
    if (m_view == nullptr)
    {
        return;
    }

    m_view->setPrompt(prompt());
    m_view->showPrompt();
}

void Emulator::startup(const Config& config)
{
    if (m_view == nullptr)
    {
        return;
    }

    if (!config.vfsPath.isEmpty())
    {
        auto res = Core::XmlVfs::readFromFile(config.vfsPath, m_vfs);
        if (!res)
        {
            m_view->write(QStringLiteral("Unable to read VFS. Reason: %1").arg(res.error()));
        }
    }

    if (!config.scriptPath.isEmpty())
    {
        const Core::ExecutionResult result = runScript(config.scriptPath);
        if (result.exitRequested())
        {
            emit exitRequested(*result.exitCode);
            return;
        }
    }

    showPrompt();
}

Core::ExecutionResult Emulator::submit(const QString& line)
{
    m_history.record(line);

    if (line.trimmed().isEmpty())
    {
        return Core::ExecutionResult::withStatus(Core::ExitCode::Success);
    }

    const Core::ExecutionContext ctx{
        .envProvider = m_env,
        .out = *m_view,
        .identity = m_identity,
    };

    return m_interpreter.execute(line, ctx);
}

Core::ExecutionResult Emulator::runScript(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_view->write(QStringLiteral("[error] cannot open script: %1 (%2)").arg(path, file.errorString()));
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
        m_view->write(line);

        result = submit(line);
        if (result.exitRequested())
        {
            break;
        }
    }

    return result;
}

void Emulator::onCommandEntered(const QString& line)
{
    if (m_view == nullptr)
    {
        return;
    }

    const Core::ExecutionResult result = submit(line);
    if (result.exitRequested())
    {
        emit exitRequested(*result.exitCode);
        return;
    }

    showPrompt();
}

void Emulator::onHistoryUp()
{
    if (m_view == nullptr)
    {
        return;
    }

    if (auto line = m_history.moveUp(); line)
    {
        m_view->setInput(*line);
    }
}

void Emulator::onHistoryDown()
{
    if (m_view == nullptr)
    {
        return;
    }

    if (auto line = m_history.moveDown(); line)
    {
        m_view->setInput(*line);
    }
}

} // namespace EmulatorMirea