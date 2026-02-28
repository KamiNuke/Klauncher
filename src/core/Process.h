#ifndef KLAUNCHER_PROCESS_H
#define KLAUNCHER_PROCESS_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>

namespace Klauncher
{
    class Process : public QObject
    {
        Q_OBJECT
    public:
        explicit Process(QObject* parent = nullptr, const QVariantMap& appInfo = QVariantMap());

        void start();
        void stop();

    private:
        void setupEnvironment();
        void killBinaryProcess();
        void killWineProcess();

    Q_SIGNALS:
        void started();
        void finished(int exitCode, QProcess::ExitStatus exitStatus);
        void errorOccurred(QProcess::ProcessError error);
    private:
        QProcess m_process;
        qint64 pid;
        QVariantMap m_appInfo;
    };
} // Klauncher

#endif //KLAUNCHER_PROCESS_H