cmake_minimum_required(VERSION 3.19)
project(LosungenQt LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)
set(app_icon_resource_windows "win_resource.rc")

qt_add_executable(LosungenQt
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    csv_parser.h csv_parser.cpp
    resource.qrc
    win_resource.rc
)

target_link_libraries(LosungenQt
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)


install(TARGETS LosungenQt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LosungenQt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
