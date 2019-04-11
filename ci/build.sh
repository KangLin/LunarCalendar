#!/bin/bash
set -ev

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -z "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
    export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA="-j2"
    fi
fi

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export PATH=/C/Qt/Tools/mingw${TOOLCHAIN_VERSION}/bin:$PATH    
fi
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        export PKG_CONFIG=/c/msys64/mingw32/bin/pkg-config.exe
        ;;
    Linux* | Unix*)
    ;;
    *)
    ;;
esac

export PATH=${QT_ROOT}/bin:$PATH
echo "PATH:$PATH"
echo "PKG_CONFIG:$PKG_CONFIG"
cd ${SOURCE_DIR}
if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    ./tag.sh
fi
mkdir -p build_${BUILD_TARGERT}
cd build_${BUILD_TARGERT}

case ${BUILD_TARGERT} in
    windows_msvc)
        MAKE=nmake
        ;;
    windows_mingw)
        if [ "${RABBIT_BUILD_HOST}"="windows" ]; then
            MAKE="mingw32-make ${RABBIT_MAKE_JOB_PARA}"
        fi
        ;;
    *)
        MAKE="make ${RABBIT_MAKE_JOB_PARA}"
        ;;
esac

if [ -n "$GENERATORS" ]; then
    cmake -G"${GENERATORS}" ${SOURCE_DIR} \
         -DCMAKE_INSTALL_PREFIX=`pwd`/install \
         -DCMAKE_VERBOSE=ON \
         -DCMAKE_BUILD_TYPE=Release \
         -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5
    cmake --build . --target install --config Release ${RABBIT_MAKE_JOB_PARA}
else
    if [ "${BUILD_TARGERT}" = "android" ]; then
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR}/LunarCalendar.pro \
            "CONFIG+=release" 
    else
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR}/LunarCalendar.pro \
            "CONFIG+=release" \
            PREFIX=`pwd`/install
    fi
    $MAKE
    echo "$MAKE install ...."
    $MAKE install
fi
#if [ "${BUILD_TARGERT}" != "android" ]; then
#    "/C/Program Files (x86)/NSIS/makensis.exe" "Install.nsi"
#fi
