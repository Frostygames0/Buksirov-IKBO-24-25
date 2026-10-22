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
    
MainWindow::MainWindow(Core::Emulator& emulator, QWidget* parent) : QMainWindow(parent)
{
    setupUi(emulator.title());

    emulator.setView(m_terminalBox);

    connect(m_terminalBox, &TerminalWidget::commandEntered, &emulator, &Core::Emulator::onCommandEntered);
    connect(m_terminalBox, &TerminalWidget::historyUpRequested, &emulator, &Core::Emulator::onHistoryUp);
    connect(m_terminalBox, &TerminalWidget::historyDownRequested, &emulator, &Core::Emulator::onHistoryDown);
    connect(&emulator, &Core::Emulator::exitRequested, qApp, &QCoreApplication::exit, Qt::QueuedConnection);
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

} // namespace EmulatorMirea::Ui
