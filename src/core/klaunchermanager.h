#pragma once

#include <QQmlEngine>
#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>

#include "processmanager.h"

using namespace Qt::StringLiterals;

class KlauncherManager : public QObject
{
    Q_OBJECT
public:
    explicit KlauncherManager();
    ~KlauncherManager() override;

public Q_SLOTS:
    QString addApp(const QVariantMap& application);
    void updateApp(const QVariantMap& application);
    void removeApp(const QVariantMap& application);
    QString loadApps();

    QString extractIcon(const QString& binaryPath);
    QString getRunners();

private:
    std::unique_ptr<QQmlApplicationEngine, QScopedPointerDeleteLater> m_engine;
    ProcessManager* m_processManager;
};
