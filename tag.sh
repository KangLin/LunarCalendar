#!/bin/bash

SOURCE_DIR=`pwd`

if [ -n "$1" ]; then
    VERSION=`git describe --tags`
    if [ -z "$VERSION" ]; then
        VERSION=` git rev-parse HEAD`
    fi
    
    echo "Current verion: $VERSION, The version to will be set: $1"
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
LunarCalender ($1) ; urgency=low

  * Upstream update

 -- Kang Lin <kl222@126.com>  $(LANG=C date -R)

`cat $2`
EOF
}

#mkchangelog $VERSION ChangeLog.md

sed -i "s/^\SET(BUILD_VERSION.*/\SET(BUILD_VERSION \"${VERSION}\")/g" ${SOURCE_DIR}/CMakeLists.txt
sed -i "s/^\!define PRODUCT_VERSION.*/\!define PRODUCT_VERSION \"${VERSION}\"/g" ${SOURCE_DIR}/Install/Install.nsi
APPVERYOR_VERSION="version: '${VERSION}.{build}'"
sed -i "s/^version: '.*{build}'/${APPVERYOR_VERSION}/g" ${SOURCE_DIR}/appveyor.yml
sed -i "s/^\    BUILD_VERSION=.*/\    BUILD_VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/App/App.pro
sed -i "s/^\Standards-Version:.*/\Standards-Version:\"${VERSION}\"/g" ${SOURCE_DIR}/debian/control
sed -i "s/^\  - export VERSION=.*/\  - export VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/.travis.yml
sed -i "s/lunarcalendar (.*)/lunarcalendar (${VERSION})/g" ${SOURCE_DIR}/debian/changelog
sed -i "s/export VERSION=.*/export VERSION=\"${VERSION}\"/g" ${SOURCE_DIR}/ci/build.sh

if [ -n "$1" ]; then
    git add .
    git commit -m "Release $1"
    git push
    git tag -d $1
    git tag -a $1 -m "Release $1"
    git push origin :refs/tags/$1
    git push origin $1
fi
