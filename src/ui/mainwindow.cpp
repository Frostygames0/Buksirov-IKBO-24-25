#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFrame>
#include <QIcon>
#include <QString>
#include <QTimer>

#include "config.h"
#include "core/executionresult.h"
#include "core/shell.h"
#include "ui/terminalwidget.h"

namespace EmulatorMirea::Ui
{

MainWindow::MainWindow(Core::Shell& shell) : QMainWindow(nullptr), m_shell(shell)
{
    setupUi();
}

void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle(tr("%1 — %2").arg(m_shell.title(), QApplication::applicationName()));

    setWindowIcon(QIcon(QStringLiteral(":/icon.png")));

    setMinimumSize(400, 300);
    resize(800, 600);

    setupTerminal();
    setCentralWidget(m_terminalWidget);
}

void MainWindow::startup(const Config& config)
{
    qDebug() << "vfs =" << (config.vfsPath.isEmpty() ? QStringLiteral("(not set)") : config.vfsPath);
    qDebug() << "script =" << (config.scriptPath.isEmpty() ? QStringLiteral("(not set)") : config.scriptPath);

    if (!config.scriptPath.isEmpty())
    {
        const Core::ExecutionResult result = runScript(config.scriptPath);
        if (result.exitRequested())
        {
            QCoreApplication::exit(*result.exitCode); // NOLINT
        }
    }

    m_historyIndex = m_shell.historySize();
    m_terminalWidget->setPrompt(m_shell.prompt());
    m_terminalWidget->showPrompt();
}

Core::ExecutionResult MainWindow::runScript(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_terminalWidget->write(QStringLiteral("[error] cannot open script: %1 (%2)").arg(path, file.errorString()));
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

        m_terminalWidget->setPrompt(m_shell.prompt());
        m_terminalWidget->showPrompt();
        m_terminalWidget->write(line);

        result = m_shell.submit(line, *m_terminalWidget, Core::Shell::HistoryMode::Ignore);
        if (result.exitRequested())
        {
            break;
        }
    }

    return result;
}

void MainWindow::setupTerminal()
{
    m_terminalWidget = new TerminalWidget(this);
    m_terminalWidget->setObjectName("terminal");

    QFont font(QStringLiteral("JetBrains Mono"));
    font.setPointSize(12);
    font.setStyleHint(QFont::Monospace);
    m_terminalWidget->setFont(font);

    m_terminalWidget->setStyleSheet(QStringLiteral("QPlainTextEdit {"
                                                   "    border: none;"
                                                   "    border-radius: 0px;"
                                                   "    background-color: #121212;"
                                                   "    color: #dcdcdc;"
                                                   "}"));

    m_terminalWidget->setFrameShape(QFrame::NoFrame);
    m_terminalWidget->setBackgroundVisible(false);

    connect(m_terminalWidget, &TerminalWidget::commandEntered, this, &MainWindow::onTerminalCommandEntered);
    connect(m_terminalWidget, &TerminalWidget::historyUpRequested, this, &MainWindow::onTerminalHistoryUpRequested);
    connect(m_terminalWidget, &TerminalWidget::historyDownRequested, this, &MainWindow::onTerminalHistoryDownRequested);
}

void MainWindow::onTerminalCommandEntered(const QString& line)
{
    Core::ExecutionResult result = m_shell.submit(line, *m_terminalWidget);
    if (result.exitRequested())
    {
        QCoreApplication::exit(*result.exitCode); // NOLINT
    }

    m_historyIndex = m_shell.historySize();

    m_terminalWidget->setPrompt(m_shell.prompt());
    m_terminalWidget->showPrompt();
}

void MainWindow::onTerminalHistoryUpRequested()
{
    if (m_shell.historySize() == 0)
    {
        return;
    }

    if (m_historyIndex > 0)
    {
        --m_historyIndex;
    }

    m_terminalWidget->setInput(m_shell.historyAt(m_historyIndex));
}

void MainWindow::onTerminalHistoryDownRequested()
{
    const qsizetype size = m_shell.historySize();
    if (size == 0)
    {
        return;
    }

    if (m_historyIndex < size - 1)
    {
        ++m_historyIndex;
        m_terminalWidget->setInput(m_shell.historyAt(m_historyIndex));
    }
    else
    {
        m_historyIndex = size;
        m_terminalWidget->setInput({});
    }
}

} // namespace EmulatorMirea::Ui