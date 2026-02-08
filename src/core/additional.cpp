#include "additional.h"

#include <QStandardPaths>
#include <QDir>

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

QString getHomeDirectory()
{
    const QString homeDirectory = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    return homeDirectory;
}

void removeIcon(QString iconPath)
{
    QFile file(iconPath);
    if (file.exists())
        file.remove();
}
