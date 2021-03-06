#!/bin/bash 
#下载工具  

set -ev

SOURCE_DIR="`pwd`"
echo $SOURCE_DIR
TOOLS_DIR=${SOURCE_DIR}/Tools
PACKAGE_DIR=${SOURCE_DIR}/Package
echo ${TOOLS_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${TOOLS_DIR}/android-sdk
    export ANDROID_NDK_ROOT=${TOOLS_DIR}/android-ndk
    export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    export PATH=${TOOLS_DIR}/apache-ant/bin:$JAVA_HOME:$PATH
else
    exit 0
fi

if [ ! -d "${TOOLS_DIR}" ]; then
    mkdir ${TOOLS_DIR}
fi
if [ ! -d "$PACKAGE_DIR" ]; then
    mkdir -p $PACKAGE_DIR
fi
cd ${TOOLS_DIR}

# Qt qt安装参见：https://github.com/benlau/qtci  
if [ -n "${QT_VERSION}" ]; then
    QT_DIR=C:/projects/${APPVEYOR_PROJECT_NAME}/Tools/Qt/${QT_VERSION}
    if [ ! -d "${QT_DIR}" ]; then
        cd $PACKAGE_DIR
        if [ ! -f qt-opensource-windows-x86-${QT_VERSION}.exe ]; then
            wget -c --no-check-certificate -nv http://download.qt.io/official_releases/qt/${QT_VERSION_DIR}/${QT_VERSION}/qt-opensource-windows-x86-${QT_VERSION}.exe
        fi
        bash ${SOURCE_DIR}/ci/qt-installer.sh qt-opensource-windows-x86-${QT_VERSION}.exe ${QT_DIR}
    fi
fi

cd ${TOOLS_DIR}

#下载ANT 
#wget -c -nv http://apache.fayea.com//ant/binaries/apache-ant-1.10.1-bin.tar.gz
#tar xzf apache-ant-1.10.1-bin.tar.gz
#rm -f apache-ant-1.10.1-bin.tar.gz
#mv apache-ant-1.10.1 apache-ant

cd ${TOOLS_DIR}

#Download android sdk  
if [ ! -d "${TOOLS_DIR}/android-sdk" ]; then
    

    cd ${PACKAGE_DIR}
    ANDROID_STUDIO_VERSION=191.5900203
    if [ ! -f android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip ]; then
        wget -c -nv https://dl.google.com/dl/android/studio/ide-zips/3.5.1.0/android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
    fi
    cp android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip ${TOOLS_DIR}/.
    cd ${TOOLS_DIR}
    unzip -q android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
    #rm android-studio-ide-${ANDROID_STUDIO_VERSION}-windows.zip
    export JAVA_HOME=${TOOLS_DIR}/android-studio/jre
    export PATH=${JAVA_HOME}/bin:$PATH
    
    cd ${PACKAGE_DIR}
    ANDROID_SDK_VERSION=4333796
    if [ ! -f sdk-tools-windows-${ANDROID_SDK_VERSION}.zip ]; then
        wget -c -nv https://dl.google.com/android/repository/sdk-tools-windows-${ANDROID_SDK_VERSION}.zip
    fi
    cp sdk-tools-windows-${ANDROID_SDK_VERSION}.zip ${TOOLS_DIR}/.
    cd ${TOOLS_DIR}
    mkdir android-sdk
    cd android-sdk
    mv ../sdk-tools-windows-${ANDROID_SDK_VERSION}.zip .
    unzip -q sdk-tools-windows-${ANDROID_SDK_VERSION}.zip
    #rm sdk-tools-windows-${ANDROID_SDK_VERSION}.zip

    echo "Install sdk and ndk ......"
    if [ -n "${ANDROID_API}" ]; then
        PLATFORMS="platforms;${ANDROID_API}"
    else
        PLATFORMS="platforms"
    fi
    if [ -z "${BUILD_TOOS_VERSION}" ]; then
        BUILD_TOOS_VERSION="28.0.3"
    fi
    (sleep 5 ; num=0 ; while [ $num -le 5 ] ; do sleep 1 ; num=$(($num+1)) ; printf 'y\r\n' ; done ) \
    | ./tools/bin/sdkmanager.bat "platform-tools" "build-tools;${BUILD_TOOS_VERSION}" "${PLATFORMS}" "ndk-bundle"
    cd ${TOOLS_DIR}
fi

cd ${SOURCE_DIR}
