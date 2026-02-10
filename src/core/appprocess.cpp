#include "appprocess.h"

#include <QFileInfo>
#include <qtimer.h>
#include <signal.h>

#include "additional.h"

AppProcess::AppProcess(const QVariantMap& application, QObject* parent)
    : QObject(parent), m_application(application)
{
    connect(&m_process, &QProcess::started,
this, &AppProcess::started);

    connect(&m_process, &QProcess::errorOccurred,
this, &AppProcess::errorOccurred);

    connect(&m_process,
        QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &AppProcess::finished);
}

void AppProcess::start()
{
    qInfo() << Q_FUNC_INFO;

    if (m_process.state() != QProcess::NotRunning)
    {
        qWarning() << "App already running:" << getProcessName();
        return;
    }

    QVariantMap effectiveSettings = getSettings(m_application);
    m_application[QStringLiteral("useMangoHud")] = effectiveSettings.value(QStringLiteral("useMangoHud"));
    m_application[QStringLiteral("useGameMode")] = effectiveSettings.value(QStringLiteral("useGameMode"));
    m_application[QStringLiteral("useWayland")] = effectiveSettings.value(QStringLiteral("useWayland"));
    m_application[QStringLiteral("useDLSSUpgrade")] = effectiveSettings.value(QStringLiteral("useDLSSUpgrade"));
    m_application[QStringLiteral("useFSRUpgrade")] = effectiveSettings.value(QStringLiteral("useFSRUpgrade"));

    setupEnvironment();

    QString program;
    QStringList args;

    bool useGameMode = m_application.value(QStringLiteral("useGameMode")).toBool();
    bool useMangoHud = m_application.value(QStringLiteral("useMangoHud")).toBool();

    if (useGameMode && useMangoHud)
    {
        program = QStringLiteral("gamemoderun");
        args << QStringLiteral("mangohud")
             << QStringLiteral("/usr/bin/umu-run");
    }
    else if (useGameMode)
    {
        program = QStringLiteral("gamemoderun");
        args << QStringLiteral("/usr/bin/umu-run");
    }
    else if (useMangoHud)
    {
        program = QStringLiteral("mangohud");
        args << QStringLiteral("/usr/bin/umu-run");
    }
    else
    {
        program = QStringLiteral("/usr/bin/umu-run");
    }

    args << m_application.value(QStringLiteral("binaryPath")).toString();

    m_process.setProgram(program);
    m_process.setArguments(args);
    m_process.start();
}

void AppProcess::stop()
{
    killBinaryProcess();
    killWineProcess();
    m_process.terminate();

    QTimer::singleShot(5000, [this]()
    {
        if (m_process.state() != QProcess::NotRunning)
            m_process.kill();
    });
}

void AppProcess::setupEnvironment()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"),
        m_application[QStringLiteral("prefixPath")].toString());
    env.insert(QStringLiteral("PROTONPATH"),
    m_application[QStringLiteral("runnerPath")].toString());

    if (m_application.value(QStringLiteral("useWayland")).toBool())
        env.insert(QStringLiteral("PROTON_ENABLE_WAYLAND"), QStringLiteral("1"));

    if (m_application.value(QStringLiteral("useDLSSUpgrade")).toBool())
        env.insert(QStringLiteral("PROTON_DLLS_UPGRADE"), QStringLiteral("1"));

    if (m_application.value(QStringLiteral("useFSRUpgrade")).toBool())
        env.insert(QStringLiteral("PROTON_FSR4_UPGRADE"), QStringLiteral("1"));

    m_process.setProcessEnvironment(env);
}

void AppProcess::killBinaryProcess()
{
    QString binaryPath = m_application[QStringLiteral("binaryPath")].toString();
    QFileInfo fileInfo(binaryPath);
    QString binaryName = fileInfo.fileName();

    QProcess pkillProcess;
    pkillProcess.start(QStringLiteral("pkill"), {QStringLiteral("-f"), binaryName});
    pkillProcess.waitForFinished(2000);
}

void AppProcess::killWineProcess()
{
    QProcess killProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"),
        m_application[QStringLiteral("prefixPath")].toString());

    killProcess.setProcessEnvironment(env);
    killProcess.start(m_application[QStringLiteral("runnerPath")].toString()
        + QStringLiteral("/bin/wineserver"), {QStringLiteral("-k")});
    killProcess.waitForFinished(5000);
}

bool AppProcess::isRunning() const
{
    return m_process.state() != QProcess::NotRunning;
}

QString AppProcess::getProcessName() const
{
    QStringList pathToBinary = m_application[QStringLiteral("binaryPath")].toString().split(QStringLiteral("/"));
    return pathToBinary.last();
}
