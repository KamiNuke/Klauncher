#include <QApplication>
#include <QtQml>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include <KAboutData>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QIcon>

#include "config.h"
#include "File.h"
#include "klaunchermanager.h"
#include "Process.h"

int parseAndLaunch(const QApplication& app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Run app in a default prefix"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
        QStringLiteral("binary"),
        QStringLiteral("Path to the Windows binary to run with Proton")
    );

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty())
    {
        qCritical() << "Error: No binary path provided";
        parser.showHelp(-1);
    }

    const QString binaryPath = args.first();
    if (!QFile::exists(binaryPath))
    {
        qCritical() << "Error: File does not exist:" << binaryPath;
        return -1;
    }

    QVariantMap appInfo = Klauncher::File::loadDefaultSettings();
    appInfo[QStringLiteral("binaryPath")] = binaryPath;

    Klauncher::Process* process = new Klauncher::Process(nullptr, appInfo);

    QObject::connect(process, &Klauncher::Process::finished,
        &app, [&app](int exitCode, QProcess::ExitStatus)
        {
            app.exit(exitCode);
        });

    QObject::connect(process, &Klauncher::Process::errorOccurred,
        &app, [&app](QProcess::ProcessError error)
        {
            qCritical() << "Process error:" << error;
            app.exit(-1);
        });

    process->start();
    return app.exec();
}

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("klauncher")));
    KLocalizedString::setApplicationDomain(APPLICATION_DOMAIN);
    //QApplication::setOrganizationName(QStringLiteral(ORGANIZATION_NAME));
    QApplication::setOrganizationDomain(QStringLiteral(ORGANIZATION_DOMAIN));
    QApplication::setApplicationName(QStringLiteral(APPLICATION_NAME));
    QApplication::setDesktopFileName(QStringLiteral(DESKTOP_FILE_NAME));

    if (argc > 1)
    {
        return parseAndLaunch(app);
    }

    QApplication::setStyle(QStringLiteral("breeze"));
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    KAboutData aboutData
    (
        // The program name used internally.
        QStringLiteral(APPLICATION_DOMAIN),
        // A displayable program name string.
        QStringLiteral(APPLICATION_NAME),
        // The program version string.
        QStringLiteral(KLAUNCHER_VERSION_STRING),
        // Short description of what the app does.
        i18n("GUI for running Windows games"),
        // The license this code is released under.
        KAboutLicense::GPL_V3,
        // Copyright Statement.
        i18n("Kami Nuke (c) 2026")
    );

    aboutData.addAuthor(
        i18n("KamiNuke"),
        i18nc("@info:credit", "Developer")
    );

    aboutData.setBugAddress("https://github.com/KamiNuke/Klauncher");

    // Set aboutData as information about the app
    KAboutData::setApplicationData(aboutData);

    //QQmlApplicationEngine engine;

    KlauncherManager* klauncherManager = new KlauncherManager("Main");

    QObject::connect(&app, &QCoreApplication::aboutToQuit, klauncherManager, &KlauncherManager::deleteLater);

    return app.exec();
}
