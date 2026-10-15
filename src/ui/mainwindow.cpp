#include "mainwindow.h"

#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QIcon>
#include <QString>

#include "ui/terminalostream.h"
#include "ui/terminalwidget.h"

namespace EmulatorMirea::Ui
{

MainWindow::MainWindow() : QMainWindow(nullptr), m_shell([](int code) { QApplication::exit(code); })
{
    setupUi();
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

    // Enter pressed: run through the shell, show output, new prompt
    connect(m_terminalBox,
            &TerminalWidget::commandEntered,
            this,
            [this](const QString& line)
            {
                TerminalOStream out(*m_terminalBox);
                m_shell.submit(line, out);
                m_historyIndex = m_shell.historySize();

                m_terminalBox->setPrompt(m_shell.prompt());
                m_terminalBox->showPrompt();
            });

    connect(m_terminalBox,
            &TerminalWidget::historyUpRequested,
            this,
            [this]
            {
                if (m_shell.historySize() == 0)
                    return;

                if (m_historyIndex > 0)
                    --m_historyIndex;

                m_terminalBox->setInput(m_shell.historyAt(m_historyIndex));
            });

    connect(m_terminalBox,
            &TerminalWidget::historyDownRequested,
            this,
            [this]
            {
                const qsizetype size = m_shell.historySize();
                if (size == 0)
                    return;

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
            });

    m_terminalBox->setPrompt(m_shell.prompt());
    m_terminalBox->showPrompt();
}

} // namespace EmulatorMirea::Ui
