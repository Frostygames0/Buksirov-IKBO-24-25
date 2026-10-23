#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFrame>
#include <QIcon>
#include <QString>
#include <QTimer>

#include "core/terminalsession.h"
#include "ui/terminalwidget.h"

namespace EmulatorMirea::Ui
{

MainWindow::MainWindow(Core::TerminalSession& session) : QMainWindow(nullptr), m_session(session)
{
    setupUi();

    connect(m_terminalWidget, &TerminalWidget::commandEntered, &session, &Core::TerminalSession::onEnter);
    connect(m_terminalWidget, &TerminalWidget::historyUpRequested, &session, &Core::TerminalSession::onHistoryUp);
    connect(m_terminalWidget, &TerminalWidget::historyDownRequested, &session, &Core::TerminalSession::onHistoryDown);

    connect(&m_session, &Core::TerminalSession::outputLine, m_terminalWidget, &TerminalWidget::writeLine);
    connect(&m_session, &Core::TerminalSession::promptChanged, m_terminalWidget, &TerminalWidget::setPrompt);
    connect(&m_session, &Core::TerminalSession::promptShown, m_terminalWidget, &TerminalWidget::showPrompt);
    connect(&m_session, &Core::TerminalSession::inputChanged, m_terminalWidget, &TerminalWidget::setInput);
    connect(
        &m_session, &Core::TerminalSession::exitRequested, this, [](int exitCode) { QApplication::exit(exitCode); });
}

void MainWindow::startDelayed(const QString& scriptPath)
{
    QTimer::singleShot(0, [this, scriptPath]() { m_session.start(scriptPath); });
}

void MainWindow::setupUi()
{
    setWindowTitle(tr("%1 — %2").arg(m_session.title(), QApplication::applicationName()));

    setWindowIcon(QIcon(QStringLiteral(":/icon.png")));

    setMinimumSize(400, 300);
    resize(800, 600);

    setupTerminal();
    setCentralWidget(m_terminalWidget);
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
}

} // namespace EmulatorMirea::Ui