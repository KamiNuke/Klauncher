#include "File.h"

#include <QFile>
#include <QJsonObject>
#include <QStandardPaths>

#include "Dir.h"

namespace Klauncher
{
    QVariantMap File::loadDefaultSettings()
    {
        const QString settingsFile = getDefaultSettingsFile();

        if (!QFile::exists(settingsFile))
        {
            QVariantMap env;
            env[QStringLiteral("PROTON_DLLS_UPGRADE")] = 1;
            env[QStringLiteral("PROTON_FSR4_UPGRADE")] = 0;
            env[QStringLiteral("PROTON_ENABLE_WAYLAND")] = 1;
            env[QStringLiteral("PROTONPATH")] = QString(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                + QStringLiteral("/.local/share/Steam/steamapps/common/Proton - Experimental"));
            env[QStringLiteral("WINEPREFIX")] = QString(Dir::getDataPath()
                + QStringLiteral("/prefixes/default"));


            QVariantMap defaults;
            defaults[QStringLiteral("alwaysCreatePrefix")] = false;
            defaults[QStringLiteral("prefixesLocation")] =
                QString(Dir::getDataPath() + QStringLiteral("/prefixes"));
            defaults[QStringLiteral("defaultPrefixLocation")] =
                QString(Dir::getDataPath() + QStringLiteral("/prefixes/default"));
            defaults[QStringLiteral("runnerPath")] =
                QString(Dir::getDataPath() + QStringLiteral("/runners"));
            defaults[QStringLiteral("env")] = env;
            defaults[QStringLiteral("useGameMode")] = true;
            defaults[QStringLiteral("useMangoHud")] = true;

            saveDefaultSettings(defaults);
            return defaults;
        }

        return loadJson(settingsFile);
    }

    void File::saveDefaultSettings(const QVariantMap& settings)
    {
        const QString settingsFile = getDefaultSettingsFile();
        saveJson(settings, settingsFile);
    }

    QVariantMap File::loadJson(const QString& filePath)
    {
        QFile jsonFile(filePath);
        (void)jsonFile.open(QFile::ReadOnly); // cast to void to avoid nodiscard warning
        return QJsonDocument::fromJson(jsonFile.readAll()).object().toVariantMap();
    }

    void File::saveJson(const QVariantMap& data, const QString& filePath)
    {
        QFile jsonFile(filePath);
        (void)jsonFile.open(QFile::WriteOnly); // cast to void to avoid nodiscard warning
        jsonFile.write(QJsonDocument(QJsonObject::fromVariantMap(data)).toJson());
    }

    void File::removeIcon(const QString& iconPath)
    {
        QFile file(iconPath);
        if (file.exists())
            file.remove();
    }

    QString File::getApplicationListFile()
    {
        const QString dataPath = Klauncher::Dir::getDirectoryPath() + QStringLiteral("/ApplicationsData.json");

        return dataPath;
    }

    QString File::getDefaultSettingsFile()
    {
        const QString dataPath = Klauncher::Dir::getDirectoryPath() + QStringLiteral("/settings.json");

        return dataPath;
    }

    QVariantMap File::getSettings(const QVariantMap& application)
    {
        QVariantMap effectiveSettings = loadDefaultSettings();

        for (auto it = application.constBegin(); it != application.constEnd(); ++it)
        {
            if (it.key() == QStringLiteral("env"))
            {
                QVariantMap effectiveEnv = effectiveSettings[QStringLiteral("env")].toMap();
                const QVariantMap appEnv = it.value().toMap();

                for (auto envIt = appEnv.constBegin(); envIt != appEnv.constEnd(); ++envIt)
                    effectiveEnv[envIt.key()] = envIt.value();

                effectiveSettings[QStringLiteral("env")] = effectiveEnv;
            }
            else
            {
                effectiveSettings[it.key()] = it.value();
            }
        }

        return effectiveSettings;

    }
} // Klauncher