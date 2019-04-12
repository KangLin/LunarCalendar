## 农历

================================================

作者：康林（kl222@126.com)

[<img src="https://raw.githubusercontent.com/KangLin/Tasks/master/Resource/Image/English.png" alt="英语" title="英语" width="16" height="16" />英语](README.md)

------------------------------------------------

### 功能

Qt 写的农历。它提供：

- 一个Qt界面的农历日历库，支持 QSS (换肤功能）。
- 一个简单的日历应用程序。
- 此库的一个完整应用 [任务](https://github.com/KangLin/Tasks)

#### 支持以下平台
- [x] Windows
- [x] Linux、Unix
- [x] Android
- [ ] Mac os
- [ ] IOS

------------------------------------------------

- [![Windows 编译状态](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/master)
- [![Linux 编译状态](https://travis-ci.org/KangLin/LunarCalendar.svg?branch=master)](https://travis-ci.org/KangLin/LunarCalendar)

------------------------------------------------

### 屏幕截图
![屏幕截图](Docs/image/ScreenShot.PNG "屏幕截图")
![屏幕截图](Docs/image/ScreenShotQss.PNG "屏幕截图")
![Android 屏幕截图](Docs/image/ScreenShotAndroid.PNG "Android 屏幕截图")

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
  + 是QT工程
    - 子模块方式：
      + 增加子模块：
      
            git submodule add https://github.com/KangLin/LunarCalendar.git 3th_libs/LunarCalendar
      
      + 在工程文件(.pro)中直接引入 LunarCalendar.pri

            include(3th_libs/LunarCalendar/LunarCalendar.pri)

    - 非子模块方式：
      + 下载源码：https://github.com/KangLin/LunarCalendar
      + 在环境变量（LunarCalendar_DIR） 或 QMAKE参数 （LunarCalendar_DIR） 
        中指定 LunarCalendar 源码根目录的位置，然后在主工程文件（.pro）中加入下列：
    
            isEmpty(LunarCalendar_DIR): LunarCalendar_DIR=$$(LunarCalendar_DIR)
            !isEmpty(LunarCalendar_DIR): exists("$${LunarCalendar_DIR}/Src/LunarCalendar.pri"){
                DEFINES += LunarCalendar
                include($${LunarCalendar_DIR}/Src/LunarCalendar.pri)
            } else{
                message("1. Please download LunarCalendar source code from https://github.com/KangLin/LunarCalendar ag:")
                message("   git clone https://github.com/KangLin/LunarCalendar.git")
                message("2. Then set value LunarCalendar_DIR to download root dirctory")
            }
    
  + cmake工程

        add_subdirectory(3th_libs/LunarCalendar/Src)

- 加载翻译资源
  + 用库中提供的函数

        CLunarCalendar::InitTranslator();

  + 自定义
  
        QString szPre;    
        #if defined(Q_OS_ANDROID) || _DEBUG
            szPre = ":/Translations";
        #else
            szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
        #endif
        m_Translator.load(szPre + "/LunarCalendar_" + QLocale::system().name() + ".qm");
        qApp->installTranslator(&m_Translator);
        
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
