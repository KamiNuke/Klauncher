configure_file(config.h.in config.h @ONLY)

add_executable(${PROJECT_NAME}
        main.cpp
        klauncher.h
        klauncher.cpp
)

ecm_add_qml_module(${PROJECT_NAME}
        URI uri.klauncher
        VERSION 0.1
)

target_sources(${PROJECT_NAME}
        PRIVATE
        main.cpp
)

ecm_target_qml_sources(${PROJECT_NAME}
        SOURCES
        qml/BaseApp.qml
        qml/MainPage.qml
        #qml/DrawerPage.qml
        qml/AboutPage.qml
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
