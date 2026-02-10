#include "klaunchermanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QDir>
#include <qcontainerfwd.h>
#include <qeventloop.h>
#include <QQmlContext>
#include <qimage.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>
#include <qurl.h>

#include "additional.h"
#include "klauncherDeclarativePlugin.h"

KlauncherManager::KlauncherManager()
    : m_engine(new QQmlApplicationEngine),
        m_processManager(new ProcessManager(this))
{
    setObjectName("klauncherManager");
    m_engine->rootContext()->setContextObject(new KLocalizedContext(m_engine.get()));

    auto uri = "uri.klauncher";
    KlauncherDeclarativePlugin* plugin = new KlauncherDeclarativePlugin;
    plugin->setParent(this);
    plugin->initializeEngine(m_engine.get(), uri);
    plugin->registerTypes(uri);

    m_engine->rootContext()->setContextProperty(QStringLiteral("klauncherManager"), this);
    m_engine->rootContext()->setContextProperty(QStringLiteral("processManager"), m_processManager);
    m_engine->loadFromModule("uri.klauncher", "Main");
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
    QString applicationDataPath = getAppListLocation();

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
    jsonObj[QStringLiteral("name")] = application.value(QStringLiteral("name")).toString();
    jsonObj[QStringLiteral("binaryPath")] = application.value(QStringLiteral("binaryPath")).toString();
    jsonObj[QStringLiteral("prefixPath")] = application.value(QStringLiteral("prefixPath")).toString();
    jsonObj[QStringLiteral("runnerPath")] = application.value(QStringLiteral("runnerPath")).toString();
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

    qDebug() << getIconsLocation();

    return QStringLiteral("Created successfully");
}

void KlauncherManager::updateApp(const QVariantMap& application)
{
    QString applicationDataPath = getAppListLocation();

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
            jsonObj[QStringLiteral("binaryPath")] = application.value(QStringLiteral("binaryPath")).toString();
            jsonObj[QStringLiteral("prefixPath")] = application.value(QStringLiteral("prefixPath")).toString();
            jsonObj[QStringLiteral("runnerPath")] = application.value(QStringLiteral("runnerPath")).toString();
            jsonObj[QStringLiteral("iconPath")] = application.value(QStringLiteral("iconPath")).toString();

            if (application.contains(QStringLiteral("useMangoHud")))
                jsonObj[QStringLiteral("useMangoHud")] = application.value(QStringLiteral("useMangoHud")).toBool();
            if (application.contains(QStringLiteral("useGameMode")))
                jsonObj[QStringLiteral("useGameMode")] = application.value(QStringLiteral("useGameMode")).toBool();
            if (application.contains(QStringLiteral("useWayland")))
                jsonObj[QStringLiteral("useWayland")] = application.value(QStringLiteral("useWayland")).toBool();
            if (application.contains(QStringLiteral("useDLSSUpgrade")))
                jsonObj[QStringLiteral("useDLSSUpgrade")] = application.value(QStringLiteral("useDLSSUpgrade")).toBool();
            if (application.contains(QStringLiteral("useFSR4Upgrade")))
                jsonObj[QStringLiteral("useFSR4Upgrade")] = application.value(QStringLiteral("useFSR4Upgrade")).toBool();


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
    QString applicationDataPath = getAppListLocation();

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

    removeIcon(application.value(QStringLiteral("iconPath")).toString());
}

QString KlauncherManager::loadApps()
{
    QString applicationDataPath = getAppListLocation();

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

    QString iconOutputPath = getIconsLocation() + QStringLiteral("/") + binaryFileName + QStringLiteral(".ico");

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
        getHomeDirectory() + QStringLiteral("/.local/share/Steam/compatibilitytools.d/"),
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
    return loadDefSettings();
}

void KlauncherManager::saveDefaultSettings(const QVariantMap& settings)
{
    QString settingsPath = getSettingsLocation();

    QJsonObject jsonObj;
    QFile file(settingsPath);
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll()) ;
        file.close();
        if (jsonDoc.isObject())
            jsonObj = jsonDoc.object();
    }

    for (QVariantMap::const_iterator it = settings.begin(); it != settings.end(); ++it)
    {
        jsonObj[it.key()] = QJsonValue::fromVariant(it.value());
    }

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(jsonObj).toJson(QJsonDocument::Indented));
        file.close();
    }
}

QVariantMap KlauncherManager::getEffectiveSettings(const QVariantMap& application)
{
    return getSettings(application);
}
