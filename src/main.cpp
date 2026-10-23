#include <cstdio>
#include <expected>
#include <memory>
#include <utility>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QFontDatabase>
#include <QTextStream>

#include "config.h"
#include "core/commands/command.h"
#include "core/commands/registry.h"
#include "core/envprovider.h"
#include "core/shell.h"
#include "core/systemenvprovider.h"
#include "core/useridentity.h"
#include "core/vfs/vfs.h"
#include "core/vfs/xmlvfsreader.h"
#include "ui/mainwindow.h"
#include "userutil.h"

using namespace EmulatorMirea;

static Config parseFromArgs(const QCoreApplication& app)
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

    Config cfg;
    cfg.vfsPath = parser.value(QStringLiteral("vfs"));
    cfg.scriptPath = parser.value(QStringLiteral("script"));

    if (!cfg.vfsPath.isEmpty() && QFileInfo::exists(cfg.vfsPath) && !QFileInfo(cfg.vfsPath).isFile())
    {
        qWarning() << QStringLiteral("warning: object at VFS path is not a file: ") << cfg.vfsPath << u'\n';
    }
    if (!cfg.scriptPath.isEmpty() && !QFileInfo(cfg.scriptPath).isFile())
    {
        qWarning() << QStringLiteral("warning: script file not found at: ") << cfg.scriptPath << u'\n';
    }

    return cfg;
}

Core::CommandRegistry createDefaultRegistry()
{
    Core::CommandRegistry registry;
    registry.registerCommand(std::make_unique<Core::ExitCommand>());

    return registry;
}

Core::Shell createShell(const Config& cfg)
{
    Core::UserIdentity identity{.userName = UserUtils::getCurrentUsername(),
                                .machineName = QSysInfo::machineHostName()};

    std::unique_ptr<Core::IEnvProvider> envProvider = std::make_unique<Core::SystemEnvProvider>(identity);
    std::unique_ptr<Core::IVfs> vfs = std::make_unique<Core::MemoryVfs>();

    std::expected<void, QString> res = Core::XmlVfs::readFromFile(cfg.vfsPath, *vfs);
    if (!res)
    {
        qWarning() << QStringLiteral("warning: vfs file not found at: ") << cfg.scriptPath << u'\n';
    }

    return Core::Shell{std::move(identity), std::move(envProvider), createDefaultRegistry(), std::move(vfs)};
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName(QStringLiteral("EmulatorMirea"));

    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/JetBrainsMono-Regular.ttf"));

    Config cfg = parseFromArgs(a);
    Core::Shell shell = createShell(cfg);

    Ui::MainWindow window(shell);

    window.show();

    window.startup(cfg);

    return QApplication::exec();
}
