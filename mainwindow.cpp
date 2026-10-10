#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "terminalwidget.h"

#include <QMessageBox>

#include "appinfo.h"
#include "os_helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/terminal-icon.svg"));
    setWindowTitle(tr("%1 — %2").arg(getUserPlusMachineName(), kAppName));

    ui->terminalBox->setPrompt(tr("[%1]$ ").arg(getUserPlusMachineName()));
    ui->terminalBox->showPrompt();

    connect(ui->terminalBox, &TerminalWidget::commandEntered, this, &MainWindow::onCommand);
}

void MainWindow::onCommand(const QString &line)
{
    Q_UNUSED(line);
    ui->terminalBox->print("Shitty " + line);
    ui->terminalBox->showPrompt();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("About"), tr("ПРИВЕТ"));
}

