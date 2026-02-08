import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kirigamiaddons.settings as KirigamiSettings

KirigamiSettings.ConfigurationView {
    id: appPreference
    property var application: ({})
    property var runnerList: []

    signal changeRequested(var application)

    modules: [
        KirigamiSettings.ConfigurationModule {
            moduleId: "appInfo"
            text: i18n("App info")
            page: () => appInfo
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "toolsInfo"
            text: i18n("Tools")
            page: () => toolsInfo
        }
    ]

    readonly property Component appInfo: Component {
        FormCard.FormCardPage {
            FormCard.FormCard {
                Layout.topMargin: Kirigami.Units.gridUnit

                FormCard.FormHeader {
                    title: i18nc("@info", "App Info")
                }

                FormCard.FormIconDelegate {
                    text: i18nc("@label", "Active icon")
                    iconName: application.iconPath

                    onIconNameChanged: {
                        appPreference.application.iconPath = iconName
                        changeRequested(appPreference.application)
                    }

                }

                FormCard.FormTextFieldDelegate {
                    label: "Title"
                    text: application.name
                }

                FormCard.FormFileDelegate {
                    label: "Path to binary"
                    currentFolder: appPreference.application.binaryPath
                }

                FormCard.FormFolderDelegate {
                    label: "Path to prefix"
                    currentFolder: appPreference.application.prefixPath
                }

                FormCard.FormComboBoxDelegate {
                    id: runnerVersion

                    text: i18n("Proton")
                    displayMode: FormCard.FormComboBoxDelegate.ComboBox
                    model: appPreference.runnerList
                    textRole: "name"

                    currentIndex: {
                        for (let i = 0; i < appPreference.runnerList.length; ++i) {
                            if(appPreference.runnerList[i].path === appPreference.application.runnerPath) {
                                return i;
                            }
                        }
                        return -1;
                    }

                    property string selectedRunnerPath: ""
                    onActivated: idx => {
                        if (idx >= 0 && idx < appPreference.runnerList.length) {
                            selectedRunnerPath = appPreference.runnerList[idx].path
                            appPreference.application.runnerPath = selectedRunnerPath
                            changeRequested(appPreference.application)
                        }
                    }
                }
            }
        }
    }

    readonly property Component toolsInfo: Component {
        FormCard.FormCardPage {

        }
    }
}