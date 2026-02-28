import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import QtQuick.Dialogs
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

Kirigami.Dialog {
    id: addDialog
    title: i18nc("@title:window", "Add application")
    implicitWidth: Kirigami.Units.gridUnit * 20
    padding: Kirigami.Units.smallSpacing
    standardButtons: Kirigami.Dialog.NoButton

    property var runnerList: []

    property string appName: ""
    property string binaryPath: ""
    property string prefixPath: ""
    property bool isMoreOpen: false

    signal addAppRequested(var application);

    FileDialog {
        id: binaryPathDialog
        title: i18n("Select binary")
        nameFilters: ["Exe files (*.exe)", "Ink files (*.ink)"]
        onAccepted: {
            addDialog.binaryPath = selectedFile.toString().replace("file://", "")
        }
    }

    FolderDialog {
        id: prefixPathDialog
        title: i18n("Select prefix")
        onAccepted: {
            addDialog.prefixPath = selectedFolder.toString().replace("file://", "")
        }
    }

    customFooterActions: [
        Kirigami.Action {
            icon.name: "dialog-ok"
            text: i18n("Add")
            enabled: addDialog.appName.trim() !== "" &&
                addDialog.binaryPath.trim() !== "" &&
                addDialog.prefixPath.trim() !== ""
            onTriggered: addDialog.add()
        },
        Kirigami.Action {
            icon.name: "dialog-cancel"
            text: i18n("Cancel")
            onTriggered: addDialog.close()
        }
    ]

    function clearFields() {
        addDialog.appName = "";
        addDialog.prefixPath = "";
        addDialog.binaryPath = "";
    }

    function add() {
        let application = {
            "name": addDialog.appName,
            "binaryPath": addDialog.binaryPath,
            "env": {
                "WINEPREFIX": addDialog.prefixPath,
                "PROTONPATH": runnerComboBox.selectedRunnerPath
            }
        }
        addDialog.addAppRequested(application)
        addDialog.close()
    }

    onClosed: clearFields()

    ColumnLayout {
        spacing: Kirigami.Units.largeSpacing

        Kirigami.FormLayout {
            Layout.fillWidth: true

            Controls.TextField {
                id: nameField
                Kirigami.FormData.label: i18n("App name")
                placeholderText: "Game"
                text: addDialog.appName
                onTextChanged: addDialog.appName = text
            }

            RowLayout {
                Layout.fillWidth: true
                Kirigami.FormData.label: i18n("Path to binary")

                Controls.TextField {
                    id: binaryPathField
                    placeholderText: "/home/user/game.exe"
                    text: addDialog.binaryPath
                    onTextChanged: addDialog.binaryPath = text
                }

                Controls.Button {
                    icon.name: "folder-open"
                    onClicked: binaryPathDialog.open()
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Kirigami.FormData.label: i18n("Path to prefix")

                Controls.TextField {
                    id: prefixPathField
                    placeholderText: "/home/user/prefix"
                    text: addDialog.prefixPath
                    onTextChanged: addDialog.prefixPath = text
                }

                Controls.Button {
                    icon.name: "folder-open"
                    onClicked: prefixPathDialog.open()
                }
            }

            Kirigami.Separator {
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing
                Layout.bottomMargin: Kirigami.Units.smallSpacing
            }

            Controls.Button {
                Layout.fillWidth: true
                icon.name: addDialog.isMoreOpen ? "arrow-down" : "arrow-right"
                text: addDialog.isMoreOpen ? i18n("Hide more options") : i18n("Show more options")
                onClicked: addDialog.isMoreOpen = !addDialog.isMoreOpen
            }

            ColumnLayout {
                Kirigami.FormData.label: i18n("Wine/Proton version")
                Layout.fillWidth: true
                visible: addDialog.isMoreOpen

                Controls.ComboBox {
                    id: runnerComboBox

                    Layout.fillWidth: true
                    spacing: Kirigami.Units.smallSpacing
                    model: addDialog.runnerList
                    textRole: "name"

                    property string selectedRunnerPath: ""
                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            selectedRunnerPath = addDialog.runnerList[currentIndex].path
                        }
                    }
                }
            }
        }
    }
}