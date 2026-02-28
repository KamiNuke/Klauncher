#ifndef KLAUNCHER_FILE_H
#define KLAUNCHER_FILE_H
#include <QJsonDocument>
#include <QString>

namespace Klauncher
{
    class File
    {
    public:
        enum Type
        {
            AppData,
            Settings,

        };

        static QVariantMap loadDefaultSettings();
        static void saveDefaultSettings(const QVariantMap& settings);

        static QVariantMap loadJson(const QString& fileName);
        static void saveJson(const QVariantMap& jsonDoc, const QString& filePath);

        static void removeIcon(const QString& iconPath);
        static QString getApplicationListFile();
        static QString getDefaultSettingsFile();

        static QVariantMap getSettings(const QVariantMap& application);

    private:

    };
} // Klauncher

#endif //KLAUNCHER_FILE_H