#-------------------------------------------------
#
# Project created by QtCreator 2019-03-25T16:17:05
#
#-------------------------------------------------

versionAtMost(QT_VERSION, 5.6) : error("Qt version must greater 5.6")

TARGET = LunarCalendar
TEMPLATE = subdirs
CONFIG *= ordered

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/RabbitCommon.pri"){
    RabbitCommon.file = 3th_lib/RabbitCommon.pro
    SUBDIRS += RabbitCommon
} else{
    message("Don't find RabbitCommon, in RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download dirctory")
}

Src.file = Src/LunarCalendar.pro
#App.depends = Src
SUBDIRS += Src App

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : ios: PREFIX=/
    else : android : PREFIX = /
    else : PREFIX = $$OUT_PWD/install
}

DISTFILES += Authors.md \
    Authors_zh_CN.md \
    ChangeLog.md \
    License.md
win32: DISTFILES += App/AppIcon.ico

other.files = $$DISTFILES
android: other.path = $$PREFIX/assets
else: other.path = $$PREFIX
other.CONFIG += directory no_check_exist
INSTALLS += other

install_win.files = Install/Install.nsi
install_win.path = $$OUT_PWD
install_win.CONFIG += directory no_check_exist
win32: INSTALLS += install_win

install_unix.files = Install/install.sh
install_unix.path = $$PREFIX
install_unix.CONFIG += directory no_check_exist
unix: !android: INSTALLS += install_unix

OTHER_FILES += appveyor.yml \
    .travis.yml \
    README.md \
    README_zh_CN.md \
    ci/* \
    tag.sh \
    CMakeLists.txt \
    cmake/* \
    Install/* \
    Update/* \
    debian/* \
    test/* \
    build_debpackage.sh
