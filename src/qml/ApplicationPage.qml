import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: appListPage
    title: i18nc("@title:window", "Klauncher")

    property var appList: []

    signal initialLoadRequested
    signal addRequested()
    signal startRequested(var application)
    signal stopRequested(var application)
    signal settingsRequested(var application)
    signal removeRequested(var application);

    Kirigami.CardsListView {
        id: cardsView
        model: appListPage.appList
        delegate: AppCard {
            application: modelData
            onStartRequested : function (application) {
                appListPage.startRequested(application)
            }
            onStopRequested : function (application) {
                appListPage.stopRequested(application)
            }
            onSettingsRequested : function (application) {
                appListPage.settingsRequested(application)
            }
            onRemoveRequested : function (application) {
                appListPage.removeRequested(application);
            }
        }
    }

    actions: [
        Kirigami.Action {
            id: addApp
            icon.name: "list-add-symbolic"
            text: i18nc("@action:button", "Add App")
            onTriggered: appListPage.addRequested()
        }
    ]

    Component.onCompleted: appListPage.initialLoadRequested()
}
