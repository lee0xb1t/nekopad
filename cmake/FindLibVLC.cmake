# cmake/FindLibVLC.cmake
# 自动下载、配置并导入 LibVLC SDK

include_guard(GLOBAL)

# ================= 用户可配置选项 =================
if(NOT DEFINED VLC_VERSION)
    set(VLC_VERSION "3.0.9.2" CACHE STRING "LibVLC version to download")
endif()

if(NOT DEFINED VLC_ARCH)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VLC_ARCH "win64")
    else()
        set(VLC_ARCH "win32")
    endif()
endif()

# 确定下载地址 (Windows 用 7z, 其他平台通常用包管理器，但这里为了统一也支持下载)
if(WIN32)
    set(VLC_FILE_NAME "vlc-${VLC_VERSION}-${VLC_ARCH}.7z")
    set(VLC_URL "https://get.videolan.org/vlc/${VLC_VERSION}/${VLC_ARCH}/${VLC_FILE_NAME}")
elseif(APPLE)
    # macOS 通常建议使用 Homebrew，但如果强制下载 SDK：
    # 注意：macOS 官方通常提供 .dmg 或 .tar.xz，这里假设使用 tar.xz (如果有) 或 dmg
    # 为了简化，本脚本主要针对 Windows 的复杂情况，macOS/Linux 推荐优先使用系统包
    message(WARNING "Auto-download for macOS is experimental. Prefer 'brew install vlc'.")
    set(VLC_FILE_NAME "vlc-${VLC_VERSION}.tar.xz") # 假设存在，实际需确认官方链接
    set(VLC_URL "https://get.videolan.org/vlc/${VLC_VERSION}/MacOSX/${VLC_FILE_NAME}")
else()
    # Linux: 优先检查系统包，如果找不到再尝试下载 (通常不推荐在 Linux 手动下载 SDK)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(VLC_SYSTEM libvlc)
        if(VLC_SYSTEM_FOUND)
            set(LibVLC_FOUND TRUE)
            set(LibVLC_INCLUDE_DIRS ${VLC_SYSTEM_INCLUDE_DIRS})
            set(LibVLC_LIBRARIES ${VLC_SYSTEM_LINK_LIBRARIES})
            # 创建导入目标
            if(NOT TARGET LibVLC::LibVLC)
                add_library(LibVLC::LibVLC INTERFACE IMPORTED)
                target_include_directories(LibVLC::LibVLC INTERFACE ${VLC_SYSTEM_INCLUDE_DIRS})
                target_link_libraries(LibVLC::LibVLC INTERFACE ${VLC_SYSTEM_LINK_LIBRARIES})
            endif()
            return() # 找到系统包，直接返回
        endif()
    endif()
    message(FATAL_ERROR "LibVLC not found via pkg-config. Please install 'libvlc-dev' (apt) or 'vlc-devel' (dnf). Auto-download on Linux is disabled by default.")
endif()

