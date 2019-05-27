#!/bin/bash
set -ev

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi
echo "SOURCE_DIR:${SOURCE_DIR}"
cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -n "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$BUILD_DOWNLOAD" = "TRUE" ]; then
        QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
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

if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    if [ "$BUILD_DOWNLOAD" = "TRUE" ]; then
        bash build_debpackage.sh ${QT_ROOT}
    else
        bash build_debpackage.sh ${QT_ROOT} 
        
        if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION_DIR}" = "512" ]; then
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/debian/lunarcalendar/opt/LunarCalendar/bin
            MD5=`md5sum ../lunarcalendar*_amd64.deb|awk '{print $1}'`
            echo "MD5:${MD5}"
            ./debian/lunarcalendar/opt/LunarCalendar/bin/LunarCalendarApp \
                -f "`pwd`/update_linux.xml" \
                --md5 ${MD5} 
            export UPLOADTOOL_BODY="Release LunarCalendar-${VERSION}"
            #export UPLOADTOOL_PR_BODY=
            wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
            bash upload.sh ../lunarcalendar*_amd64.deb update_linux.xml
        fi
        
    fi
    if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION_DIR}" = "512" ]; then
        cd debian/lunarcalendar/opt/LunarCalendar
        sed -i "s/^\Icon=.*/Icon=calendar/g" share/applications/LunarCalendar.desktop
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QT_ROOT}/bin:${QT_ROOT}/lib
        wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
        chmod a+x linuxdeployqt-continuous-x86_64.AppImage
        export VERSION="0.0.4"
        ./linuxdeployqt-continuous-x86_64.AppImage share/applications/*.desktop \
            -qmake=${QT_ROOT}/bin/qmake -appimage

        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh

        bash upload.sh lunarcalendar*.AppImage*
    fi
    exit 0
fi

if [ -n "$GENERATORS" ]; then
    if [ -n "${STATIC}" ]; then
        CONFIG_PARA="-DBUILD_SHARED_LIBS=${STATIC}"
    fi
    cmake -G"${GENERATORS}" ${SOURCE_DIR} ${CONFIG_PARA} \
        -DCMAKE_INSTALL_PREFIX=`pwd`/install \
        -DCMAKE_VERBOSE=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5
    cmake --build . --target install --config Release -- ${RABBIT_MAKE_JOB_PARA}
else
    if [ "ON" = "${STATIC}" ]; then
        CONFIG_PARA="CONFIG*=static"
    fi
    if [ "${BUILD_TARGERT}" = "android" ]; then
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
            "CONFIG+=release" ${CONFIG_PARA}
        
        $MAKE
        $MAKE install INSTALL_ROOT=`pwd`/android-build
        ${QT_ROOT}/bin/androiddeployqt \
                       --input `pwd`/App/android-libLunarCalendarApp.so-deployment-settings.json \
                       --output `pwd`/android-build \ 
                       --android-platform ${ANDROID_API} \
                        --gradle --verbose
                        #--jdk ${JAVA_HOME}
    else
        ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
                "CONFIG+=release" ${CONFIG_PARA}\
                PREFIX=`pwd`/install
                
        $MAKE
        echo "$MAKE install ...."
        $MAKE install
    fi
fi

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    if [ "${AUTOBUILD_ARCH}" = "x86" ]; then
        cp /C/OpenSSL-Win32/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win32/bin/ssleay32.dll install/bin
    elif [ "${AUTOBUILD_ARCH}" = "x64" ]; then
        cp /C/OpenSSL-Win64/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win64/bin/ssleay32.dll install/bin
    fi
    
    if [ -z "${STATIC}" ]; then
        "/C/Program Files (x86)/NSIS/makensis.exe" "Install.nsi"
        MD5=`md5sum LunarCalendar-Setup-*.exe|awk '{print $1}'`
        echo "MD5:${MD5}"
        install/bin/LunarCalendarApp.exe -f "`pwd`/update_windows.xml" --md5 ${MD5}
        
        cat update_windows.xml
    fi
fi
