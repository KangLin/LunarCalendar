TARGET = LunarCalendarApp
TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

INCLUDEPATH = ../Src

DESTDIR = $$OUT_PWD/../bin

SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        MainWindow.h

FORMS += \
        MainWindow.ui

android {
    LIBS *= "-L$$OUT_PWD/../LunarCalendar"
} else {
    LIBS *= "-L$$DESTDIR"
}
LIBS *= -lLunarCalendar

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
else: target.path = $$OUT_PWD/../install/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += \
    CMakeLists.txt
