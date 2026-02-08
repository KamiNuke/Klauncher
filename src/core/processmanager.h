#ifndef KLAUNCHER_PROCESSMANAGER_H
#define KLAUNCHER_PROCESSMANAGER_H

#include <QQmlEngine>

#include "appprocess.h"

class ProcessManager : public QObject
{
	Q_OBJECT
public:
    explicit ProcessManager(QObject* parent = nullptr);

public Q_SLOTS:
    void start(const QVariantMap& application);
    void stop(const QVariantMap& application);
    bool isRunning(const QVariantMap& application);

Q_SIGNALS:
    void started(QString binaryPath);
    void stopped(QString binaryPath);
    void error(QString binaryPath, QString error);

private:
    QMap<QString, AppProcess*> m_runningProcesses;

    void onProcessFinished(const QString& appName);
    [[nodiscard]] QString getBinaryPath(const QVariantMap& application) const;
};


#endif //KLAUNCHER_PROCESSMANAGER_H