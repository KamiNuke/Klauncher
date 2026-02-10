import QtQuick
import org.kde.kirigami as Kirigami

Kirigami.GlobalDrawer {
    id: drawer
    isMenu: true

    signal addRequested()
    signal aboutRequested()
    signal openSettings()

    actions: [
        Kirigami.Action {
            text: i18n("Add application")
            icon.name: "list-add-symbolic"
            onTriggered: drawer.addRequested()
        },

        Kirigami.Action {
            text: i18n("Settings")
            icon.name: "settings-configure"
            onTriggered: drawer.openSettings()
        },

        Kirigami.Action {
            text: i18n("About Klauncher")
            icon.name: "help-about"
            onTriggered: drawer.aboutRequested()
        },

        Kirigami.Action {
            text: i18n("Quit")
            icon.name: "application-exit-symbolic"
            onTriggered: Qt.quit()
        }
    ]
}