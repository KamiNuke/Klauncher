#include "processmanager.h"

ProcessManager::ProcessManager(QObject* parent)
    : QObject(parent)
{

}

void ProcessManager::start(const QVariantMap& application)
{
    QString binaryPath = getBinaryPath(application);

    if (m_runningProcesses.contains(binaryPath))
        return;

    AppProcess* process = new AppProcess(application, this);

    connect(process, &AppProcess::started, this,
            [this, binaryPath]() { Q_EMIT started(binaryPath); });
    connect(process, &AppProcess::finished, this,
            [this, binaryPath]() { onProcessFinished(binaryPath); });
    connect(process, &AppProcess::errorOccurred, this,
            [this, binaryPath](QProcess::ProcessError error) {
                Q_EMIT this->error(binaryPath, QString::number(error));
            });

    m_runningProcesses[binaryPath] = process;
    process->start();
}

void ProcessManager::stop(const QVariantMap& application)
{
    QString binaryPath = getBinaryPath(application);

    if (!m_runningProcesses.contains(binaryPath)) {
        return;
    }

    m_runningProcesses[binaryPath]->stop();
    m_runningProcesses.remove(binaryPath);
}

bool ProcessManager::isRunning(const QVariantMap& application)
{
    QString binaryPath = getBinaryPath(application);

    if (!m_runningProcesses.contains(binaryPath))
        return false;

    return m_runningProcesses[binaryPath]->isRunning();
}

void ProcessManager::onProcessFinished(const QString& binaryPath)
{
    if (m_runningProcesses.contains(binaryPath)) {
        AppProcess* process = m_runningProcesses.take(binaryPath);
        process->deleteLater();
        Q_EMIT stopped(binaryPath);
    }
}

QString ProcessManager::getBinaryPath(const QVariantMap& application) const
{
    return application.value(QStringLiteral("binaryPath")).toString();
}
