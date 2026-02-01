import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    id: window

    width: 430
    height: 610

    minimumWidth: 250
    minimumHeight: 200

    // hides unnecessary header
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    header: Kirigami.AbstractApplicationHeader {
        contentItem: RowLayout {
            anchors {
                left: parent.left
                leftMargin: Kirigami.Units.smallSpacing
                right: parent.right
                rightMargin: Kirigami.Units.smallSpacing
            }
            Kirigami.ActionToolBar {
                alignment: Qt.AlignRight
                overflowIconName: "application-menu-symbolic"

                actions: [
                    Kirigami.Action {
                        text: i18n("About")
                        icon.name: "help-about"
                        displayHint: Kirigami.DisplayHint.AlwaysHide
                        onTriggered: window.pageStack.pushDialogLayer(Qt.resolvedUrl("./AboutPage.qml"))
                    },

                    Kirigami.Action {
                        text: i18n("Quit")
                        icon.name: "application-exit-symbolic"
                        displayHint: Kirigami.DisplayHint.AlwaysHide
                        onTriggered: Qt.quit()
                    }
                ]
            }
        }
    }

    //globalDrawer: DrawerPage { wideScreen: root.width >= Kirigami.Units.gridUnit * 50 }

    pageStack {
        initialPage: mainPageComponent
        columnView.columnResizeMode: pageStack.wideMode ? Kirigami.ColumnView.DynamicColumns : Kirigami.ColumnView.SingleColumn
    }

    Component {
        id: mainPageComponent
        MainPage {}
    }
}
