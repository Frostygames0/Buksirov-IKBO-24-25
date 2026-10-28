#include <expected>
#include <memory>

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QObject>
#include <QString>
#include <QSysInfo>
#include <QTimer>

#include "core/commands/command.h"
#include "core/commands/registry.h"
#include "core/envprovider.h"
#include "core/shell.h"
#include "core/systemenvprovider.h"
#include "core/useridentity.h"
#include "core/vfs/memoryvfs.h"
#include "core/vfs/vfs.h"
#include "platform/scriptreader.h"
#include "platform/userutil.h"
#include "platform/xmlvfsreader.h"
#include "presentation/terminalpresenter.h"
#include "ui/mainwindow.h"
#include "ui/terminalwidget.h"

using namespace EmulatorMirea;

namespace
{
struct Config
{
    QString vfsPath;
    QString scriptPath;
};

Config parseFromArgs(const QCoreApplication& app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Эмулятор оболочки"));
    parser.addHelpOption();

    parser.addOption({
        {QStringLiteral("v"), QStringLiteral("vfs")},
        QStringLiteral("Путь к локации VFS."),
        QStringLiteral("путь")
    });
    parser.addOption({
        {QStringLiteral("s"), QStringLiteral("script")},
        QStringLiteral("Путь к скрипту"),
        QStringLiteral("путь")
    });

    parser.process(app);

    return Config{
        .vfsPath = parser.value(QStringLiteral("vfs")),
        .scriptPath = parser.value(QStringLiteral("script")),
    };
}

Core::CommandRegistry createDefaultRegistry(const QString& vfsLoadPath)
{
    Core::CommandRegistry registry;
    registry.registerCommand(std::make_unique<Core::HelpCommand>());
    registry.registerCommand(std::make_unique<Core::ExitCommand>());
    registry.registerCommand(std::make_unique<Core::ListCommand>());
    registry.registerCommand(std::make_unique<Core::VfsInitCommand>(vfsLoadPath));
    registry.registerCommand(std::make_unique<Core::ChangeDirectoryCommand>());
    registry.registerCommand(std::make_unique<Core::ClearCommand>());
    registry.registerCommand(std::make_unique<Core::EchoCommand>());

    return registry;
}

std::unique_ptr<Core::IEnvProvider> createEnvProvider(const Core::UserIdentity& identity)
{
    return std::make_unique<Core::SystemEnvProvider>(identity);
}

std::unique_ptr<Core::IVfs> createAndReadVfs(const QString& vfsPath, QStringList& diagnostics)
{
    std::unique_ptr<Core::IVfs> vfs = std::make_unique<Core::MemoryVfs>();

    if (vfsPath.isEmpty())
    {
        return vfs;
    }

    if (auto res = Platform::XmlVfs::readFromFile(vfsPath, *vfs); !res)
    {
        diagnostics << QStringLiteral("warning: ") + res.error();
    }

    diagnostics << QStringLiteral("info: Loaded VFS from path '%1'").arg(QDir::cleanPath(vfsPath));

    return vfs;
}

QStringList readScript(const QString& scriptPath, QStringList& diagnostics)
{
    if (scriptPath.isEmpty())
    {
        diagnostics << QStringLiteral("warning: Unable to read script. Path is empty");
        return {};
    }

    auto res = Platform::ScriptReader::readScript(scriptPath);
    if (!res)
    {
        diagnostics << QStringLiteral("warning: ") + res.error();
        return {};
    }

    diagnostics << QStringLiteral("info: Loaded startup script from path '%1'")
                       .arg(QDir::cleanPath(scriptPath));
    return *res;
}

Core::UserIdentity createIdentityFromRealUser()
{
    return Core::UserIdentity{
        .userName = Platform::UserUtils::getCurrentUsername(),
        .machineName = QSysInfo::machineHostName(),
    };
}

Core::Shell createShell(const Config& cfg, QStringList& errors)
{
    Core::UserIdentity identity{createIdentityFromRealUser()};

    return Core::Shell(identity,
                       createEnvProvider(identity),
                       createAndReadVfs(cfg.vfsPath, errors),
                       createDefaultRegistry(cfg.vfsPath));
}

void wireUIandPresenter(Ui::MainWindow& window, Presentation::TerminalPresenter& session)
{
    using Presenter = Presentation::TerminalPresenter;

    Ui::TerminalWidget* widget = &window.terminalWidget();

    QObject::connect(widget, &Ui::TerminalWidget::commandEntered, &session, &Presenter::onEnter);
    QObject::connect(widget, &Ui::TerminalWidget::historyUpRequested, &session, &Presenter::onHistoryUp);
    QObject::connect(widget, &Ui::TerminalWidget::historyDownRequested, &session, &Presenter::onHistoryDown);

    QObject::connect(&session, &Presenter::output, widget, &Ui::TerminalWidget::write);
    QObject::connect(&session, &Presenter::outputLine, widget, &Ui::TerminalWidget::writeLine);
    QObject::connect(&session, &Presenter::promptShown, widget, &Ui::TerminalWidget::showPrompt);
    QObject::connect(&session, &Presenter::inputSet, widget, &Ui::TerminalWidget::setInput);
    QObject::connect(&session, &Presenter::exitRequested, qApp, &QCoreApplication::exit);
    QObject::connect(&session, &Presenter::cleared, widget, &Ui::TerminalWidget::clearScreen);

    window.setWindowTitle(QStringLiteral("%1 — %2").arg(session.title(), QApplication::applicationName()));
}
} // namespace

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName(QStringLiteral("EmulatorMirea"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/JetBrainsMono-Regular.ttf"));

    Config cfg = parseFromArgs(a);

    QStringList errorsToPassThrough;

    Presentation::TerminalPresenter presenter{createShell(cfg, errorsToPassThrough)};
    Ui::MainWindow window;
    wireUIandPresenter(window, presenter);

    QStringList scriptToLaunch = readScript(cfg.scriptPath, errorsToPassThrough);

    window.show();
    QTimer::singleShot(0, &presenter, [&] {
        presenter.runStartup({.banner = errorsToPassThrough, .script = scriptToLaunch});
    });

    return QApplication::exec();
}
