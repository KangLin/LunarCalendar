## LunarCalendar

=========================================================

Author: KangLin (kl222@126.com)

[<img src="https://github.com/KangLin/Tasks/blob/master/Resource/Image/china.png" alt="Chinese" title="Chinese" width="16" height="16"/>Chinese](README_zh_CN.md)
------------------------------------------------

### Features

Use QT write lunar calendar.


- Windows
- Linux、Unix
- Android
- Mac os
- IOS

------------------------------------------------

- [![Windows Build status](https://ci.appveyor.com/api/projects/status/p5vhmmbuql9fyfpl/branch/master?svg=true)](https://ci.appveyor.com/project/KangLin/lunarcalendar/branch/master)
- [![Linux Build Status](https://travis-ci.org/KangLin/LunarCalendar.svg?branch=master)](https://travis-ci.org/KangLin/LunarCalendar)

------------------------------------------------
### Introduction

Lunar calendar written in Qt, support QSS

------------------------------------------------

### Screenshots
![Screenshots](Docs/image/ScreenShot.PNG "Screenshots")
![Screenshots](Docs/image/ScreenShotQss.PNG "Screenshots")

------------------------------------------------

### Compile
- Create and enter the build directory

        git clone --recursive https://github.com/KangLin/LunarCalendar.git
        cd LunarCalendar
        mkdir build

+ Use qmake 

      cd build
      qmake ../LunarCalendar.pro
      make install
  
+ Use cmake

      cd build
      cmake ..
      cmake --build .

------------------------------------------------
### Use
- Direct source code.

        git submodule add https://github.com/KangLin/LunarCalendar.git 3th_libs/LunarCalendar

  + Is a QT project, directly introduces LunarCalendar.pri

         Include(LunarCalendar.pri)

  + cmake project

        add_subdirectory(3th_libs/LunarCalendar/Src)
        
------------------------------------------------

### Download
https://github.com/KangLin/LunarCalendar/releases/latest

------------------------------------------------

## Donation
- Donation (greater than ¥20):  
![donation (greater than ¥20)](https://raw.githubusercontent.com/KangLin/Tasks/master/Src/Resource/image/Contribute.png "donation (greater than ¥20)")

- Donate ¥20  
![donation ¥20](https://raw.githubusercontent.com/KangLin/Tasks/master/Src/Resource/image/Contribute20.png "donation ¥20")

------------------------------------------------

### [License Agreement](License.md "License.md")

Please follow the license agreement for the third-party libraries below and the license and thank the authors of the third-party libraries.

#### The third-party libraries

- [sxtwl_cpp](https://github.com/yuangu/sxtwl_cpp)