# ================= 下载与解压逻辑 (主要针对 Windows) =================
if(WIN32)
    set(DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/_deps/vlc_download")
    set(VLC_DOWNLOAD_PATH "${DOWNLOAD_DIR}/${VLC_FILE_NAME}")
    set(VLC_SDK_ROOT "${CMAKE_BINARY_DIR}/_deps/vlc_sdk")
    
    # 1. 查找 7z 工具
    find_program(SEVEN_ZIP_EXE NAMES 7z 7za)
    if(NOT SEVEN_ZIP_EXE)
        message(FATAL_ERROR "7-Zip (7z.exe) is required to extract LibVLC on Windows. Please install it and add to PATH.")
    endif()

    # 2. 下载
    if(NOT EXISTS "${VLC_DOWNLOAD_PATH}")
        message(STATUS "Downloading LibVLC ${VLC_VERSION} (${VLC_ARCH})...")
        file(DOWNLOAD 
            "${VLC_URL}" 
            "${VLC_DOWNLOAD_PATH}"
            SHOW_PROGRESS
            EXPECTED_HASH SHA256=ea53f04691829e8a90ae23a30f8f9c01a535802b2a1f3c98b444b1b68051313b
        )
    else()
        message(STATUS "LibVLC archive already downloaded.")
    endif()

    # 3. 解压
    # 预期解压后的目录结构: vlc-3.0.21/sdk/...
    set(VLC_EXTRACT_SUBDIR "vlc-${VLC_VERSION}")
    set(VLC_EXTRACT_FULL_PATH "${VLC_SDK_ROOT}/${VLC_EXTRACT_SUBDIR}")

    if(NOT EXISTS "${VLC_EXTRACT_FULL_PATH}/sdk")
        message(STATUS "Extracting LibVLC SDK...")
        file(MAKE_DIRECTORY "${VLC_SDK_ROOT}")
        execute_process(
            COMMAND ${SEVEN_ZIP_EXE} x "${VLC_DOWNLOAD_PATH}" "-o${VLC_SDK_ROOT}" -y
            RESULT_VARIABLE EXTRACT_RESULT
            OUTPUT_QUIET
        )
        if(NOT EXTRACT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to extract LibVLC SDK.")
        endif()
    endif()

    # 4. 定位路径
    set(_VLC_INCLUDE_DIR "${VLC_EXTRACT_FULL_PATH}/sdk/include")
    set(_VLC_LIB_DIR "${VLC_EXTRACT_FULL_PATH}/sdk/lib")
    set(_VLC_BIN_DIR "${VLC_EXTRACT_FULL_PATH}/")

    # 验证
    if(NOT EXISTS "${_VLC_INCLUDE_DIR}/vlc/vlc.h")
        message(FATAL_ERROR "Extraction failed: vlc.h not found in ${_VLC_INCLUDE_DIR}")
    endif()

    # 设置输出变量
    set(LibVLC_FOUND TRUE)
    set(LibVLC_INCLUDE_DIRS "${_VLC_INCLUDE_DIR}")
    set(LibVLC_LIBRARY_DIRS "${_VLC_LIB_DIR}")
    set(LibVLC_BIN_DIRS "${_VLC_BIN_DIR}")
    set(LibVLC_LIBRARIES libvlc libvlccore)
    
    # 保存路径供主 CMake 使用
    set(VLC_SDK_ROOT_PATH "${VLC_EXTRACT_FULL_PATH}" CACHE INTERNAL "Path to extracted VLC SDK")

endif()

# ================= 创建导入目标 (Imported Target) =================
if(LibVLC_FOUND AND NOT TARGET LibVLC::LibVLC)
    add_library(LibVLC::LibVLC INTERFACE IMPORTED)
    
    target_include_directories(LibVLC::LibVLC INTERFACE ${LibVLC_INCLUDE_DIRS})
    
    if(WIN32)
        target_link_directories(LibVLC::LibVLC INTERFACE ${LibVLC_LIBRARY_DIRS})
        target_link_libraries(LibVLC::LibVLC INTERFACE libvlc libvlccore)
        
        # 定义 DLL 部署属性 (方便主程序调用)
        set(LibVLC_DLLS 
            "${LibVLC_BIN_DIRS}/libvlc.dll"
            "${LibVLC_BIN_DIRS}/libvlccore.dll"
        )
        set(LibVLC_PLUGINS_DIR "${LibVLC_BIN_DIRS}/plugins")
        
    elseif(UNIX AND NOT APPLE)
        # Linux .so
        target_link_libraries(LibVLC::LibVLC INTERFACE vlc vlccore)
    endif()
endif()

# 打印状态
if(LibVLC_FOUND)
    message(STATUS "LibVLC ${VLC_VERSION} configured successfully.")
    message(STATUS "  Include: ${LibVLC_INCLUDE_DIRS}")
    if(WIN32)
        message(STATUS "  Bin Dir: ${LibVLC_BIN_DIRS} (Will be copied to output)")
    endif()
else()
    message(FATAL_ERROR "LibVLC not found and could not be auto-configured.")
endif()