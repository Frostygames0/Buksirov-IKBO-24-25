#include "mainwindow.h"

#include <utility>

#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QIcon>
#include <QString>
#include <QTimer>

#include "core/executionresult.h"
#include "core/scriptrunner.h"
#include "core/terminalostream.h"
#include "ui/terminalwidget.h"


namespace EmulatorMirea::Ui
{

MainWindow::MainWindow(Core::LaunchConfig config) : QMainWindow(nullptr), m_config(std::move(config))
{
    setupUi();

    QTimer::singleShot(0, this, &MainWindow::startup);
}

void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle(tr("%1 — %2").arg(m_shell.title(), QApplication::applicationName()));

    setWindowIcon(QIcon(":/icon.png"));

    setMinimumSize(400, 300);
    resize(800, 600);

    setupTerminal();
    setCentralWidget(m_terminalBox);
}

void MainWindow::startup()
{
    qDebug() << "vfs =" << (m_config.vfsPath.isEmpty() ? QStringLiteral("(not set)") : m_config.vfsPath);
    qDebug() << "script =" << (m_config.scriptPath.isEmpty() ? QStringLiteral("(not set)") : m_config.scriptPath);

    Core::ScriptRunner runner(m_shell, *m_terminalBox);
    if (!m_config.scriptPath.isEmpty())
    {
        const Core::ExecutionResult result = runner.run(m_config.scriptPath);
        if (result.exitRequested())
        {
            QCoreApplication::exit(*result.exitCode); // NOLINT
        }
    }

    m_historyIndex = m_shell.historySize();
    m_terminalBox->setPrompt(m_shell.prompt());
    m_terminalBox->showPrompt();
}

void MainWindow::setupTerminal()
{
    m_terminalBox = new TerminalWidget(this);
    m_terminalBox->setObjectName("terminalBox");

    QFont font("JetBrains Mono");
    font.setPointSize(12);
    font.setStyleHint(QFont::Monospace);
    m_terminalBox->setFont(font);

    m_terminalBox->setStyleSheet("QPlainTextEdit {"
                                 "    border: none;"
                                 "    border-radius: 0px;"
                                 "    background-color: #121212;"
                                 "    color: #dcdcdc;"
                                 "}");

    m_terminalBox->setFrameShape(QFrame::NoFrame);
    m_terminalBox->setBackgroundVisible(false);

    connect(m_terminalBox, &TerminalWidget::commandEntered, this, &MainWindow::onTerminalCommandEntered);
    connect(m_terminalBox, &TerminalWidget::historyUpRequested, this, &MainWindow::onTerminalHistoryUpRequested);
    connect(m_terminalBox, &TerminalWidget::historyDownRequested, this, &MainWindow::onTerminalHistoryDownRequested);
}

void MainWindow::onTerminalCommandEntered(const QString& line)
{
    Core::TerminalOStream out(*m_terminalBox);

    Core::ExecutionResult result = m_shell.submit(line, out);
    if (result.exitRequested())
    {
        QCoreApplication::exit(*result.exitCode); // NOLINT
    }

    m_historyIndex = m_shell.historySize();

    m_terminalBox->setPrompt(m_shell.prompt());
    m_terminalBox->showPrompt();
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

    m_terminalBox->setInput(m_shell.historyAt(m_historyIndex));
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
        m_terminalBox->setInput(m_shell.historyAt(m_historyIndex));
    }
    else
    {
        m_historyIndex = size;
        m_terminalBox->setInput({});
    }
}

} // namespace EmulatorMirea::Ui
