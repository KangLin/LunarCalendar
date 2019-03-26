QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += LunarCalendar.cpp \
    $$PWD/LunarCalendarModel.cpp
HEADERS += LunarCalendar.h \
    $$PWD/LunarCalendarModel.h
FORMS += LunarCalendar.ui
