农历
================================================

作者：康林（kl222@126.com)

[<img src="https://raw.githubusercontent.com/KangLin/Tasks/master/Resource/Image/English.png" alt="英语" title="英语" width="16" height="16" />英语](README.md)

- [![Windows 编译状态](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/master)
- [![Linux 编译状态](https://travis-ci.org/KangLin/LunarCalendar.svg?branch=master)](https://travis-ci.org/KangLin/LunarCalendar)

------------------------------------------------

- [功能](#功能)
- [捐赠](#捐赠)
- [屏幕截图](#屏幕截图)
- [下载安装包](#下载安装包)
    + [linux](#linux)
    + [ubuntu](#ubuntu)
    + [windows](#windows)
    + [android](#android)
- [编译](#编译)
- [其它应用使用本项目](#其它应用使用本项目)
- [贡献](#贡献)
- [许可协议](#许可协议)
    + [第三方库](#第三方库)

------------------------------------------------

### 功能

Qt 写的农历。它提供：

- 一个Qt界面的农历日历库，支持 QSS (换肤功能）。
  + 阳历
  + 农历
  + 阳历和农历节日
  + 阳历和农历纪念日（例如：生日，结婚纪念日等）
  + 月视图
  + 周视图
- 一个简单的日历应用程序。用于示例如何使用此农历日历库。
- 此库的一个完整应用 [任务](https://github.com/KangLin/Tasks)
- 跨平台，支持多操作系统
  + [x] Windows
  + [x] Linux、Unix
  + [x] Android
  + [ ] Mac os
  + [ ] IOS

Mac os 和 IOS ，本人没有相应设备，请有相应设备的同学自己编译，测试。

## 捐赠
- 捐赠(大于￥20)：  
![捐赠( 大于 ￥20 )](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠(大于￥20)")

- 捐赠￥20  
![捐赠￥20](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute20.png "捐赠￥20")

------------------------------------------------

### 屏幕截图
- Windows

![屏幕截图](Docs/image/ScreenShot.PNG "屏幕截图")
![屏幕截图](Docs/image/ScreenShotQss.PNG "屏幕截图")

- Android

![Android 屏幕截图](Docs/image/ScreenShotAndroid.PNG "Android 屏幕截图")

------------------------------------------------

### [下载安装包](https://github.com/KangLin/LunarCalendar/releases/latest)

- linux
    + [LunarCalendar_v0.0.13.tar.gz](https://github.com/KangLin/LunarCalendar/releases/download/v0.0.13/LunarCalendar_v0.0.13.tar.gz)  
    AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/
    用法：
       1. 解压。把 LunarCalendar_v0.0.13.tar.gz 复制到你想安装的位置，并解压：

                mkdir LunarCalendar
                cd LunarCalendar
                cp $DOWNLOAD/LunarCalendar_v0.0.13.tar.gz .
                tar xvfz LunarCalendar_v0.0.13.tar.gz

       2. 运行 install.sh install 安装

                ./install.sh install


       3. 如果要卸载，运行 install.sh remove

                ./install.sh remove

- ubuntu
    + [lunarcalendar_0.0.13_amd64.deb](https://github.com/KangLin/LunarCalendar/releases/download/v0.0.13/lunarcalendar_0.0.13_amd64.deb)
    deb 安装包,可用于 Ubuntu
  
- windows
    + [LunarCalendar-Setup-v0.0.13.exe](https://github.com/KangLin/LunarCalendar/releases/download/v0.0.13/LunarCalendar-Setup-v0.0.13.exe)  
    Windows安装包，支持 Windows xp 以上系统 

- android
    + [android-build-debug.apk](https://github.com/KangLin/LunarCalendar/releases/download/v0.0.13/android-build-debug.apk)  
    Android 安装包
    
------------------------------------------------

- 目录结构
```
    |-App                  应用程序代码
    |-Src                  日历库代码
       |- LunarCalendar.h  日历库接口头文件
```

------------------------------------------------
### 编译
- 依赖  
  [RabbitCommon](https://github.com/KangLin/RabbitCommon) 
  
        git clone https://github.com/KangLin/RabbitCommon.git

- 建立并进入build目录

        git clone --recursive https://github.com/KangLin/LunarCalendar.git
        cd LunarCalendar
        mkdir build

- 编译
  + 用 qmake 

        cd build
        qmake ../LunarCalendar.pro RabbitCommon_DIR=
        make install
    
  + 用 cmake
  
        cd build
        cmake .. -DQt5_DIR=${QT_ROOT}/lib/cmake/Qt5 -DRabbitCommon_DIR=
        cmake --build .

- 安装注意  
Qt因为版权原因，没有提供openssl动态库，所以必须自己复制openssl的动态库到安装目录下。
    + windows
        - 如果是32的，可以在Qt安装程序Tools\QtCreator\bin目录下，找到openssl的动态库（libeay32.dll、ssleay32.dll）
        - 如果是64位，则需要自己下载openssl的二进制安装包。
    + linux

    
        ```
        sudo apt-get install libssl1.1
        ```

------------------------------------------------

### 其它应用使用本项目
- 直接用本项目源码   
  + cmake工程
    - 子模块方式
      + 增加子模块：
    
            git submodule add https://github.com/KangLin/LunarCalendar.git 3th_lib/LunarCalendar
            git submodule update --init --recursive
      
      + 在 CMakeLists.txt 中以子目录方式加入
      
            add_subdirectory(3th_lib/LunarCalendar/Src)
            
    - 非子模块方式
      + 下载源码：https://github.com/KangLin/LunarCalendar
    
            git clone --recursive https://github.com/KangLin/LunarCalendar.git
       
       + 引入以 add_subdirectory 本项目录
          
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
                
       + 在使用的工程目录CMakeLists.txt
          
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

- 以库方式使用使用
  + Qt 工程文件
     + 在环境变量（LunarCalendar_DIR）或 QMAKE参数 （LunarCalendar_DIR） 
        中指定 LunarCalendar 库安装根目录的位置，然后在主工程文件（.pro）中加入下列：
    
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
    cmake 参数 LunarCalendar_DIR 指定安装根目录

          find_package(LunarCalendar)
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

- 加载翻译资源
  + 用库中提供的函数

        CLunarCalendar::InitResource();

------------------------------------------------

### 贡献

问题：https://github.com/KangLin/LunarCalendar/issues  
项目位置：https://github.com/KangLin/LunarCalendar

------------------------------------------------

### [许可协议](License.md "License.md")

请遵守本协议和下列第三方库的许可协议，并感谢第三方库的作者。

#### 第三方库

- [寿星天文历](https://github.com/yuangu/sxtwl_cpp) 
