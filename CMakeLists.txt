cmake_minimum_required(VERSION 3.16)

project(nekopad VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LibVLC REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appnekopad
    main.cpp
)

qt_add_qml_module(appnekopad
    URI nekopad
    QML_FILES
        Main.qml
        components/Logo.qml
        components/Fav.qml
        components/FavList.qml
        components/User.qml
        components/Head.qml
        components/VList.qml
        components/VItem.qml
    SOURCES appmgr.h appmgr.cpp
    SOURCES loginmgr.h loginmgr.cpp
    SOURCES cookiemgr.h cookiemgr.cpp
    SOURCES qrcodegen.hpp qrcodegen.cpp
    SOURCES usermgr.h usermgr.cpp
    SOURCES favmgr.h favmgr.cpp
    SOURCES models/favitem.h models/favitem.cpp
    SOURCES videolistmodel.h videolistmodel.cpp
    SOURCES models/videoitem.h models/videoitem.cpp
    SOURCES nplayer.h nplayer.cpp
    SOURCES videoinfo.h videoinfo.cpp
    SOURCES biliwbi.h biliwbi.cpp
    SOURCES nammgr.h nammgr.cpp
    SOURCES requesttag.h requesttag.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appnekopad PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appnekopad
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appnekopad
    PRIVATE Qt6::Quick Qt6::Network LibVLC::LibVLC
)

if(WIN32 AND LibVLC_FOUND)
    message(STATUS "Configuring VLC deployment for target 'appnekopad'...")

    # Copy DLLs
    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LibVLC_DLLS}
        $<TARGET_FILE_DIR:appnekopad>
        COMMENT "Copying LibVLC DLLs..."
    )

    # Copy plugins
    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LibVLC_PLUGINS_DIR}
        "$<TARGET_FILE_DIR:appnekopad>/plugins"
        COMMENT "Copying LibVLC plugins..."
    )

    # Copy lua (Optional)
    if(EXISTS "${LibVLC_BIN_DIRS}/lua")
        add_custom_command(TARGET appnekopad POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${LibVLC_BIN_DIRS}/lua"
            "$<TARGET_FILE_DIR:appnekopad>/lua"
            COMMENT "Copying LibVLC lua..."
        )
    endif()
endif()

install(TARGETS appnekopad
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)