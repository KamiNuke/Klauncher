import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kirigamiaddons.settings as KirigamiSettings

KirigamiSettings.ConfigurationView {
    id: appPreference
    property var application: ({})
    property var defaultSettings: ({})
    property var runnerList: []


    modules: [
        KirigamiSettings.ConfigurationModule {
            moduleId: "mainSetting"
            text: i18n("Main Settings")
            page: () => mainPage
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "options"
            text: i18n("Options")
            page: () => optionsPage
        }
    ]


    readonly property Component mainPage: Component {
       FormCard.FormCardPage {
            id: mainPageRoot

            Component.onCompleted: {
                var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                defaultPrefixesLocationDelegate.currentFolder = settings.prefixesLocation
                defaultPrefixDelegate.currentFolder = settings.env.WINEPREFIX
                alwaysCreatePrefix.checked = settings.alwaysCreatePrefix
            }

            FormCard.FormCard {
                Layout.topMargin: Kirigami.Units.gridUnit

                FormCard.FormHeader {
                    id: header
                    title: i18nc("@info", "Default Settings")
                }

                FormCard.FormDelegateSeparator {
                    above: header
                    below: defaultPrefixDelegate
                }

                FormCard.FormFolderDelegate {
                    id: defaultPrefixDelegate
                    label: i18n("Default prefix")
                    currentFolder: {
                        var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                        return settings.env.WINEPREFIX
                    }
                    onAccepted: {
                        klauncherManager.saveDefaultSettings({
                            "env": { "WINEPREFIX": currentFolder.toString().replace("file://", "") }
                        })
                    }
                }

                FormCard.FormComboBoxDelegate {
                    id: runnerVersion
                    text: i18n("Proton")
                    displayMode: FormCard.FormComboBoxDelegate.ComboBox
                    model: appPreference.runnerList
                    textRole: "name"

                    currentIndex: {
                        var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                        for (let i = 0; i < appPreference.runnerList.length; ++i) {
                            if (appPreference.runnerList[i].path === settings.env.PROTONPATH)
                                return i
                        }
                        return -1
                    }

                    onActivated: idx => {
                        if (idx >= 0 && idx < appPreference.runnerList.length) {
                            klauncherManager.saveDefaultSettings({
                                "env": { "PROTONPATH": appPreference.runnerList[idx].path }
                            })
                        }
                    }
                }

                FormCard.FormFolderDelegate {
                    id: defaultPrefixesLocationDelegate
                    label: i18n("Default prefixes location")
                    currentFolder: {
                        var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                        return settings.prefixesLocation
                    }
                    onAccepted: {
                        klauncherManager.saveDefaultSettings({
                            "prefixesLocation": currentFolder.toString().replace("file://", "")
                        })
                    }
                }

                FormCard.FormSwitchDelegate {
                    id: alwaysCreatePrefix
                    text: i18n("Use default prefix by default")
                    description: i18n("DOESN'T WORK AS FOR NOW")
                    checked: {
                        var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                        return settings.alwaysCreatePrefix ?? false
                    }
                    onToggled: {
                        klauncherManager.saveDefaultSettings({
                            "alwaysCreatePrefix": checked
                        })
                    }
                }
            }

        }
    }

    readonly property Component optionsPage: Component {
        FormCard.FormCardPage {
            id: optionPageRoot
            function saveSettings() {
                var settings = {
                    "useMangoHud": mangohudSwitch.checked,
                    "useGameMode": gamemodeSwitch.checked,
                    "env": {
                        "PROTON_ENABLE_WAYLAND": waylandSwitch.checked,
                        "PROTON_DLLS_UPGRADE": dlssUpgradeSwitch.checked,
                        "PROTON_FSR4_UPGRADE": fsr4UpgradeSwitch.checked
                    }
                }
                klauncherManager.saveDefaultSettings(settings)
            }
            Component.onCompleted: {
                var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                mangohudSwitch.checked = settings.useMangoHud
                gamemodeSwitch.checked = settings.useGameMode
                waylandSwitch.checked = settings.env.PROTON_ENABLE_WAYLAND
                dlssUpgradeSwitch.checked = settings.env.PROTON_DLLS_UPGRADE
                fsr4UpgradeSwitch.checked = settings.env.PROTON_FSR4_UPGRADE
            }

            FormCard.FormCard {
                Layout.topMargin: Kirigami.Units.gridUnit

                FormCard.FormHeader {
                    id: header
                    title: i18nc("@info", "Default Settings")
                }

                FormCard.FormDelegateSeparator {
                    above: header
                    below: mangohudSwitch
                }

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
                    text: i18n("Enable FSR upgrade")
                    description: i18n("Downloads and uses newest FSR4 version")
                    onToggled: optionPageRoot.saveSettings()
                }
            }
        }
    }
}
