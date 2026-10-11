#include "ui/mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Regular.ttf");

    EmulatorMirea::MainWindow w;
    w.show();

    return QApplication::exec();
}
