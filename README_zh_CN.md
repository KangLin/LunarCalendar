## 农历

================================================

作者：康林（kl222@126.com)

[<img src="https://raw.githubusercontent.com/KangLin/Tasks/master/Resource/Image/English.png" alt="英语" title="英语" width="16" height="16" />英语](README.md)

------------------------------------------------

### 功能

Qt 写的农历库。带一个日历应用程序。

#### 支持以下平台
- Windows
- Linux、Unix
- Android
- Mac os
- IOS

------------------------------------------------

- [![Windows 编译状态](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/master)
- [![Linux 编译状态](https://travis-ci.org/KangLin/LunarCalendar.svg?branch=master)](https://travis-ci.org/KangLin/LunarCalendar)

------------------------------------------------
### 介绍

用Qt写的农历，支持QSS

------------------------------------------------

### 屏幕截图
![屏幕截图](Docs/image/ScreenShot.PNG "屏幕截图")
![屏幕截图](Docs/image/ScreenShotQss.PNG "屏幕截图")

------------------------------------------------

### 编译
- 建立并进入build目录

        git clone --recursive https://github.com/KangLin/LunarCalendar.git
        cd LunarCalendar
        mkdir build

- 编译
  + 用 qmake 

        cd build
        qmake ../LunarCalendar.pro
        make install
    
  + 用 cmake
  
        cd build
        cmake ..
        cmake --build .

------------------------------------------------

### 使用
- 直接用源码

        git submodule add https://github.com/KangLin/LunarCalendar.git 3th_libs/LunarCalendar

  + 是QT工程，则直接引入 LunarCalendar.pri

        include(LunarCalendar.pri)

  + cmake工程

        add_subdirectory(3th_libs/LunarCalendar/Src)
        
------------------------------------------------

### 下载
https://github.com/KangLin/LunarCalendar/releases/latest

------------------------------------------------

## 捐赠
- 捐赠(大于￥20)：  
![捐赠( 大于 ￥20 )](https://raw.githubusercontent.com/KangLin/Tasks/master/Src/Resource/image/Contribute.png "捐赠(大于￥20)")

- 捐赠￥20  
![捐赠￥20](https://raw.githubusercontent.com/KangLin/Tasks/master/Src/Resource/image/Contribute20.png "捐赠￥20")

------------------------------------------------

### [许可协议](License.md "License.md")

请遵守本协议和下列第三方库的许可协议，并感谢第三方库的作者。

#### 第三方库

- [寿星天文历](https://github.com/yuangu/sxtwl_cpp) 
