#ifndef KLAUNCHER_APPPROCESS_H
#define KLAUNCHER_APPPROCESS_H
#include <QMap>
#include <QObject>
#include <QProcess>
#include <QVariant>

class AppProcess : public QObject
{
    Q_OBJECT
public:
    explicit AppProcess(const QVariantMap& application, QObject* parent = nullptr);

    void start();
    void stop();
    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] QString getProcessName() const;

Q_SIGNALS:
    void started();
    void finished(int exitCode);
    void errorOccurred(QProcess::ProcessError error);

private:
    QProcess m_process;
    QVariantMap m_application;

    void setupEnvironment();
    void killBinaryProcess();
    void killWineProcess();
};

#endif //KLAUNCHER_APPPROCESS_H