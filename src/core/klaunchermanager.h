#pragma once

#include <QQmlEngine>
#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>

#include "Process.h"

using namespace Qt::StringLiterals;

class KlauncherManager : public QObject
{
    Q_OBJECT
public:
    explicit KlauncherManager(QAnyStringView typeName);
    ~KlauncherManager() override;

public Q_SLOTS:
    QString addApp(const QVariantMap& application);
    void updateApp(const QVariantMap& application);
    void removeApp(const QVariantMap& application);
    QString loadApps();

    QString extractIcon(const QString& binaryPath);
    QString getRunners();

    QString loadDefaultSettings();
    void saveDefaultSettings(const QVariantMap& settings);
    QVariantMap getEffectiveSettings(const QVariantMap& application);

    void startApp(const QVariantMap& application);
    void stopApp(const QVariantMap& application);
    bool isAppRunning(const QString& name);

Q_SIGNALS:
    void appStarted(const QString& name);
    void appStopped(const QString& name);

private:
    std::unique_ptr<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
    QMap<QString, Klauncher::Process*> m_processes;
};
