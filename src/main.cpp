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
    parser.setApplicationDescription(QStringLiteral("Эмулятор оболочки"));
    parser.addHelpOption();

    parser.addOption({
        {QStringLiteral("v"), QStringLiteral("vfs")},
        QStringLiteral("Путь к локации VFS."), QStringLiteral("путь")
    });
    parser.addOption({
        {QStringLiteral("s"), QStringLiteral("script")},
        QStringLiteral("Путь к скрипту"), QStringLiteral("путь")
    });

    parser.process(app);

    cfg.vfsPath = parser.value(QStringLiteral("vfs"));
    cfg.scriptPath = parser.value(QStringLiteral("script"));

    QTextStream err(stderr);
    if (!cfg.vfsPath.isEmpty() && QFileInfo::exists(cfg.vfsPath) && !QFileInfo(cfg.vfsPath).isFile())
    {
        err << QStringLiteral("error: object at VFS path is not a file: ") << cfg.vfsPath << u'\n';
        return 2;
    }
    if (!cfg.scriptPath.isEmpty() && !QFileInfo(cfg.scriptPath).isFile())
    {
        err << QStringLiteral("error: script file not found at: ") << cfg.scriptPath << u'\n';
        return 2;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName(QStringLiteral("EmulatorMirea"));

    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/JetBrainsMono-Regular.ttf"));

    EmulatorMirea::Core::LaunchConfig cfg;
    if (const int result = parseFromArgs(a, cfg); result != 0)
    {
        return result;
    }

    EmulatorMirea::Ui::MainWindow window(std::move(cfg));
    window.show();

    return QApplication::exec();
}
