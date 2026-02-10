configure_file(config.h.in config.h @ONLY)

add_executable(${PROJECT_NAME}
        main.cpp
        core/klaunchermanager.h
        core/klaunchermanager.cpp
        core/additional.h
        core/additional.cpp
        core/klauncherDeclarativePlugin.cpp
        core/klauncherDeclarativePlugin.h
        core/processmanager.cpp
        core/processmanager.h
        core/appprocess.cpp
        core/appprocess.h
)

ecm_add_qml_module(${PROJECT_NAME}
        URI uri.klauncher
        VERSION ${PROJECT_VERSION}
)

target_sources(${PROJECT_NAME}
        PRIVATE
        main.cpp
)

ecm_target_qml_sources(${PROJECT_NAME}
        SOURCES
        qml/Main.qml
        qml/DrawerPage.qml
        qml/AboutPage.qml
        qml/ApplicationPage.qml
        qml/AddAppDialog.qml
        qml/AppCard.qml
        qml/AppSettingsPage.qml
        qml/SettingsPage.qml
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::Gui
        Qt6::QuickControls2
        Qt6::Widgets
        KF6::I18n
        KF6::CoreAddons
        KF6::IconThemes
)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
