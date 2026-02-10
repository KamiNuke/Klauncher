#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include <KAboutData>
#include <QCommandLineParser>
#include <qapplication.h>
#include <qcommandlineoption.h>
#include <qcoreapplication.h>
#include <qfileinfo.h>

#include "config.h"
#include "klaunchermanager.h"

#include "ProcessManager2.h"

void handleParser(int argc, QApplication& app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Run app in a default prefix"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
        QStringLiteral("/path/to/binary.exe"),
        QStringLiteral("Path to the Windows binary to run with Proton")
    );

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty())
    {
        QString binaryPath = args.first();
       
        QFileInfo file(binaryPath);

        if (!file.exists())
        {
            qCritical() << "Error: File does not exist:" << binaryPath;
            parser.showHelp(1);
            return;
        }
        else if (!file.isFile())
        {
            qCritical() << "Error: Path is to a directory, not a file" << binaryPath;
            parser.showHelp(1);
            return;
        }
    }

}

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain(APPLICATION_DOMAIN);
    //QApplication::setOrganizationName(QStringLiteral(ORGANIZATION_NAME));
    QApplication::setOrganizationDomain(QStringLiteral(ORGANIZATION_DOMAIN));
    QApplication::setApplicationName(QStringLiteral(APPLICATION_NAME));
    QApplication::setDesktopFileName(QStringLiteral(DESKTOP_FILE_NAME));

    if (argc > 1)
    {
        handleParser(argc, app);
        return 1;
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

    KlauncherManager* klauncherManager = new KlauncherManager();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, klauncherManager, &KlauncherManager::deleteLater);

    return app.exec();
}
