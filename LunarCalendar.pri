QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

SOURCES += LunarCalendar.cpp \
    $$PWD/LunarCalendarModel.cpp
HEADERS += LunarCalendar.h \
    $$PWD/LunarCalendarModel.h
FORMS += LunarCalendar.ui
