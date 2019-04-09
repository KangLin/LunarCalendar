# For autocompiling qm-files.
#qthelp://org.qt-project.qmake.5120/qmake/qmake-variable-reference.html

TRANSLATIONS = \
    $$PWD/LunarCalendar_zh_CN.ts \
    $$PWD/LunarCalendar_zh_TW.ts

QM_FILES_RESOURCE_PREFIX = Translations
QM_FILES_INSTALL_PATH = $$PREFIX/Translations

android : CONFIG *= embed_translations
CONFIG(debug, debug|release) : CONFIG *= embed_translations

lessThan(QT_MAJOR_VERSION, 5) : LESS_5_12 = 1
greaterThan(QT_MAJOR_VERSION, 4) : lessThan(QT_MINOR_VERSION, 12) LESS_5_12 = 1

isEmpty(LESS_5_12) {
    CONFIG *= lrelease
    qm_files.CONFIG = no_check_exist
} else {
    include($$PWD/../../../pri/lrelease.pri)
}
