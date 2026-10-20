#include <cstdio>
#include <expected>
#include <memory>
#include <qchar.h>
#include <qsysinfo.h>
#include <utility>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QFontDatabase>
#include <QTextStream>

#include "core/commands/command.h"
#include "core/commands/executor.h"
#include "core/commands/registry.h"
#include "core/interpreter.h"
#include "core/launchconfig.h"
#include "core/shell.h"
#include "core/systemenvprovider.h"
#include "core/useridentity.h"
#include "core/vfs/xmlvfsreader.h"
#include "ui/mainwindow.h"
#include "utils/user.h"

using namespace EmulatorMirea;

static int parseFromArgs(const QCoreApplication& app, Core::LaunchConfig& cfg)
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

    Core::CommandRegistry registry;
    registry.registerCommand(std::make_unique<Core::ExitCommand>());
    registry.registerCommand(std::make_unique<Core::StubCommand>(QStringLiteral("ls")));
    registry.registerCommand(std::make_unique<Core::StubCommand>(QStringLiteral("cd")));

    Core::CommandExecutor executor{registry};
    Core::Interpreter interpreter{executor};

    Core::UserIdentity identity{.userName = UserUtils::getCurrentUsername(),
                                .machineName = QSysInfo::machineHostName()};
    Core::SystemEnvProvider envProvider{identity};

    std::expected<Core::MemoryVfs, QString> vfs = Core::XmlVfs::readFromFile(cfg.vfsPath);

    if (!vfs)
    {
        // todo
    }

    EmulatorMirea::Ui::MainWindow window(std::move(cfg));
    window.show();

    return QApplication::exec();
}
