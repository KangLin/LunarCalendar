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

if(DEFINED RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/RabbitCommon)
        add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
    endif()
    include(${RabbitCommon_DIR}/cmake/RabbitCommonUtils.cmake)
else()
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon")
    message("   ag:")
    message("       git clone https://github.com/KangLin/RabbitCommon.git")
    message("2. Then set cmake value or environment variable RabbitCommon_DIR to download root directory.")
    message("   ag:")
    message(FATAL_ERROR "       cmake -DRabbitCommon_DIR= ")
endif()

GET_VERSION(OUT_VERSION LunarCalendar_VERSION
    OUT_REVISION LunarCalendar_REVISION)
if(NOT LunarCalendar_VERSION)
    SET(LunarCalendar_VERSION "0.2.4")
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

set(LunarCalendar_INSTALL_HEAD_FILES
    LunarCalendar.h
    )
set(LunarCalendar_HEAD_FILES
    ${LunarCalendar_INSTALL_HEAD_FILES}
    LunarCalendarModel.h
    CalendarLunar.h
    ../3th_lib/sxtwl/src/lunar.h
    ../3th_lib/sxtwl/src/eph.h
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
    ../3th_lib/sxtwl/src/lunar.cpp
    ../3th_lib/sxtwl/src/eph.cpp
    ../3th_lib/sxtwl/src/JD.cpp
    ../3th_lib/sxtwl/src/SSQ.cpp
    ../3th_lib/sxtwl/src/XL.cpp
    LunarCalendarDelegate.cpp
    LunarTable.cpp
    )

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
SET(QT_COMPONENTS ${QT_COMPONENTS} Core Gui Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()

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
    )

foreach(d ${LunarCalendar_DEFINITIONS})
    SET(LunarCalendar_DEFINITIONS_PC "${LunarCalendar_DEFINITIONS_PC} -D${d}")
endforeach()

# Install pc files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/LunarCalendar.pc.in
    ${CMAKE_BINARY_DIR}/LunarCalendar.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/LunarCalendar.pc
    DESTINATION lib/pkgconfig)

# Install dababase files
if(ANDROID)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/database/chinese_holidays.sql
        DESTINATION "assets/data/db" COMPONENT Runtime)
else()
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/database/chinese_holidays.sql
        DESTINATION "./data/db" COMPONENT Runtime)
endif()

INSTALL_ICON_THEME()
