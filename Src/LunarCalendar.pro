TARGET = LunarCalendar
TEMPLATE = lib
DESTDIR = $$OUT_PWD/../bin
include(LunarCalendar.pri)

OTHER_FILES += \
    CMakeLists.txt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
else: target.path = $$OUT_PWD/../install/bin
!isEmpty(target.path): INSTALLS += target
