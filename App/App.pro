TARGET = LunarCalendarApp
TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#Get app version use git, please set git path to environment variable PATH
isEmpty(BUILD_VERSION) {
    isEmpty(GIT) : GIT=$$(GIT)
    isEmpty(GIT) : GIT=git
    isEmpty(GIT_DESCRIBE) {
        GIT_DESCRIBE = $$system(cd $$system_path($$_PRO_FILE_PWD_) && $$GIT describe --tags)
        isEmpty(BUILD_VERSION) {
            BUILD_VERSION = $$GIT_DESCRIBE
        }
    }
    isEmpty(BUILD_VERSION) {
        BUILD_VERSION = $$system(cd $$system_path($$_PRO_FILE_PWD_) && $$GIT rev-parse --short HEAD)
    }
    
    isEmpty(BUILD_VERSION){
        warning("Built without git, please add BUILD_VERSION to DEFINES or add git path to environment variable GIT or qmake parameter GIT")
    }
}
isEmpty(BUILD_VERSION){
    BUILD_VERSION="v0.0.3"
}
message("BUILD_VERSION:$$BUILD_VERSION")
DEFINES += BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"

msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

INCLUDEPATH = ../Src ../Src/export

!android: DESTDIR = $$OUT_PWD/../bin

SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        MainWindow.h

FORMS += \
        MainWindow.ui

RESOURCES += \
    Resource/Resource.qrc

RC_FILE = AppIcon.rc

android {
    LIBS *= "-L$$OUT_PWD/../Src"
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
    !android: target.path = $${PREFIX}/bin
    !isEmpty(target.path): INSTALLS += target
}

OTHER_FILES += \
    CMakeLists.txt \
    AppIcon.rc

include(../pri/Translations.pri)

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/RabbitCommon.pri"){
    DEFINES += RABBITCOMMON
    include("$${RabbitCommon_DIR}/Src/RabbitCommon.pri")
} else{
    message("RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download root dirctory")
}
