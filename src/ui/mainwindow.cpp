#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFrame>
#include <QIcon>
#include <QString>
#include <QTimer>

#include "ui/terminalwidget.h"

namespace EmulatorMirea::Ui
{

MainWindow::MainWindow() : QMainWindow(nullptr)
{
    setupUi();
}

TerminalWidget& MainWindow::terminalWidget()
{
    return *m_terminalWidget;
}

void MainWindow::setupUi()
{
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