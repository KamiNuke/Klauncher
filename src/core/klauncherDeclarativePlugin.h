#ifndef KLAUNCHER_KLAUNCHERDECLARATIVEPLUGIN_H
#define KLAUNCHER_KLAUNCHERDECLARATIVEPLUGIN_H

#include <QQmlExtensionPlugin>

class KlauncherDeclarativePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char* uri) override;
};


#endif //KLAUNCHER_KLAUNCHERDECLARATIVEPLUGIN_H