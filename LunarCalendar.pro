#-------------------------------------------------
#
# Project created by QtCreator 2019-03-25T16:17:05
#
#-------------------------------------------------

TARGET = LunarCalendar
TEMPLATE = subdirs
CONFIG *= ordered
Src.file = Src/LunarCalendar.pro
App.depends = Src
SUBDIRS = Src App

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
    else : android : PREFIX = /.
    else : unnix : PREFIX = /usr/local
    else : PREFIX = $$OUT_PWD/install
}

DISTFILES += Authors.md \
    Authors_zh_CN.md \
    ChangeLog.md \
    License.md \
    App/AppIcon.ico

other.files = $$DISTFILES
other.path = $$PREFIX
other.CONFIG += directory no_check_exist 

install.files = Install/Install.nsi
install.path = $$OUT_PWD
install.CONFIG += directory no_check_exist 
!android : INSTALLS += other
win32:  INSTALLS += install

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
    debian/*
