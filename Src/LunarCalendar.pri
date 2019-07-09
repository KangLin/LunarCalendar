QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$_PRO_FILE_PWD_/../../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/RabbitCommon.pri"){
    INCLUDEPATH *= $${RabbitCommon_DIR}/Src $${RabbitCommon_DIR}/Src/export
    CONFIG(static): DEFINES *= RABBITCOMMON_STATIC_DEFINE
} else{
    message("Don't find RabbitCommon, in RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download dirctory")
}

INCLUDEPATH *= $$PWD $$PWD/export $$PWD/../3th_lib/sxtwl/src
LIBS *= "-L$$DESTDIR" -lRabbitCommon

TRANSLATIONS_DIR=$$PWD
include($$PWD/../pri/Translations.pri)

msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

CONFIG(static): DEFINES *= LUNARCALENDAR_STATIC_DEFINE
else: DEFINES *= LunarCalendar_EXPORTS

isEmpty(BUILD_VERSION): error("Please set BUILD_VERSION")
VERSION=$$replace(BUILD_VERSION, v,)
win32{
    VERSION=$$split(VERSION, -)
    VERSION=$$first(VERSION)
}

SOURCES += $$PWD/LunarCalendar.cpp \
    $$PWD/LunarCalendarModel.cpp \
    $$PWD/CalendarLunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/lunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/eph.cpp \
    $$PWD/../3th_lib/sxtwl/src/JD.cpp \
    $$PWD/../3th_lib/sxtwl/src/SSQ.cpp \
    $$PWD/../3th_lib/sxtwl/src/XL.cpp \  
    $$PWD/LunarCalendarDelegate.cpp \
    $$PWD/LunarTable.cpp

INSTALL_HEADERS += $$PWD/LunarCalendar.h \
    $$PWD/export/lunarcalendar_export.h \
    $$PWD/export/lunarcalendar_export_windows.h \
    $$PWD/export/lunarcalendar_export_linux.h 

HEADERS += $$INSTALL_HEADERS \
    $$PWD/LunarCalendarModel.h \
    $$PWD/CalendarLunar.h \
    $$PWD/../3th_lib/sxtwl/src/lunar.h \
    $$PWD/../3th_lib/sxtwl/src/eph.h \
    $$PWD/../3th_lib/sxtwl/src/JD.h \
    $$PWD/../3th_lib/sxtwl/src/SSQ.h \
    $$PWD/../3th_lib/sxtwl/src/XL.h \
    $$PWD/LunarCalendarDelegate.h \
    $$PWD/LunarTable.h

FORMS +=

RESOURCES += \
    $$PWD/Resource/ResourceLunarCalendar.qrc
