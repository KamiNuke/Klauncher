#include "klaunchermanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QDir>
#include <QtContainerFwd>
#include <QEventLoop>
#include <QQmlContext>
#include <QImage>
#include <QJsonDocument>
#include <QJsonValue>

#include "Dir.h"
#include "File.h"


KlauncherManager::KlauncherManager(QAnyStringView typeName)
    : m_engine(new QQmlApplicationEngine)
{
    setObjectName("klauncherManager");
    m_engine->rootContext()->setContextObject(new KLocalizedContext(m_engine.get()));

    m_engine->rootContext()->setContextProperty(QStringLiteral("klauncherManager"), this);
    m_engine->loadFromModule("uri.klauncher", typeName);
}

KlauncherManager::~KlauncherManager()
{
#ifndef NDEBUG
    qInfo() << Q_FUNC_INFO;
#endif

    m_engine->deleteLater();
}

QString KlauncherManager::addApp(const QVariantMap& application)
{
    QString applicationDataPath = Klauncher::File::getApplicationListFile();

    QJsonArray jsonArr;
    QFile file(applicationDataPath);
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QTextStream fileText(&file);
        const QString jsonString = fileText.readAll();
        file.close();
        QByteArray jsonBytes = jsonString.toLocal8Bit();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
        if (jsonDoc.isArray())
        {
            jsonArr = jsonDoc.array();
        }
    }

    const auto it = std::ranges::find_if(jsonArr, [&application](const QJsonValue& value)
    {
        if (value.isObject())
        {
            return value.toObject().value(QStringLiteral("name")).toString() == application.value(QStringLiteral("name")).toString();
        }
        return false;
    });

    if (it != jsonArr.end())
    {
        return QStringLiteral("Value already exist");
    }

    QJsonObject jsonObj;
    for (QVariantMap::const_iterator it = application.begin(); it != application.end(); ++it)
    {
        jsonObj[it.key()] = QJsonValue::fromVariant(it.value());
    }
    jsonObj[QStringLiteral("iconPath")] = extractIcon(application.value(QStringLiteral("binaryPath")).toString());
    jsonArr.append(jsonObj);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << QStringLiteral("Failed to save file ") + applicationDataPath;
        return QStringLiteral("Failed to save file");;
    }

    QJsonDocument jsonDoc(jsonArr);
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << Klauncher::Dir::getIconsLocation();

    return QStringLiteral("Created successfully");
}

void KlauncherManager::updateApp(const QVariantMap& application)
{
    QString applicationDataPath = Klauncher::File::getApplicationListFile();

    QJsonArray jsonArr;
    QFile file(applicationDataPath);
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QTextStream fileText(&file);
        QString jsonString = fileText.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toLocal8Bit());
        if (jsonDoc.isArray())
        {
            jsonArr = jsonDoc.array();
        }
    }

    for (int i = 0; i < jsonArr.size(); ++i)
    {
        QJsonObject jsonObj = jsonArr[i].toObject();
        if (jsonObj.value(QStringLiteral("name")).toString() == application.value(QStringLiteral("name")).toString())
        {

            for (QVariantMap::const_iterator it = application.begin(); it != application.end(); ++it)
            {
                jsonObj[it.key()] = QJsonValue::fromVariant(it.value());
            }

            jsonArr[i] = jsonObj;
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument jsonDoc(jsonArr);
        file.write(jsonDoc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void KlauncherManager::removeApp(const QVariantMap& application)
{
    QString applicationDataPath = Klauncher::File::getApplicationListFile();

    QJsonArray jsonArr;
    QFile file(applicationDataPath);
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QTextStream fileText(&file);
        const QString jsonString = fileText.readAll();
        file.close();
        QByteArray jsonBytes = jsonString.toLocal8Bit();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
        if (jsonDoc.isArray())
        {
            jsonArr = jsonDoc.array();
        }
    }

    const auto it = std::ranges::find_if(jsonArr, [&application](const QJsonValue& value)
    {
        if (value.isObject())
        {
            return value.toObject().value(QStringLiteral("name")).toString() == application.value(QStringLiteral("name")).toString();
        }
        return false;
    });

    if (it != jsonArr.end())
    {
        jsonArr.erase(it);
    }

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << QStringLiteral("Failed to save file ") + applicationDataPath;
        return;
    }

    QJsonDocument jsonDoc(jsonArr);
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    Klauncher::File::removeIcon(application.value(QStringLiteral("iconPath")).toString());
}

QString KlauncherManager::loadApps()
{
    QString applicationDataPath = Klauncher::File::getApplicationListFile();

    QFile file(applicationDataPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << QStringLiteral("Failed to open ") + applicationDataPath;
        return {};
    }

    QTextStream fileText(&file);
    QString jsonString = fileText.readAll();
    file.close();
    QByteArray jsonBytes = jsonString.toLocal8Bit();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull())
    {
        qDebug() << QStringLiteral("Failed to create JSON doc");
        return {};
    }
    if (!jsonDoc.isArray())
    {
        qDebug() << QStringLiteral("JSON is not an array");
        return {};
    }

    return jsonString;
}

