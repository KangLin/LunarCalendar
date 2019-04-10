QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

INCLUDEPATH = $$PWD $$PWD/../3th_lib/sxtwl/src
msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

DEFINES += LunarCalendar_EXPORTS

SOURCES += $$PWD/LunarCalendar.cpp \
    $$PWD/LunarCalendarModel.cpp \
    $$PWD/CalendarLunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/lunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/eph.cpp \
    $$PWD/../3th_lib/sxtwl/src/JD.cpp \
    $$PWD/../3th_lib/sxtwl/src/SSQ.cpp \
    $$PWD/../3th_lib/sxtwl/src/XL.cpp \  
    $$PWD/LunarCalendarDelegate.cpp

INSTALL_HEADERS += $$PWD/LunarCalendar.h \
    $$PWD/lunarcalendar_export.h \
    $$PWD/lunarcalendar_export_windows.h \
    $$PWD/lunarcalendar_export_linux.h 

HEADERS += $$INSTALL_HEADERS \
    $$PWD/LunarCalendarModel.h \
    $$PWD/CalendarLunar.h \
    $$PWD/../3th_lib/sxtwl/src/lunar.h \
    $$PWD/../3th_lib/sxtwl/src/eph.h \
    $$PWD/../3th_lib/sxtwl/src/JD.h \
    $$PWD/../3th_lib/sxtwl/src/SSQ.h \
    $$PWD/../3th_lib/sxtwl/src/XL.h \
    $$PWD/LunarCalendarDelegate.h

FORMS += $$PWD/LunarCalendar.ui

include($$PWD/../pri/Translations.pri)
