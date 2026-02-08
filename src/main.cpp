#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include <KAboutData>

#include "config.h"
#include "core/klaunchermanager.h"

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain(APPLICATION_DOMAIN);
    //QApplication::setOrganizationName(QStringLiteral(ORGANIZATION_NAME));
    QApplication::setOrganizationDomain(QStringLiteral(ORGANIZATION_DOMAIN));
    QApplication::setApplicationName(QStringLiteral(APPLICATION_NAME));
    QApplication::setDesktopFileName(QStringLiteral(DESKTOP_FILE_NAME));

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
        KAboutLicense::GPL,
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
