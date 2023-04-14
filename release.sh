#!/bin/bash

set -e

SOURCE_DIR=`pwd`

if [ -n "$1" ]; then
    VERSION=`git describe --tags`
    if [ -z "$VERSION" ]; then
        VERSION=` git rev-parse HEAD`
    fi
    
    echo "Current version: $VERSION, The version to will be set: $1"
    echo "Please check the follow list:"
    echo "    - Test is ok ?"
    echo "    - Translation is ok ?"
    echo "    - Setup file is ok ?"
    
    read -t 30 -p "Be sure to input Y, not input N: " INPUT
    if [ "$INPUT" != "Y" -a "$INPUT" != "y" ]; then
        exit 0
    fi
    git tag -a $1 -m "Release $1"
fi

VERSION=`git describe --tags`
if [ -z "$VERSION" ]; then
    VERSION=` git rev-parse --short HEAD`
fi

mkchangelog() {
  cat >debian/changelog<<EOF
LunarCalendar ($1) ; urgency=low

  * Upstream update

 -- Kang Lin <kl222@126.com>  $(LANG=C date -R)

`cat $2`
EOF
}

#mkchangelog $VERSION ChangeLog.md

sed -i "s/^\SET(LunarCalendar_VERSION.*/\SET(LunarCalendar_VERSION \"${VERSION}\")/g" ${SOURCE_DIR}/CMakeLists.txt
sed -i "s/^\!define PRODUCT_VERSION.*/\!define PRODUCT_VERSION \"${VERSION}\"/g" ${SOURCE_DIR}/Install/Install.nsi
sed -i "s/^version: '.*{build}'/version: '${VERSION}.{build}'/g" ${SOURCE_DIR}/appveyor.yml
sed -i "s/^\  - export VERSION=.*/\  - export VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/.travis.yml
sed -i "s/LunarCalendar_VERSION:.*/LunarCalendar_VERSION: \"${VERSION}\"/g" ${SOURCE_DIR}/appveyor.yml
sed -i "s/^\    LunarCalendar_VERSION=.*/\    LunarCalendar_VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/App/App.pro
sed -i "s/^\    LunarCalendar_VERSION=.*/\    LunarCalendar_VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/Src/LunarCalendar.pro
sed -i "s/^\Standards-Version:.*/\Standards-Version:\"${VERSION}\"/g" ${SOURCE_DIR}/debian/control
sed -i "s/export VERSION=".*"/export VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/ci/build.sh
sed -i "s/<VERSION>.*</<VERSION>${VERSION}</g" ${SOURCE_DIR}/Update/update.xml
sed -i "s/LunarCalendar_VERSION:.*/LunarCalendar_VERSION: ${VERSION}/g" ${SOURCE_DIR}/.github/workflows/msvc.yml
sed -i "s/LunarCalendar_VERSION:.*/LunarCalendar_VERSION: ${VERSION}/g" ${SOURCE_DIR}/.github/workflows/macos.yml

DEBIAN_VERSION=`echo ${VERSION}|cut -d "v" -f 2`
sed -i "s/[0-9]\+\.[0-9]\+\.[0-9]\+/${DEBIAN_VERSION}/g" ${SOURCE_DIR}/README*.md
sed -i "s/lunarcalendar (.*)/lunarcalendar (${DEBIAN_VERSION})/g" ${SOURCE_DIR}/debian/changelog
sed -i "s/[0-9]\+\.[0-9]\+\.[0-9]\+/${DEBIAN_VERSION}/g" ${SOURCE_DIR}/App/android/AndroidManifest.xml
sed -i "s/LunarCalendar_VERSION:.*/LunarCalendar_VERSION: ${DEBIAN_VERSION}/g" ${SOURCE_DIR}/.github/workflows/ubuntu.yml

MAJOR_VERSION=`echo ${DEBIAN_VERSION}|cut -d "." -f 1`
sed -i "s/android:versionCode=.*android/android:versionCode=\"${MAJOR_VERSION}\" android/g"  ${SOURCE_DIR}/App/android/AndroidManifest.xml

if [ -n "$1" ]; then
    git add .
    git commit -m "Release $1"
    git tag -d $1
    git tag -a $1 -m "Release $1"
    git push
    git push origin $1
fi
