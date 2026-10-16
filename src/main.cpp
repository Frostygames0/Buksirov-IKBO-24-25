#include <cstdio>
#include <utility>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QFontDatabase>
#include <QTextStream>

#include "core/launchconfig.h"
#include "ui/mainwindow.h"

static int parseFromArgs(const QCoreApplication& app, EmulatorMirea::Core::LaunchConfig& cfg)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Эмулятор оболочки");
    parser.addHelpOption();

    parser.addOption({
        {"v", "vfs"},
        "Путь к локации VFS.", "путь"
    });
    parser.addOption({
        {"s", "script"},
        "Путь к скрипту", "путь"
    });

    parser.process(app);

    cfg.vfsPath = parser.value("vfs");
    cfg.scriptPath = parser.value("script");

    QTextStream err(stderr);
    if (!cfg.vfsPath.isEmpty() && !QFileInfo(cfg.vfsPath).isDir())
    {
        err << "ошибка: путь к VFS не является директорией: " << cfg.vfsPath << "\n";
        return 2;
    }
    if (!cfg.scriptPath.isEmpty() && !QFileInfo(cfg.scriptPath).isFile())
    {
        err << "ошибка: файл скрипта не найден: " << cfg.scriptPath << "\n";
        return 2;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName("EmulatorMirea");

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Regular.ttf");

    EmulatorMirea::Core::LaunchConfig cfg;
    if (const int result = parseFromArgs(a, cfg); result != 0)
        return result;

    EmulatorMirea::Ui::MainWindow window(std::move(cfg));
    window.show();

    return QApplication::exec();
}
