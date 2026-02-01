cmake_minimum_required(VERSION 4.1)
project(klauncher)

set(APPLICATION_NAME "klauncher")
set(APPLICATION_DOMAIN "klauncher")
set(COMPONENT_NAME "klauncher")
set(ORGANIZATION_NAME "KamiNuke")
set(ORGANIZATION_DOMAIN "com.github.kaminuke")

# to make lsp work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT6_MIN_VERSION 6.5.0)
set(KF6_MIN_VERSION 6.22.0)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS
        Core
        Quick
        Test
        Gui
        QuickControls2
        Widgets
)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
        Kirigami
        CoreAddons
        I18n
        QQC2DesktopStyle
        IconThemes
)

find_package(KF6KirigamiAddons ${KIRIGAMI_ADDONS_MIN_VERSION} REQUIRED)
set_package_properties(KF6KirigamiAddons PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components and runtime dependency"
)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_subdirectory(src)

install(PROGRAMS klauncher.desktop DESTINATION ${KDE_INSTALL_APPDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
