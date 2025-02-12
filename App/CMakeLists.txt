project(LunarCalendarApp LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
#需要的QT组件
SET(QT_COMPONENTS ${QT_COMPONENTS} Core Gui Widgets)
if(ANDROID)
    if(${QT_VERSION_MAJOR} VERSION_LESS 6.0.0)
        LIST(APPEND QT_COMPONENTS AndroidExtras)
    endif()
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 6.0.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 6.0.0")
endif()
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        LIST(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()

set(HEAD_FILES
    MainWindow.h 
    )
set(SOURCE_FILES
    main.cpp 
    MainWindow.cpp 
    )
set(SOURCE_UI_FILES
    MainWindow.ui
    )

set(RESOURCE_FILES ${RESOURCE_FILES}
    Resource/Resource.qrc)
if(WIN32)
    list(APPEND RESOURCE_FILES AppIcon.rc)
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    ISWINDOWS
    VERSION ${LunarCalendar_VERSION}
    SOURCE_FILES ${SOURCE_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES}
        ${RESOURCE_FILES}
    PRIVATE_LIBS RabbitCommon LunarCalendar ${QT_LIBRARIES}
    PRIVATE_DEFINITIONS RABBITCOMMON
        LunarCalendar_VERSION="${LunarCalendar_VERSION}"
        LunarCalendar_REVISION="${LunarCalendar_REVISION}"
    )

if(UNIX AND NOT ANDROID)
    INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/share/LunarCalendar.sh
        DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Application)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/share/org.Rabbit.LunarCalendar.desktop
        DESTINATION "share/applications" COMPONENT Application)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resource/image/LunarCalendar.png
        DESTINATION "share/pixmaps" COMPONENT Application)
endif()

SET(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    )
INSTALL_FILE(SOURCES ${OTHER_FILES}
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Application)

IF(WIN32)
    # 替换 Install.nsi 中的 CMAKE_INSTALL_PREFIX 等
    configure_file(${CMAKE_SOURCE_DIR}/Install/Install.nsi
        ${CMAKE_BINARY_DIR}/Install.nsi @ONLY)
    file(COPY AppIcon.ico DESTINATION ${CMAKE_BINARY_DIR})
ENDIF()

if(EXISTS ${RabbitCommon_ROOT}/Src/etc/logqt.ini)
    INSTALL_FILE(SOURCES ${RabbitCommon_ROOT}/Src/etc/logqt.ini
        DESTINATION etc
            COMPONENT Application
        RENAME LunarCalendar_logqt.ini)
endif()
