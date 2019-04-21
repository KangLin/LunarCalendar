#!/bin/bash

export RabbitCommon_DIR=`pwd`/../RabbitCommon
export Qt5_DIR=/android/qt/5.12.1/gcc_64/lib/cmake/Qt5
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Qt5_DIR/../..
export LD_LIBRARY_PATH
fakeroot debian/rules binary 

