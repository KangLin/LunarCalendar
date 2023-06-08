cmake_minimum_required(VERSION 3.21)

project(LunarCalendar)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")

SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    MARK_AS_ADVANCED(WIN32_USE_MP)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF(MSVC)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
#需要的QT组件
SET(QT_COMPONENTS ${QT_COMPONENTS} Core)
if(QT_VERSION_MAJOR EQUAL 5 AND ANDROID)
    LIST(APPEND QT_COMPONENTS AndroidExtras)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 5.10.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 5.10.0")
endif()
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        SET(QT_LIBRARIES ${QT_LIBRARIES} Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckCCompilerFlag)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(CMakeDependentOption)
include(cmake/DownloadFile.cmake)

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
    if(NOT EXISTS ${RabbitCommon_DIR}/Src)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../../../RabbitCommon)
    endif()
endif()
if(RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    message("Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message("RabbitCommon_DIR is not found. Please use one of the following ways to set it:")
        message("1. Set RabbitCommon_DIR to the install prefix of RabbitCommon.")
        message("2. Set RabbitCommon_DIR to source code root of RabbitCommon.")
        message("2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon")
        message("   ag:")
        message("       git clone https://github.com/KangLin/RabbitCommon.git")
        message("2.2 Then set cmake variable or environment variable RabbitCommon_DIR to download root directory.")
        message("   ag:")
        message("       cmake -DRabbitCommon_DIR= ")
        message(FATAL_ERROR "RabbitCommon_DIR isn't set.")
    endif()
endif()

GET_VERSION(OUT_VERSION LunarCalendar_VERSION
    OUT_REVISION LunarCalendar_REVISION)
if(NOT LunarCalendar_VERSION)
    SET(LunarCalendar_VERSION "v0.2.5")
endif()
message("LunarCalendar_VERSION:${LunarCalendar_VERSION}")

add_subdirectory(Src)
option(BUILD_APP "Set to ON to build applaction" ON)
if(BUILD_APP)
    add_subdirectory(App)
endif(BUILD_APP)

SET(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    )
if(ANDROID)
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets" COMPONENT Runtime)
else()
    INSTALL(FILES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)
endif()

if(WIN32)
    INSTALL(FILES Install/Install.nsi
        ${CMAKE_SOURCE_DIR}/App/AppIcon.ico
        ${OTHER_FILES}
        DESTINATION "${CMAKE_BINARY_DIR}" COMPONENT Runtime)
endif(WIN32)

message("Build arch:${BUILD_ARCH}")
message("Build app:${BUILD_APP}")
message("Build shared library:${BUILD_SHARED_LIBS}")
