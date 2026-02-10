import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import uri.klauncher

Kirigami.ApplicationWindow {
    id: root

    width: 430
    height: 610

    minimumWidth: 250
    minimumHeight: 200

    function refresh() {
        var appResult = klauncherManager.loadApps();
        applicationPage.appList = JSON.parse(appResult);

        var runnersResult = JSON.parse(klauncherManager.getRunners());
        addDialog.runnerList = runnersResult
        appSettingsPage.runnerList = runnersResult
    }

    SettingsPage {
        id: settingsPage
        window: root
    }

    AppSettingsPage {
        id: appSettingsPage
        window: root

        onChangeRequested: function (application) {
            klauncherManager.updateApp(application)
            refresh()
        }
    }

    AboutPage {
        id: aboutPage
        visible: false
    }

    globalDrawer: DrawerPage {
        onAddRequested: {
            refresh()
            addDialog.open()
        }
        onAboutRequested: {
            if (root.pageStack.layers.currentItem !== aboutPage) {
                root.pageStack.layers.push(aboutPage)
            }
        }
        onOpenSettings: {
            settingsPage.open()
        }
    }

    AddAppDialog {
        id: addDialog
        onAddAppRequested: function (application) {
            showPassiveNotification(klauncherManager.addApp(application))
            refresh();
        }
    }

    pageStack.initialPage: ApplicationPage {
        id: applicationPage
        onInitialLoadRequested: {
            refresh()
        }
        onAddRequested: {
            addDialog.open()
        }
        onStartRequested : function (application) {
            processManager.start(application)
        }
        onStopRequested : function (application) {
            processManager.stop(application)
        }
        onSettingsRequested : function (application) {
            appSettingsPage.application = application
            appSettingsPage.open()
            refresh();
        }
        onRemoveRequested : function (application) {
           klauncherManager.removeApp(application)
           refresh()
        }
    }
}
