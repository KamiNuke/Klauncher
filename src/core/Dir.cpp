#include "Dir.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace Klauncher
{
    QString Dir::getDirectoryPath()
    {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

        const QDir dir(configPath);
        if (!dir.mkpath(QStringLiteral(".")))
            qWarning("Couldn't create directory");

        return configPath;
    }

    QString Dir::getIconsLocation()
    {
        const QString iconsPath = getDirectoryPath() + QStringLiteral("/icons");

        const QDir dir(iconsPath);
        if (!dir.mkpath(QStringLiteral(".")))
            qWarning("Couldn't create directory");

        return iconsPath;
    }

    QString Dir::getHomeDirectory()
    {
        const QString homeDirectory = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

        return homeDirectory;
    }

    QString Dir::getDataPath()
    {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
} // Klauncher