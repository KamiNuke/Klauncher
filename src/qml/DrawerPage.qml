import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.GlobalDrawer {
    id: drawer

    property bool wideScreen: false

    maximumSize: Kirigami.Units.gridUnit * 15

    modal: !drawer.wideScreen
    interactiveResizeEnabled: true

    header: Kirigami.AbstractApplicationHeader {
        Kirigami.Heading {
            text: i18nc("@title", "Menu")
            level: 1
            anchors.centerIn: parent
        }
    }


    footer: ColumnLayout {
        Kirigami.Separator {
            Layout.fillWidth: true
        }

        states: [
            State {
                name: "full"
                when: drawer.wideScreen
                PropertyChanges { drawer.drawerOpen: true }
            },
            State {
                name: "compact"
                when: !drawer.wideScreen
                PropertyChanges { drawer.drawerOpen: false }
            }
        ]
    }
}