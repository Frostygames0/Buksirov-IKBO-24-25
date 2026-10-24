#include <expected>
#include <memory>

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
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
#include "core/vfs/vfs.h"
#include "core/vfs/xmlvfsreader.h"
#include "presentation/terminalsession.h"
#include "ui/mainwindow.h"
#include "ui/terminalwidget.h"
#include "userutil.h"

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
        QStringLiteral("Путь к локации VFS."), QStringLiteral("путь")
    });
    parser.addOption({
        {QStringLiteral("s"), QStringLiteral("script")},
        QStringLiteral("Путь к скрипту"), QStringLiteral("путь")
    });

    parser.process(app);

    return Config{.vfsPath = parser.value(QStringLiteral("vfs")), .scriptPath = parser.value(QStringLiteral("script"))};
}

Core::CommandRegistry createDefaultRegistry()
{
    Core::CommandRegistry registry;
    registry.registerCommand(std::make_unique<Core::ExitCommand>());
    registry.registerCommand(std::make_unique<Core::ListCommand>());

    return registry;
}

std::unique_ptr<Core::IEnvProvider> createEnvProvider(const Core::UserIdentity& identity)
{
    return std::make_unique<Core::SystemEnvProvider>(identity);
}

std::unique_ptr<Core::IVfs> createAndReadVfs(const QString& vfsPath)
{
    std::unique_ptr<Core::IVfs> vfs = std::make_unique<Core::MemoryVfs>();

    if (vfsPath.isEmpty())
    {
        return vfs;
    }

    if (auto res = Core::XmlVfs::readFromFile(vfsPath, *vfs); !res)
    {
        qWarning().noquote() << "warning: failed to load VFS from" << vfsPath << "-" << res.error();
    }

    return vfs;
}

Core::UserIdentity createIdentityFromRealUser()
{
    return Core::UserIdentity{.userName = UserUtils::getCurrentUsername(), .machineName = QSysInfo::machineHostName()};
}

Core::Shell createShell(const Config& cfg)
{
    Core::UserIdentity identity{createIdentityFromRealUser()};

    return Core::Shell(identity, createEnvProvider(identity), createDefaultRegistry(), createAndReadVfs(cfg.vfsPath));
}

void wireUIandPresenter(Ui::MainWindow& window, Presentation::TerminalSession& session)
{
    using Session = Presentation::TerminalSession;

    Ui::TerminalWidget* widget = &window.terminalWidget();

    QObject::connect(widget, &Ui::TerminalWidget::commandEntered, &session, &Session::onEnter);
    QObject::connect(widget, &Ui::TerminalWidget::historyUpRequested, &session, &Session::onHistoryUp);
    QObject::connect(widget, &Ui::TerminalWidget::historyDownRequested, &session, &Session::onHistoryDown);

    QObject::connect(&session, &Session::outputLine, widget, &Ui::TerminalWidget::writeLine);
    QObject::connect(&session, &Session::promptChanged, widget, &Ui::TerminalWidget::setPrompt);
    QObject::connect(&session, &Session::promptShown, widget, &Ui::TerminalWidget::showPrompt);
    QObject::connect(&session, &Session::inputChanged, widget, &Ui::TerminalWidget::setInput);
    QObject::connect(&session, &Session::exitRequested, &window, [](int exitCode) { QApplication::exit(exitCode); });

    window.setWindowTitle(QStringLiteral("%1 — %2").arg(session.title(), QApplication::applicationName()));
}
} // namespace

int main(int argc, char* argv[])
{
    QApplication a(argc, argv); // NOLINT
    QApplication::setApplicationName(QStringLiteral("EmulatorMirea"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/JetBrainsMono-Regular.ttf"));

    Config cfg = parseFromArgs(a);

    Presentation::TerminalSession session{createShell(cfg)};
    Ui::MainWindow window;

    wireUIandPresenter(window, session);

    window.show();
    QTimer::singleShot(0, &session, [&] { session.start(cfg.scriptPath); });

    return QApplication::exec();
}
