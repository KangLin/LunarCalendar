project(LunarCalendar LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")

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
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/RabbitCommon)
        add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
    endif()
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
    SET(LunarCalendar_VERSION "1.0.4")
endif()
message("LunarCalendar_VERSION:${LunarCalendar_VERSION}")
message("LunarCalendar_REVISION:${LunarCalendar_REVISION}")

if(LunarCalendar_VERSION)
    list(APPEND LunarCalendar_DEFINITIONS
        LunarCalendar_VERSION="${LunarCalendar_VERSION}"
        )
endif()
if(LunarCalendar_REVISION)
    list(APPEND LunarCalendar_DEFINITIONS
        LunarCalendar_REVISION="${LunarCalendar_REVISION}")
endif()

set(QT_COMPONENTS )
set(LunarCalendar_INSTALL_HEAD_FILES
    LunarCalendar.h
    )
set(LunarCalendar_HEAD_FILES
    ${LunarCalendar_INSTALL_HEAD_FILES}
    LunarCalendarModel.h
    CalendarLunar.h
    ../3th_lib/sxtwl/src/sxtwl.h
    ../3th_lib/sxtwl/src/eph.h
    ../3th_lib/sxtwl/src/day.h
    ../3th_lib/sxtwl/src/JD.h
    ../3th_lib/sxtwl/src/SSQ.h
    ../3th_lib/sxtwl/src/XL.h
    LunarCalendarDelegate.h
    LunarTable.h
    )
set(LunarCalendar_SOURCE_FILES
    LunarCalendar.cpp
    LunarCalendarModel.cpp
    CalendarLunar.cpp
    ../3th_lib/sxtwl/src/sxtwl.cpp
    ../3th_lib/sxtwl/src/eph.cpp
    ../3th_lib/sxtwl/src/day.cpp
    ../3th_lib/sxtwl/src/JD.cpp
    ../3th_lib/sxtwl/src/SSQ.cpp
    ../3th_lib/sxtwl/src/XL.cpp
    LunarCalendarDelegate.cpp
    LunarTable.cpp
    )

option(WITH_QT_DESIGENER "Add qt designer plugin" ON)
if(NOT ANDROID AND WITH_QT_DESIGENER)
    list(APPEND QT_COMPONENTS Designer)
    list(APPEND LunarCalendar_HEAD_FILES
        FrmLunarCalendarPlugin.h)
    list(APPEND LunarCalendar_SOURCE_FILES
        FrmLunarCalendarPlugin.cpp)
endif()

#设置资源文件
SET(RCC_FILES
    Resource/ResourceLunarCalendar.qrc
    )

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    LIST(APPEND RCC_FILES
        ${TRANSLATIONS_QRC_FILES}
        Resource/ResourceSql.qrc
        )
endif()

#需要的QT组件
list(APPEND QT_COMPONENTS ${QT_COMPONENTS} Core Gui Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
        set(LunarCalendar_QT_COMPONENTS "${LunarCalendar_QT_COMPONENTS} ${_COMPONENT}")
    ENDFOREACH()
endif()

include(GenerateExportHeader)
ADD_TARGET(NAME ${PROJECT_NAME}
    NOT_INSTALL_QT
    SOURCE_FILES ${LunarCalendar_SOURCE_FILES}
        ${LunarCalendar_HEAD_FILES}
        ${RCC_FILES}
    INSTALL_HEADER_FILES ${LunarCalendar_INSTALL_HEAD_FILES}
    PRIVATE_LIBS ${QT_LIBRARIES} RabbitCommon
    INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # See: http://www.it1352.com/478094.html
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE_INCLUDE_DIRS
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../3th_lib/sxtwl/src
    VERSION ${LunarCalendar_VERSION}
    DEFINITIONS ${LunarCalendar_DEFINITIONS}
    INSTALL_EXPORT_NAME ${PROJECT_NAME}Targets
    INSTALL_CMAKE_CONFIG_IN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/LunarCalendarConfig.cmake.in
    )

# Install dababase files
INSTALL_FILE(
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Resource/database/chinese_holidays.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/Resource/database/holidays.sql
    DESTINATION "${CMAKE_INSTALL_DATADIR}/db")

INSTALL_ICON_THEME(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/icons
    PARAMETERS PATTERN "*.theme" EXCLUDE
    )
