TARGET = LunarCalendarApp
TEMPLATE = app
CONFIG(staticlib): CONFIG*=static

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

android{
    versionAtMost(QT_VERSION, 5.4.0) : error("Android: Qt version must greater than or equal to 5.4.0")
    QT += androidextras gui
}

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
    BUILD_VERSION="0.0.5"
}
message("BUILD_VERSION:$$BUILD_VERSION")
DEFINES += BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"

msvc {
    QMAKE_CXXFLAGS += "/utf-8"
    QMAKE_LFLAGS *= /SUBSYSTEM:WINDOWS",5.01"
}

isEmpty(DESTDIR): DESTDIR = $$OUT_PWD/../bin
INCLUDEPATH = ../Src ../Src/export
LIBS *= "-L$$DESTDIR" -lLunarCalendar

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
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    isEmpty(THIRDLIBRARY_PATH) message("Please set THIRDLIBRARY_PATH")
    CONFIG(static){
        LIBS *= -lssl
    } else {
        #ANDROID_EXTRA_LIBS += #\
          #$${THIRDLIBRARY_PATH}/libssl.so \
          #$${THIRDLIBRARY_PATH}/libcrypto.so 
    }

    RESOURCE_QRC_FILE = $$OUT_PWD/ResourceAbout.qrc

    RESOURCE_QRC_FILE_CONTENT = \
                "<!DOCTYPE RCC><RCC version=\"1.0\">" \
                "<qresource prefix=\"file\">"
    
    RESOURCE_QRC_FILE_CONTENT += \
            "<file alias=\"Authors\">$$PWD/../Authors.md</file>"
    RESOURCE_QRC_FILE_CONTENT += \
            "<file alias=\"Authors_zh_CN\">$$PWD/../Authors_zh_CN.md</file>"
    RESOURCE_QRC_FILE_CONTENT += \
            "<file alias=\"ChangeLog\">$$PWD/../ChangeLog.md</file>"
    RESOURCE_QRC_FILE_CONTENT += \
            "<file alias=\"License\">$$PWD/../License.md</file>"

    RESOURCE_QRC_FILE_CONTENT += \
            "</qresource>" \
            "</RCC>"
    !write_file($$RESOURCE_QRC_FILE, RESOURCE_QRC_FILE_CONTENT): \
            error()
    RESOURCES += $$RESOURCE_QRC_FILE

}

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

!android : unix {
    DESKTOP_FILE.target = DESKTOP_FILE
    DESKTOP_FILE.files = $$PWD/../debian/LunarCalendar.desktop
    DESKTOP_FILE.path = $$system_path($${PREFIX})/share/applications
    DESKTOP_FILE.CONFIG += directory no_check_exist

    # install icons
    icon.target = icon
    icon.files = Resource/image/LunarCalendar.png
    icon.path = $${PREFIX}/share/pixmaps
    icon.CONFIG = directory no_check_exist
    INSTALLS += DESKTOP_FILE icon
}

OTHER_FILES += \
    CMakeLists.txt \
    AppIcon.rc \
    android/* \
    android/res/* \
    android/res/values/*

include(../pri/Translations.pri)

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/RabbitCommon.pri"){
    DEFINES += RABBITCOMMON
    include("$${RabbitCommon_DIR}/Src/RabbitCommon.pri")
} else{
    message("Don't find RabbitCommon, in RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download dirctory")
}
