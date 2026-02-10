#include "additional.h"

#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>


static QString getDirectoryPath()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir(configPath);
    if (!dir.mkpath(QStringLiteral(".")))
        qWarning("Couldn't create directory");

    return configPath;
}

QString getIconsLocation()
{
    const QString iconsPath = getDirectoryPath() + QStringLiteral("/icons");

    QDir dir(iconsPath);
    if (!dir.mkpath(QStringLiteral(".")))
        qWarning("Couldn't create directory");

    return iconsPath;
}

QString getAppListLocation()
{
    const QString dataPath = getDirectoryPath() + QStringLiteral("/ApplicationsData.json");

    return dataPath;
}

QString getSettingsLocation()
{
    const QString dataPath = getDirectoryPath() + QStringLiteral("/settings.json");

    return dataPath;
}

QString getHomeDirectory()
{
    const QString homeDirectory = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    return homeDirectory;
}

QString loadDefSettings()
{
    QString settingsPath = getSettingsLocation();

    QFile file(settingsPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QJsonObject jsonObj;
        jsonObj[QStringLiteral("useMangoHud")] = true;
        jsonObj[QStringLiteral("useGameMode")] = true;
        jsonObj[QStringLiteral("useWayland")] = false;
        jsonObj[QStringLiteral("useDLSSUpgrade")] = false;
        jsonObj[QStringLiteral("useFSR4Upgrade")] = false;

        return QString::fromUtf8(QJsonDocument(jsonObj).toJson());
    }

    QTextStream fileText(&file);
    QString jsonString = fileText.readAll();
    file.close();

    return jsonString;
}

QVariantMap getSettings(const QVariantMap& application)
{
    QJsonDocument defaultDoc = QJsonDocument::fromJson(loadDefSettings().toUtf8());
    QVariantMap effectiveSettings = defaultDoc.object().toVariantMap();

    if (application.contains(QStringLiteral("useMangoHud")))
        effectiveSettings[QStringLiteral("useMangoHud")] = application.value(QStringLiteral("useMangoHud"));

    if (application.contains(QStringLiteral("useGameMode")))
        effectiveSettings[QStringLiteral("useGameMode")] = application.value(QStringLiteral("useGameMode"));

    if (application.contains(QStringLiteral("useWayland")))
        effectiveSettings[QStringLiteral("useWayland")] = application.value(QStringLiteral("useWayland"));

    if (application.contains(QStringLiteral("useDLSSUpgrade")))
        effectiveSettings[QStringLiteral("useDLSSUpgrade")] = application.value(QStringLiteral("useDLSSUpgrade"));

    if (application.contains(QStringLiteral("useFSR4Upgrade")))
        effectiveSettings[QStringLiteral("useFSR4Upgrade")] = application.value(QStringLiteral("useFSR4Upgrade"));

    return effectiveSettings;
}

void removeIcon(QString iconPath)
{
    QFile file(iconPath);
    if (file.exists())
        file.remove();
}
