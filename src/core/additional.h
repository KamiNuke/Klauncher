#ifndef KLAUNCHER_ADDITIONAL_H
#define KLAUNCHER_ADDITIONAL_H
#include <QString>

QString getIconsLocation();
QString getAppListLocation();
QString getSettingsLocation();
QString getHomeDirectory();
void removeIcon(QString iconPath);

QString loadDefSettings();
QVariantMap getSettings(const QVariantMap& application);

#endif //KLAUNCHER_ADDITIONAL_H