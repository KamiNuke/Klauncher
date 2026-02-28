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
            moduleId: "options"
            text: i18n("Options")
            page: () => optionsPage
        }
    ]

    readonly property Component appInfo: Component {
        FormCard.FormCardPage {
            FormCard.FormCard {
                Layout.topMargin: Kirigami.Units.gridUnit

                FormCard.FormHeader {
                    id: header
                    title: i18nc("@info", "App Info")
                }

                FormCard.FormDelegateSeparator {
                    above: header
                    below: iconButton
                }

                FormCard.FormIconDelegate {
                    id: iconButton
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
                    onAccepted: {
                        appPreference.application.binaryPath = selectedFile.toString().replace("file://", "")
                        changeRequested(appPreference.application)
                    }
                }

                FormCard.FormFolderDelegate {
                    label: "Path to prefix"
                    currentFolder: appPreference.application.env.WINEPREFIX
                    onAccepted: {
                        appPreference.application.env.WINEPREFIX = currentFolder.toString().replace("file://", "")
                        changeRequested(appPreference.application)
                    }
                }

                FormCard.FormComboBoxDelegate {
                    id: runnerVersion

                    text: i18n("Proton")
                    displayMode: FormCard.FormComboBoxDelegate.ComboBox
                    model: appPreference.runnerList
                    textRole: "name"

                    currentIndex: {
                        for (let i = 0; i < appPreference.runnerList.length; ++i) {
                            if(appPreference.runnerList[i].path === appPreference.application.env.PROTONPATH) {
                                return i;
                            }
                        }
                        return -1;
                    }

                    onActivated: idx => {
                        if (idx >= 0 && idx < appPreference.runnerList.length) {
                            if (!appPreference.application.env)
                                appPreference.application.env = {}
                            appPreference.application.env.PROTONPATH = appPreference.runnerList[idx].path
                            changeRequested(appPreference.application)
                        }
                    }
                }
            }
        }
    }

    readonly property Component optionsPage: Component {
        FormCard.FormCardPage {
            id: optionPageRoot

            function saveSettings() {
                appPreference.application.useMangoHud = mangohudSwitch.checked
                appPreference.application.useGameMode = gamemodeSwitch.checked
                if (!appPreference.application.env)
                    appPreference.application.env = {}
                appPreference.application.env.PROTON_ENABLE_WAYLAND = waylandSwitch.checked ? 1 : 0
                appPreference.application.env.PROTON_DLLS_UPGRADE = dlssUpgradeSwitch.checked ? 1 : 0
                appPreference.application.env.PROTON_FSR4_UPGRADE = fsr4UpgradeSwitch.checked ? 1 : 0
                changeRequested(appPreference.application)
            }

            Component.onCompleted: {
                var defaults = JSON.parse(klauncherManager.loadDefaultSettings())
                var env = appPreference.application.env ?? {}
                var defEnv = defaults.env ?? {}

                mangohudSwitch.checked = appPreference.application.useMangoHud !== undefined
                    ? appPreference.application.useMangoHud
                    : defaults.useMangoHud

                gamemodeSwitch.checked = appPreference.application.useGameMode !== undefined
                    ? appPreference.application.useGameMode
                    : defaults.useGameMode

                waylandSwitch.checked = env.PROTON_ENABLE_WAYLAND !== undefined
                    ? env.PROTON_ENABLE_WAYLAND === 1
                    : defEnv.PROTON_ENABLE_WAYLAND === 1

                dlssUpgradeSwitch.checked = env.PROTON_DLLS_UPGRADE !== undefined
                    ? env.PROTON_DLLS_UPGRADE === 1
                    : defEnv.PROTON_DLLS_UPGRADE === 1

                fsr4UpgradeSwitch.checked = env.PROTON_FSR4_UPGRADE !== undefined
                    ? env.PROTON_FSR4_UPGRADE === 1
                    : defEnv.PROTON_FSR4_UPGRADE === 1
            }


            FormCard.FormCard {
                Layout.topMargin: Kirigami.Units.gridUnit

                FormCard.FormSwitchDelegate {
                    id: mangohudSwitch
                    text: i18n("Enable MangoHud")
                    description: i18n("Shows an overlay for FPS, GPU/CPU usage and more")
                    onToggled: optionPageRoot.saveSettings()
                }

                FormCard.FormSwitchDelegate {
                    id: gamemodeSwitch
                    text: i18n("Enable GameMode")
                    description: i18n("Tweak for increasing performance (increases battery drain)")
                    onToggled: optionPageRoot.saveSettings()
                }

                FormCard.FormSwitchDelegate {
                    id: waylandSwitch
                    text: i18n("Enable wayland (unstable)")
                    description: i18n("Decreases latency(good), may cause issue in fullscreen apps")
                    onToggled: optionPageRoot.saveSettings()
                }

                FormCard.FormSwitchDelegate {
                    id: dlssUpgradeSwitch
                    text: i18n("Enable DLSS upgrade")
                    description: i18n("Downloads and uses newest DLSS version")
                    onToggled: optionPageRoot.saveSettings()
                }

                FormCard.FormSwitchDelegate {
                    id: fsr4UpgradeSwitch
                    text: i18n("Enable FSR4 upgrade")
                    description: i18n("Downloads and uses newest FSR4 version")
                    onToggled: optionPageRoot.saveSettings()
                }
            }
        }
    }
}
