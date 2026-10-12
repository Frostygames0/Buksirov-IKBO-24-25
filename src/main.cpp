#include <QApplication>
#include <QFontDatabase>

#include "ui/mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName("EmulatorMirea");

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Regular.ttf");

    EmulatorMirea::Ui::MainWindow w;
    w.show();

    return QApplication::exec();
}
