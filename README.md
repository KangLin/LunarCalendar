## LunarCalendar

Author: KangLin (kl222@126.com)

- Project position: https://github.com/KangLin/LunarCalendar
- [Project home](https://kanglin.github.io/LunarCalendar/)

[<img src="https://gitee.com/kl222/Calendar/raw/master/Resource/Image/china.png" alt="Chinese" title="Chinese" width="16" height="16"/>Chinese](README_zh_CN.md)

[![msvc](https://github.com/KangLin/LunarCalendar/actions/workflows/msvc.yml/badge.svg)](https://github.com/KangLin/LunarCalendar/actions/workflows/msvc.yml)
[![qmake](https://github.com/KangLin/LunarCalendar/actions/workflows/qmake.yml/badge.svg)](https://github.com/KangLin/LunarCalendar/actions/workflows/qmake.yml)
[![ubuntu](https://github.com/KangLin/LunarCalendar/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/KangLin/LunarCalendar/actions/workflows/ubuntu.yml)
[![Windows Build status](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/master)
[![Windows Build status](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/develop?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/develop)
[![Linux Build Status](https://travis-ci.org/KangLin/LunarCalendar.svg?branch=master)](https://travis-ci.org/KangLin/LunarCalendar)

[![GitHub stars](https://img.shields.io/github/stars/KangLin/LunarCalendar)](https://github.com/KangLin/LunarCalendar/stars)
[![Gitee stars](https://gitee.com/kl222/LunarCalendar/badge/star.svg?theme=dark)](https://gitee.com/kl222/LunarCalendar/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/KangLin/LunarCalendar)](https://github.com/KangLin/LunarCalendar/forks)
[![GitHub issues](https://img.shields.io/github/issues/KangLin/LunarCalendar)](https://github.com/KangLin/LunarCalendar/issues)
[![Current release](https://img.shields.io/github/release/KangLin/LunarCalendar)](https://github.com/KangLin/LunarCalendar/releases)
[![All releases downloads](https://img.shields.io/github/downloads/KangLin/LunarCalendar/total)](https://github.com/KangLin/LunarCalendar/releases)
[![Download LunarCalendar](https://img.shields.io/sourceforge/dt/lunarcalendar.svg)](https://sourceforge.net/projects/lunarcalendar/files/latest/download)

------------------------------------------------

### Table of Contents:
- [Features](#Features)
- [Donation](#Donation)
- [Screenshots](#Screenshots)
- [Download setup package](#Download-setup-package)
- [Compile](#Compile)
- [Other application use the libary](#Other-application-use-the-libary)
- [Contribution](#Contribution)
- [License Agreement](#License-Agreement)
    - [The third-party libraries](#The-third-party-libraries)

------------------------------------------------

### Features

The lunar calendar written by Qt. It provides:

- A lunar calendar library of Qt GUI, support QSS.
   + Solar calendar
   + Lunar calendar
   + Solar and Lunar holiday(include chinese holidays)
   + Solar and Lunar Day (eg birthday, wedding anniversary, etc.)
   + Month view
   + Week view
- Example
  + A simple calendar app. Used to illustrate how to use this lunar calendar library.
  + A complete application of this library [Calendar](https://github.com/KangLin/Calendar)
- Cross-platform, support multiple operating systems
   + [x] Windows
   + [x] Linux、Unix
   + [x] Android
   + [ ] Mac os
   + [ ] IOS

Mac os and IOS, I don't have the corresponding equipment,
please compile and test the students with the corresponding equipment.

### Donation

[![donation](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")

### Screenshots
- Windows

![Screenshots](Docs/image/ScreenShot.PNG "Screenshots")
![Screenshots](Docs/image/ScreenShotQss.PNG "Screenshots")

- Android

![Android Screenshots](Docs/image/ScreenShotAndroid.PNG "Android Screenshots")

### [Download setup package](https://github.com/KangLin/LunarCalendar/releases/latest)

- Ubuntu:
  + Ubuntu 20.04: lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb
    
        sudo apt install ./lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb
        
        # If you're on an older Linux distribution, you will need to run this instead:
        # sudo dpkg -i lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb
        # sudo apt-get install -f # Install dependencies

    - [Download from github](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb)
    - [Download from sourceforge](https://sourceforge.net/projects/LunarCalendar/files/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb/download)
  + MD5SUM file: lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb.md5sum
    - [Download from github](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb.md5sum)
    - [Download from sourceforge](https://sourceforge.net/projects/LunarCalendar/files/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-20.04.deb.md5sum/download)
  + Ubuntu 18.04: lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb

        sudo apt install ./lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb
        
        # If you're on an older Linux distribution, you will need to run this instead:
        # sudo dpkg -i lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb
        # sudo apt-get install -f # Install dependencies

    - [Download from github](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb)
    - [Download from sourceforge](https://sourceforge.net/projects/LunarCalendar/files/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb/download)
  + MD5SUM file: lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb.md5sum
    - [Download from github](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb.md5sum)
    - [Download from sourceforge](https://sourceforge.net/projects/LunarCalendar/files/v0.2.1/lunarcalendar_0.2.1_amd64_Ubuntu-18.04.deb.md5sum/download)

- windows: LunarCalendar-Setup-v0.2.1.exe, Windows installation package, support for Windows xp and above
    + [Download from github](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/LunarCalendar_Setup_v0.2.1.exe)
    + [Download from sourceforge](https://sourceforge.net/projects/lunarcalendar/files/v0.2.1/LunarCalendar_Setup_v0.2.1.exe/download)

- android
    + [LunarCalendar_v0.2.1.apk](https://github.com/KangLin/LunarCalendar/releases/download/v0.2.1/LunarCalendar_v0.2.1.apk)
    Android installation package
    
            adb install LunarCalendar_v0.2.1.apk

### Compile

- Depend
  + [Qt (LGPL v2.1)](http://qt.io/)
  + [RabbitCommon](https://github.com/KangLin/RabbitCommon)

  ```
  git clone https://github.com/KangLin/RabbitCommon.git
  ```

- Create and enter the build directory

        git clone --recursive https://github.com/KangLin/LunarCalendar.git
        cd LunarCalendar
        mkdir build

+ Use qmake

        cd build
        qmake ../LunarCalendar.pro RabbitCommon_DIR=
        make install

+ Use cmake
  - linux

        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install \
             -DCMAKE_BUILD_TYPE=Release \
             -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 \
             -DRabbitCommon_DIR=
        cmake --build . --config Release --target install
    
  - windows

        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install ^
               -DCMAKE_BUILD_TYPE=Release ^
               -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 ^
               -DRabbitCommon_DIR=
        cmake --build . --config Release --target install

  - Android
    - The host is linux
    
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release \
                 -DCMAKE_INSTALL_PREFIX=`pwd`/android-build \
                 -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
                 -DANDROID_ABI="armeabi-v7a with NEON" \
                 -DANDROID_PLATFORM=android-18 \
                 -DQt5_DIR= \
                 -DRabbitCommon_DIR= 
          cmake --build . --config Release --target install
          cmake --build . --target APK   
    
    - The host is windows
    
          cd build
          cmake .. -G"Unix Makefiles" ^
               -DCMAKE_BUILD_TYPE=Release ^
               -DCMAKE_INSTALL_PREFIX=`pwd`/android-build ^
               -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake ^
               -DCMAKE_MAKE_PROGRAM=${ANDROID_NDK}/prebuilt/windows-x86_64/bin/make.exe ^
               -DANDROID_PLATFORM=android-18 ^
               -DANDROID_ABI=arm64-v8a ^
               -DANDROID_ARM_NEON=ON ^
               -DQt5_DIR= ^
               -DRabbitCommon_DIR= 
          cmake --build . --config Release --target install
          cmake --build . --target APK     

    - Parameter Description: https://developer.android.google.cn/ndk/guides/cmake
      + ANDROID_ABI: The following values can be taken:
         Goal ABI. If the target ABI is not specified, CMake uses armeabi-v7a by default.
         Valid ABI are:
        - armeabi：CPU with software floating point arithmetic based on ARMv5TE
        - armeabi-v7a：ARMv7-based device with hardware FPU instructions (VFP v3 D16)
        - armeabi-v7a with NEON：Same as armeabi-v7a, but with NEON floating point instructions enabled. This is equivalent to setting -DANDROID_ABI=armeabi-v7a and -DANDROID_ARM_NEON=ON.
        - arm64-v8a：ARMv8 AArch64 Instruction Set
        - x86：IA-32 Instruction Set
        - x86_64 - x86-64 Instruction Set
      + ANDROID_NDK <path> The path of installed ndk in host
      + ANDROID_PLATFORM: For a full list of platform names and corresponding Android system images, see the [Android NDK Native API] (https://developer.android.google.com/ndk/guides/stable_apis.html)
      + ANDROID_ARM_MODE
      + ANDROID_ARM_NEON
      + ANDROID_STL: Specifies the STL that CMake should use. 
        - c++_shared: The shared library variant of libc++.
        - c++_static: The static library variant of libc++.
        - none: No C++ standard library suport.
        - system: The system STL
    
    - Install apk to devices
    
           adb install android-build-debug.apk 

- Note
    + windows
       If you build app. Qt does not provide openssl dynamic library for copyright reasons, so you must copy the dynamic library of openssl to the installation directory.
        - If it is 32, you can find the dynamic library of openssl (libeay32.dll, ssleay32.dll) in the Qt installer Tools\QtCreator\bin directory.
        - If it is 64-bit, you will need to download the binary installation package for openssl yourself.

    + linux

     ```
     sudo apt-get install libssl1.1
     ```

### Develop 
- Directory

```
  |-App                                        application source code
  |-Src                                        calendar library source code
     |- LunarCalendar.h                        calendar interface header file
         |- Resource
               |- database
                     |- chinese_holidays.sql   chinese holidays sql
```

### Other application use the libary

- Direct use the libary source code.
    + cmake
        - Submodule
            + add submodule:

                  git submodule add https://github.com/KangLin/LunarCalendar.git 3th_lib/LunarCalendar
                  git submodule update --init --recursive


            + Add follow code in CMakeLists.txt

                  add_subdirectory(3th_lib/LunarCalendar/Src)

        - No submodule
            + Download LunarCalendar source code from https://github.com/KangLin/LunarCalendar

                  git clone --recursive https://github.com/KangLin/LunarCalendar.git


            + Add follow code in CMakeLists.txt

                  set(LunarCalendar_DIR $ENV{LunarCalendar_DIR} CACHE PATH "Set LunarCalendar source code root directory.")
                  if(EXISTS ${LunarCalendar_DIR}/Src)
                      add_subdirectory(${LunarCalendar_DIR}/Src ${CMAKE_BINARY_DIR}/LunarCalendar)
                  else()
                      message("1. Please download LunarCalendar source code from https://github.com/KangLin/LunarCalendar")
                      message("   ag:")
                      message("       git clone https://github.com/KangLin/LunarCalendar.git")
                      message("2. Then set cmake value or environment variable LunarCalendar_DIR to download root dirctory.")
                      message("   ag:")
                      message(FATAL_ERROR "       cmake -DLunarCalendar_DIR= ")
                  endif()

            + Add libraries and include in CMakeLists.txt

                  SET(APP_LIBS ${PROJECT_NAME} ${QT_LIBRARIES})
                  if(TARGET LunarCalendar)
                      target_compile_definitions(${PROJECT_NAME}
                          PRIVATE -DLunarCalendar)
                      target_include_directories(${PROJECT_NAME}
                          PRIVATE "${LunarCalendar_DIR}/Src"
                          "${LunarCalendar_DIR}/Src/export")
                      set(APP_LIBS ${APP_LIBS} LunarCalendar)
                  endif()
                  target_link_libraries(${PROJECT_NAME} ${APP_LIBS})

- Use in library mode
    + Qt project file
        + Environment variable（LunarCalendar_DIR）or QMAKE parameters LunarCalendar_DIR Specify the location of the LunarCalendar install root directory, then add the following to the main project file (.pro):

                isEmpty(LunarCalendar_DIR): LunarCalendar_DIR=$$(LunarCalendar_DIR)
                !isEmpty(LunarCalendar_DIR): exists("$${LunarCalendar_DIR}/include/LunarCalendar.h"){
                     DEFINES += LunarCalendar
                     INCLUDEPATH *= $${LunarCalendar_DIR}/include $${LunarCalendar_DIR}/include/export
                     LIBS *= -L$${LunarCalendar_DIR}/lib -lLunarCalendar
                 } else{
                     message("1. Please download LunarCalendar source code from https://github.com/KangLin/LunarCalendar ag:")
                     message("   git clone https://github.com/KangLin/LunarCalendar.git")
                     message("2. Build and make install the project")
                     error("3. Then set value LunarCalendar_DIR to install dirctory")
                 }

    + cmake
        - Cmake parameter LunarCalendar_DIR specifies the installation root directory

                find_package(LunarCalendar)

        - Add libraries and include in CMakeLists.txt

                SET(APP_LIBS ${PROJECT_NAME} ${QT_LIBRARIES})
                if(LunarCalendar_FOUND)
                    target_compile_definitions(${PROJECT_NAME}
                                PRIVATE -DLunarCalendar)
                    target_include_directories(${PROJECT_NAME}
                                PRIVATE "${LunarCalendar_INCLUDE_DIRS}/Src"
                                        "${LunarCalendar_INCLUDE_DIRS}/Src/export")
                    set(APP_LIBS ${APP_LIBS} ${LunarCalendar_LIBRARIES})
                endif()
                target_link_libraries(${PROJECT_NAME} ${APP_LIBS})

- Load translator
    + Use libary function

            CLunarCalendar::InitResource();

    + Customize

            QString szPre;
            #if defined(Q_OS_ANDROID) || _DEBUG
                szPre = ":/Translations";
            #else
                szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
            #endif
            m_Translator.load(szPre + "/LunarCalendar_" + QLocale::system().name() + ".qm");
            qApp->installTranslator(&m_Translator);


### Contribution

- [Question](https://github.com/KangLin/LunarCalendar/issues)
- [Contributors](https://github.com/KangLin/LunarCalendar/graphs/contributors)

### [License Agreement](License.md "License.md")

Please follow the license agreement for the third-party libraries below and the license and thank the authors of the third-party libraries.

#### The third-party libraries

- [sxtwl_cpp](https://github.com/yuangu/sxtwl_cpp)