QString KlauncherManager::extractIcon(const QString& binaryPath)
{
    QProcess process;
    const QString program = QStringLiteral("/usr/bin/wrestool");

    QFileInfo fileInfo(binaryPath);
    QString binaryFileName = fileInfo.completeBaseName();

    QString iconOutputPath = Klauncher::Dir::getIconsLocation() + QStringLiteral("/") + binaryFileName + QStringLiteral(".ico");

    QStringList arguments;
    arguments << QStringLiteral("-x")
              << QStringLiteral("-t")
              << QStringLiteral("14")
              << binaryPath
              << QStringLiteral("-o")
              << iconOutputPath;

    process.setProgram(program);
    process.setArguments(arguments);

    QEventLoop loop;
    QObject::connect(&process,
        QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        &loop, &QEventLoop::quit);

    process.start();
    loop.exec();
    return iconOutputPath;
}

QString KlauncherManager::getRunners()
{
    QJsonArray runnersArray;

    QStringList searchPaths = {
        QStringLiteral("/usr/share/steam/compatibilitytools.d/"),
        Klauncher::Dir::getHomeDirectory() + QStringLiteral("/.local/share/Steam/compatibilitytools.d/"),
    };

    for (const auto& path : searchPaths)
    {
        QDir dir(path);
        if (!dir.exists())
            continue;

        QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const auto& subdir : subdirs)
        {
            QString compatPath = dir.absoluteFilePath(subdir + QStringLiteral("/compatibilitytool.vdf"));
            QFile file(compatPath);
            if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
                continue;

            QTextStream fileText(&file);
            QString vdfString = fileText.readAll();
            file.close();

            QRegularExpression re(QStringLiteral("\"display_name\"\\s+\"([^\"]+)\""));
            QRegularExpressionMatch match = re.match(vdfString);
            if (match.hasMatch())
            {
                QJsonObject runner;
                runner[QStringLiteral("name")] = match.captured(1);
                runner[QStringLiteral("path")] = dir.absoluteFilePath(subdir);
                runnersArray.append(runner);
            }

        }
    }

    QJsonDocument jsonDoc(runnersArray);
    return QString::fromUtf8(jsonDoc.toJson());
}

QString KlauncherManager::loadDefaultSettings()
{
    const QVariantMap settings = Klauncher::File::loadDefaultSettings();
    return QString::fromUtf8(QJsonDocument(QJsonObject::fromVariantMap(settings)).toJson());
}

void KlauncherManager::saveDefaultSettings(const QVariantMap& settings)
{
    QVariantMap current = Klauncher::File::loadDefaultSettings();

    for (auto it = settings.constBegin(); it != settings.constEnd(); ++it)
    {
        if (it.key() == QStringLiteral("env"))
        {
            QVariantMap currentEnv = current[QStringLiteral("env")].toMap();
            const QVariantMap newEnv = it.value().toMap();

            for (auto envIt = newEnv.constBegin(); envIt != newEnv.constEnd(); ++envIt)
                currentEnv[envIt.key()] = envIt.value();

            current[QStringLiteral("env")] = currentEnv;
        }
        else
        {
            current[it.key()] = it.value();
        }
    }

    Klauncher::File::saveJson(current, Klauncher::File::getDefaultSettingsFile());

}

QVariantMap KlauncherManager::getEffectiveSettings(const QVariantMap& application)
{
    return Klauncher::File::getSettings(application);
}

void KlauncherManager::startApp(const QVariantMap& application)
{
    const QString name = application[QStringLiteral("name")].toString();

    if (m_processes.contains(name))
        return;

    Klauncher::Process* process = new Klauncher::Process(this, application);

    connect(process, &Klauncher::Process::finished, this,
        [this, name, process](int exitCode, QProcess::ExitStatus exitStatus)
        {
            m_processes.remove(name);
            Q_EMIT appStopped(name);
            process->deleteLater();
        });

    connect(process, &Klauncher::Process::errorOccurred, this,
        [this, name, process](QProcess::ProcessError error)
        {
            m_processes.remove(name);
            Q_EMIT appStopped(name);
            process->deleteLater();
        });

    m_processes.insert(name, process);
    process->start();
    Q_EMIT appStarted(name);
}

void KlauncherManager::stopApp(const QVariantMap& application)
{
    const QString name = application[QStringLiteral("name")].toString();

    if (!m_processes.contains(name))
        return;

    m_processes[name]->stop();
}

bool KlauncherManager::isAppRunning(const QString& name)
{
    return m_processes.contains(name);
}
