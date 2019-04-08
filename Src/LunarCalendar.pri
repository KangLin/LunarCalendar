QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

DEFINES += LunarCalendar_EXPORTS
INCLUDEPATH = $$PWD/../3th_lib/sxtwl/src
msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

SOURCES += LunarCalendar.cpp \
    $$PWD/LunarCalendarModel.cpp \
    $$PWD/CalendarLunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/lunar.cpp \
    $$PWD/../3th_lib/sxtwl/src/eph.cpp \
    $$PWD/../3th_lib/sxtwl/src/JD.cpp \
    $$PWD/../3th_lib/sxtwl/src/SSQ.cpp \
    $$PWD/../3th_lib/sxtwl/src/XL.cpp \  
    $$PWD/LunarCalendarDelegate.cpp

INSTALL_HEADERS += LunarCalendar.h
HEADERS += $$INSTALL_HEADERS \
    $$PWD/LunarCalendarModel.h \
    $$PWD/CalendarLunar.h \
    $$PWD/../3th_lib/sxtwl/src/lunar.h \
    $$PWD/../3th_lib/sxtwl/src/eph.h \
    $$PWD/../3th_lib/sxtwl/src/JD.h \
    $$PWD/../3th_lib/sxtwl/src/SSQ.h \
    $$PWD/../3th_lib/sxtwl/src/XL.h \
    $$PWD/LunarCalendarDelegate.h

FORMS += LunarCalendar.ui

install.files = $$INSTALL_HEADERS
# Default rules for deployment.
qnx: install.path = /tmp/$${TARGET}
else: unix:!android: install.path = /opt/$${TARGET}
else: install.path = $$OUT_PWD/../install
install.path = $${install.path}/include
!isEmpty(install.path): INSTALLS += install
