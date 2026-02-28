import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Kirigami.AbstractCard {
    id: card

    property var application: ({})

    signal startRequested(var application)
    signal stopRequested(var application)
    signal settingsRequested(var application)
    signal removeRequested(var application)

    contentItem: Item {
        implicitHeight: delegateLayout.implicitHeight

        RowLayout {
            id: delegateLayout
            anchors.fill: parent
            spacing: Kirigami.Units.smallSpacing

            Kirigami.Icon {
                source: card.application.iconPath || "applications-graphics"
                Layout.fillHeight: true
                Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                Layout.preferredWidth: height
            }

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                Layout.maximumWidth: delegateLayout.width
                text: card.application.name
                elide: Text.ElideRight
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignRight|Qt.AlignVCenter

                Controls.Button {
                    id: runButton
                    property bool isRunning: klauncherManager.isAppRunning(card.application.name)
                    text: isRunning ? i18n("Stop") : i18n("Start")
                    onClicked: {
                        if(!isRunning) {
                            card.startRequested(card.application)
                        } else {
                            card.stopRequested(card.application)
                        }
                    }

                    Connections {
                        target: klauncherManager
                        function onAppStarted(name) {
                            if (name === card.application.name)
                                runButton.isRunning = true
                        }
                        function onAppStopped(name) {
                            if (name === card.application.name)
                                runButton.isRunning = false
                        }
                    }
                }

                Controls.Button {
                    text: i18n("Settings")
                    onClicked: card.settingsRequested(card.application)
                }

                Controls.Button {
                    text: i18n("Remove")
                    onClicked: card.removeRequested(card.application)
                }
            }
        }
    }
}