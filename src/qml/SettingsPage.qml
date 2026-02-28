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

            function saveSettings() {
                var settings = {
                    "prefixesLocation": defaultPrefixesLocationDelegate.currentFolder.toString().replace("file://", ""),
                    "alwaysCreatePrefix": alwaysCreatePrefix.checked
                }
                klauncherManager.saveDefaultSettings(settings)
            }

            Component.onCompleted: {
                var settings = JSON.parse(klauncherManager.loadDefaultSettings())
                defaultPrefixesLocationDelegate.currentFolder = settings.prefixesLocation
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
                    below: defaultPrefixesLocationDelegate
                }

                FormCard.FormFolderDelegate {
                    id: defaultPrefixesLocationDelegate
                    label: "Default prefixes location"
                    //description: i18n("Defines where Proton store configuration files")
                    onAccepted: mainPageRoot.saveSettings()
                }

                FormCard.FormSwitchDelegate {
                    id: alwaysCreatePrefix
                    text: i18n("Use default prefix by default")
                    description: i18n("Creates Default/ prefix in prefixes location and uses it for every app unless told otherwise")
                    onToggled: mainPageRoot.saveSettings()
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
