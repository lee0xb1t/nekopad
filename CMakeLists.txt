cmake_minimum_required(VERSION 3.16)

project(nekopad VERSION 1.0 LANGUAGES CXX)

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
        components/PlayControls.qml
        components/NProgressBar.qml
        components/PlaySetting.qml
        components/NToolTip.qml

    RESOURCES resources.qrc

    SOURCES appicon.rc

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
    SOURCES models/playurlinfo.h models/playurlinfo.cpp
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


### -----------libvlc-------------

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
endif()


### -----------deploy-------------

if(WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")

    set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/deploy")

    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:appnekopad> ${DEPLOY_DIR}
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --dir ${DEPLOY_DIR}
                --release
                --no-translations
                --no-compiler-runtime
                --qmldir ${CMAKE_SOURCE_DIR}
                $<TARGET_FILE:appnekopad>
        COMMENT "Deploying Qt dependencies..."
    )


    # Mingw runtime
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    message(${MINGW_BIN_DIR})

    # 需要复制的 MinGW 运行时 DLL
    set(MINGW_RUNTIME_DLLS
        "${MINGW_BIN_DIR}/libstdc++-6.dll"
        "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
    )

    # 复制到 deploy 目录
    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_RUNTIME_DLLS}
            ${DEPLOY_DIR}
        COMMENT "Copying MinGW runtime DLLs..."
    )

    ##------
    # Copy LibVLC DLLs
    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LibVLC_DLLS}
        ${DEPLOY_DIR}
        COMMENT "Copying LibVLC DLLs..."
    )

    # Copy LibVLC plugins
    add_custom_command(TARGET appnekopad POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LibVLC_PLUGINS_DIR}
        "${DEPLOY_DIR}/plugins"
        COMMENT "Copying LibVLC plugins..."
    )


    ##------
    file(GLOB_RECURSE TEMP_FILES
        "${CMAKE_BINARY_DIR}/deploy/qml/*.qml"
        "${CMAKE_BINARY_DIR}/deploy/qml/*.png"
        "${CMAKE_BINARY_DIR}/deploy/opengl32sw.dll"
    )

    if(TEMP_FILES)
        file(REMOVE ${TEMP_FILES})
        message(STATUS "Cleaned ${TEMP_FILES} temporary files from build directory")
    endif()
endif()

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    "${CMAKE_BINARY_DIR}/deploy"
)


### -----------Inno Setup-------------

if(WIN32)
    if(DEFINED ENV{ProgramW6432})
        set(PROGRAMFILES_X86 "$ENV{ProgramW6432}")
    else()
        set(PROGRAMFILES_X86 "$ENV{ProgramFiles}")
    endif()

    # find ISCC
    find_program(INNO_SETUP_COMPILER ISCC
        PATHS
            "$ENV{LOCALAPPDATA}/Programs/Inno Setup 6"
            "$ENV{PROGRAMFILES}/Inno Setup 6"
            "${PROGRAMFILES_X86}/Inno Setup 6"
        DOC "Inno Setup compiler"
    )

    if(INNO_SETUP_COMPILER)
        message(STATUS "Inno Setup found: ${INNO_SETUP_COMPILER}")

        set(INSTALLER_OUTPUT "${CMAKE_BINARY_DIR}/NEKOPAD_Setup_${PROJECT_VERSION}.exe")

        configure_file(
            ${CMAKE_SOURCE_DIR}/nekopad.iss.in
            ${CMAKE_BINARY_DIR}/nekopad.iss
            @ONLY
        )

        add_custom_command(TARGET appnekopad POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/resources/logo_64x64.ico
            ${CMAKE_BINARY_DIR}
            COMMENT "Copying logo icon..."
        )

        add_custom_target(installer
            COMMAND ${INNO_SETUP_COMPILER}
                    /O"${CMAKE_BINARY_DIR}"
                    /F"NEKOPAD_Setup_${PROJECT_VERSION}"
                    "${CMAKE_BINARY_DIR}/nekopad.iss"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Building Inno Setup installer..."
            DEPENDS appnekopad
        )

        # add_dependencies(installer appnekopad)
    else()
        message(WARNING "Inno Setup not found. Installer target disabled.")
    endif()
endif()