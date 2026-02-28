#include "Process.h"

#include <QDir>
#include <QProcess>
#include <csignal>
#include <QCoreApplication>
#include <QJsonObject>
#include <QTimer>
#include <QJsonValue>
#include <QStandardPaths>

#include "File.h"

namespace Klauncher
{
    Process::Process(QObject* parent, const QVariantMap& appInfo)
        : QObject(parent)
    {
        qInfo() << Q_FUNC_INFO;

        connect(&m_process, &QProcess::started,
            this, &Process::started);

        connect(&m_process, &QProcess::errorOccurred,
            this, &Process::errorOccurred);

        connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Process::finished);

        m_appInfo = Klauncher::File::getSettings(appInfo);
    }

    void Process::start()
    {
        qInfo() << Q_FUNC_INFO;

        if (m_process.state() != QProcess::NotRunning)
        {
            qWarning() << "App already running:" << m_appInfo[QStringLiteral("runnerPath")].toString();
            return;
        }

        setupEnvironment();

        QString program = QStandardPaths::findExecutable(QStringLiteral("umu-run"));
        QStringList args;

        bool useMangohud = m_appInfo[QStringLiteral("useMangoHud")].toBool();
        bool useGameMode = m_appInfo[QStringLiteral("useGameMode")].toBool();
        if (useMangohud) {
            args.prepend(program);
            program = QStringLiteral("mangohud");
        }
        if (useGameMode) {
            args.prepend(program);
            program = QStringLiteral("gamemoderun");
        }

        args << m_appInfo[QStringLiteral("binaryPath")].toString();

        m_process.setProgram(program);
        m_process.setArguments(args);
        m_process.setProcessChannelMode(QProcess::ForwardedChannels);
        m_process.start();
    }

    void Process::stop()
    {
        qInfo() << Q_FUNC_INFO;

        killBinaryProcess();
        killWineProcess();
        m_process.terminate();

        QTimer::singleShot(5000, this, [this]()
        {
            if (m_process.state() != QProcess::NotRunning)
                m_process.kill();
        });
    }

    void Process::setupEnvironment()
    {
        qInfo() << Q_FUNC_INFO;

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        const QVariantMap appEnv = m_appInfo.value(QStringLiteral("env")).toMap();

        for (auto it = appEnv.constBegin(); it != appEnv.constEnd(); ++it)
            env.insert(it.key(), it.value().toString());

        m_process.setProcessEnvironment(env);
    }

    void Process::killBinaryProcess()
    {
        qInfo() << Q_FUNC_INFO;

        QProcess pkillProcess;
        pkillProcess.start(QStandardPaths::findExecutable(QStringLiteral("pkill")), {QStringLiteral("-f"),
            m_appInfo[QStringLiteral("binaryPath")].toString()});
        pkillProcess.waitForFinished(2000);
    }

    void Process::killWineProcess()
    {
        qInfo() << Q_FUNC_INFO;

        const QVariantMap env = m_appInfo[QStringLiteral("env")].toMap();
        const QString winePrefix = env[QStringLiteral("WINEPREFIX")].toString();
        const QString protonPath = env[QStringLiteral("PROTONPATH")].toString();

        QProcess killProcess;
        QProcessEnvironment processEnv = QProcessEnvironment::systemEnvironment();
        processEnv.insert(QStringLiteral("WINEPREFIX"), winePrefix);

        killProcess.setProcessEnvironment(processEnv);
        killProcess.start(QString(protonPath + QStringLiteral("/files/bin/wineserver")),
            {QStringLiteral("-k")});
        killProcess.waitForFinished(5000);
    }
} // Klauncher