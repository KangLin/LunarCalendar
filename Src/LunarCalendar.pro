TARGET = LunarCalendar
TEMPLATE = lib
DESTDIR = $$OUT_PWD/../bin
include(LunarCalendar.pri)

OTHER_FILES += \
    CMakeLists.txt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}
else: unix:!android: target.path = /opt/$${TARGET}
else: target.path = $$OUT_PWD/../install
target.path = $${target.path}/bin
!isEmpty(target.path): INSTALLS += target
