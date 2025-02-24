# 开发文档

## 版本号说明：

- 版本号说明： [https://semver.org/lang/zh-CN/](https://semver.org/lang/zh-CN/)

## 工具

+ [cmake](https://cmake.org)
+ [git](https://git-scm.com/)
+ 编译器
  - Linux
    - gcc
  - Windows
    - msvc
    - MinGW

## 依赖  

+ [Qt (LGPL v2.1)](https://www.qt.io/)
+ [RabbitCommon](https://github.com/KangLin/RabbitCommon) 
  
      git clone https://github.com/KangLin/RabbitCommon.git

## 编译

### 建立并进入build目录

    git clone --recursive https://github.com/KangLin/LunarCalendar.git
    cd LunarCalendar
    mkdir build

### 平台

#### linux
    
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
          -DCMAKE_BUILD_TYPE=Release \
          -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
          -DRabbitCommon_ROOT=......
    cmake --build . --config Release --target install ;安装
    cmake --build . --config Release --target package ;生成安装包
        
#### windows
    
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install ^
             -DCMAKE_BUILD_TYPE=Release ^
             -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 ^
             -DRabbitCommon_ROOT=......
    cmake --build . --config Release --target install ;安装
    cmake --build . --config Release --target package ;生成安装包

#### android

##### 主机是 linux

    cd build
    # 如果是 Qt6
    
    # 使用 qt-cmake
    qt-cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
        -DRabbitCommon_ROOT=......

    # 使用 cmake
    cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI="armeabi-v7a with NEON" \
        -DANDROID_PLATFORM=android-18 \
        -DQT_DIR= \
        -DQt6_DIR= \
        -DRabbitCommon_ROOT=......
    cmake --build . --target all

    # 如果是 Qt5
    cmake .. -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI="armeabi-v7a with NEON" \
        -DANDROID_PLATFORM=android-18 \
        -DQT_DIR= \
        -DQt5_DIR= \
        -DRabbitCommon_ROOT=......
    cmake --build . --config Release --target install
    cmake --build . --target APK
    
##### 主机是 windows

    ; 如果是 Qt6
    ; 使用 qt-cmake
    qt-cmake .. -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build ^
        -DRabbitCommon_ROOT=......

    ; 使用 cmake
    cmake .. -G"Unix Makefiles" ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build ^
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake ^
        -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe ^
        -DANDROID_PLATFORM=android-18 ^
        -DANDROID_ABI=arm64-v8a ^
        -DANDROID_ARM_NEON=ON ^
        -DQT_DIR= ^
        -DQt6_DIR= ^
        -DRabbitCommon_ROOT=......
    cmake --build . --target all

    cd build
    ; 如果是 Qt5
    cmake .. -G"Unix Makefiles" ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=`pwd`/android-build ^
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake ^
        -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe ^
        -DANDROID_PLATFORM=android-18 ^
        -DANDROID_ABI=arm64-v8a ^
        -DANDROID_ARM_NEON=ON ^
        -DQT_DIR= ^
        -DQt5_DIR= ^
        -DRabbitCommon_ROOT=......
    cmake --build . --config Release --target install
    cmake --build . --target APK

- 参数说明：https://developer.android.google.cn/ndk/guides/cmake
  + ANDROID_ABI: 可取下列值：  
目标 ABI。如果未指定目标 ABI，则 CMake 默认使用 armeabi-v7a。
有效的目标名称为：  
    - armeabi：带软件浮点运算并基于 ARMv5TE 的 CPU。
    - armeabi-v7a：带硬件 FPU 指令 (VFPv3_D16) 并基于 ARMv7 的设备。
    - armeabi-v7a with NEON：与 armeabi-v7a 相同，但启用 NEON 浮点指令。这相当于设置 -DANDROID_ABI=armeabi-v7a 和 -DANDROID_ARM_NEON=ON。
    - arm64-v8a：ARMv8 AArch64 指令集。
    - x86：IA-32 指令集。
    - x86_64 - 用于 x86-64 架构的指令集。
  + ANDROID_NDK <path> 主机上安装的 NDK 根目录的绝对路径
  + ANDROID_PLATFORM: 如需平台名称和对应 Android 系统映像的完整列表，请参阅 [Android NDK 原生 API](https://developer.android.google.cn/ndk/guides/stable_apis.html)
  + ANDROID_ARM_MODE
  + ANDROID_ARM_NEON
  + ANDROID_STL:指定 CMake 应使用的 STL。 
    - c++_shared: 使用 libc++ 动态库
    - c++_static: 使用 libc++ 静态库
    - none: 没有 C++ 库支持
    - system: 用系统的 STL
- 安装 apk 到设备
    
      adb install android-build-debug.apk 

## 在 v1.0.2 以前，安装注意

Qt因为版权原因，没有提供openssl动态库，所以必须自己复制openssl的动态库到安装目录下。

+ windows
  - 如果是32的，可以在 Qt 安装程序 Tools\QtCreator\bin 目录下，找到openssl的动态库（libeay32.dll、ssleay32.dll）
  - 如果是64位，则需要自己下载 openssl 的二进制安装包。
+ linux

      sudo apt-get install libssl1.1

**注意:** Qt6.2.4 以前版本使用 openssl 1.1X 。Qt6.2.4 及以后版本使用 openssl 3.x.x 版本。

## 开发

- [在线开发文档](https://kanglin.github.io/LunarCalendar/html/index.html)
- 目录结构

```
|-App                                  应用程序代码
|-Src                                  日历库代码
   |- LunarCalendar.h                  日历库接口头文件
   |- Resource
       |- database
            |- chinese_holidays.sql 中国节假日 SQL 文件
            |- holidays.sql         节日及节气 SQL 文件
```

- 中国假日更新
  - 从中国政府网 “首页 → 信息公开 → 国务院文件 > 综合政务 → 其他” 找到假期文件
  
例如： [国务院办公厅关于2020年部分节假日安排的通知](http://www.gov.cn/zhengce/content/2019-11/21/content_5454164.htm)

  - 修改中国假期 SQL 文件： Src/Resource/database/chinese_holidays.sql
