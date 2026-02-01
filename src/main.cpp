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

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain(APPLICATION_DOMAIN);
    QApplication::setOrganizationName(QStringLiteral(ORGANIZATION_NAME));
    QApplication::setOrganizationDomain(QStringLiteral(ORGANIZATION_DOMAIN));
    QApplication::setApplicationName(QStringLiteral(APPLICATION_NAME));
    QApplication::setDesktopFileName(QStringLiteral(APPLICATION_NAME));

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
        QStringLiteral("0.1"),
        // Short description of what the app does.
        i18n("GUI for running windows games"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("© 2026 Vaity's team")
    );

    aboutData.addAuthor(
        i18n("Vaity"),
        i18nc("@info:credit", "Developer")
    );

    // Set aboutData as information about the app
    KAboutData::setApplicationData(aboutData);

    // Register a singleton that will be accessible from QML.
    qmlRegisterSingletonType(
        "uri.klauncher", // How the import statement should look like
        0, 1, // Major and minor versions of the import
        "About", // The name of the QML object
        [](QQmlEngine* engine, QJSEngine *) -> QJSValue {
            // Here we retrieve our aboutData and give it to the QML engine
            // to turn it into a QML type
            return engine->toScriptValue(KAboutData::applicationData());
        }
    );

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("uri.klauncher", "BaseApp");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
