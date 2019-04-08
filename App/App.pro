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

isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : unnix : PREFIX = /usr/local
    else : PREFIX = $$OUT_PWD/../install
}

win32 {    
    INSTALL_TARGET = $$system_path($${DESTDIR}/$(TARGET))
      
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.files = $$system_path($${DESTDIR}/)
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib
} else {
    # Default rules for deployment.
    target.path = $${PREFIX}/bin
    !isEmpty(target.path): INSTALLS += target
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
else: target.path = $$OUT_PWD/../install/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += \
    CMakeLists.txt
