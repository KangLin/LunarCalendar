TARGET = LunarCalendar
TEMPLATE = lib
DESTDIR = $$OUT_PWD/../bin

isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : unnix : PREFIX = /usr/local
    else : PREFIX = $$OUT_PWD/../install
}

include(LunarCalendar.pri)

OTHER_FILES += \
    CMakeLists.txt


header_files.target = header_files
header_files.files = $$INSTALL_HEADERS
win32 {
    header_files.path = $$system_path($${PREFIX})/include

    INSTALL_TARGET = $$system_path($${DESTDIR}/$(TARGET))

    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.files = $$system_path($${DESTDIR}/)
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib target header_files
} else {
    header_files.path = $${PREFIX}/include
    # Default rules for deployment.
    target.path = $${PREFIX}/bin
    !isEmpty(target.path): INSTALLS += target header_files
}
