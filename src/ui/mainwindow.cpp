#include "ui/mainwindow.h"
#include "ui/terminalwidget.h"

#include <QMessageBox>
#include <QString>
#include <QMenu>
#include <QMenuBar>

#include "appinfo.h"

namespace EmulatorMirea
{

MainWindow::MainWindow()
    : QMainWindow(nullptr)
{
    setupUi();
}

void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle(tr("%1 — %2").arg(m_session.fancyName(), AppInfo::kAppName));
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

    m_terminalBox->setStyleSheet(
        "QPlainTextEdit {"
        "    border: none;"
        "    border-radius: 0px;"
        "    background-color: #000000;"
        "    color: #dcdcdc;"
        "}");

    m_terminalBox->setFrameShape(QFrame::NoFrame);
    m_terminalBox->setBackgroundVisible(false);

    // Enter pressed: save to history, run, show output, new prompt
    connect(m_terminalBox, &TerminalWidget::commandEntered, this,
            [this](const QString& line)
            {
                m_session.addHistory(line);   // skips blank lines and consecutive duplicates

                if (!line.trimmed().isEmpty())
                    m_terminalBox->print(m_interpreter.Execute(line, m_session));

                m_terminalBox->showPrompt();
            });

    // Up arrow: older history item
    connect(m_terminalBox, &TerminalWidget::historyUpRequested, this, [this] {
        m_terminalBox->setInput(m_session.previousHistoryItem());
    });

    // Down arrow: newer item, or empty input past the newest
    connect(m_terminalBox, &TerminalWidget::historyDownRequested, this, [this] {
        m_terminalBox->setInput(m_session.nextHistoryItem());
    });

    m_terminalBox->setPrompt(tr("[%1]$ ").arg(m_session.fancyName()));
    m_terminalBox->showPrompt();
}

}

