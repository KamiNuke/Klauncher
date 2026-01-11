#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>

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

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("uri.klauncher", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
