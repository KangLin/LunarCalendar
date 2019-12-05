TARGET = LunarCalendar
TEMPLATE = lib

isEmpty(DESTDIR): DESTDIR = $$OUT_PWD/../bin
CONFIG += c++11 link_pkgconfig create_prl link_prl
CONFIG(staticlib): CONFIG*=static
#android: CONFIG*=static

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
    BUILD_VERSION="v0.1.7"
}
message("LunarCalendar BUILD_VERSION:$$BUILD_VERSION")
DEFINES += BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"

isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : PREFIX = $$OUT_PWD/../install
}

include(LunarCalendar.pri)

OTHER_FILES += \
    CMakeLists.txt \
    database/*

# Default rules for deployment.
!android: target.path = $${PREFIX}/bin
INSTALLS += target

header_files.target = header_files
header_files.files = $$INSTALL_HEADERS
header_files.path = $$system_path($${PREFIX}/include/LunarCalendar)
!android: INSTALLS += header_files

!CONFIG(static): win32 {
    INSTALL_TARGET = $$system_path($${DESTDIR}/$(TARGET))

    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.files = $$system_path($${DESTDIR}/)
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib
}
