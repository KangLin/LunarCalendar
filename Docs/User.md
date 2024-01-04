## 使用文档

### 直接用本项目源码   

#### cmake 工程可以直接用本项目源码

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

        if(NOT LunarCalendar_DIR)
            set(LunarCalendar_DIR $ENV{LunarCalendar_DIR} CACHE PATH "Set LunarCalendar source code root directory.")
        endif()
        if(EXISTS ${LunarCalendar_DIR}/Src)
            add_subdirectory(${LunarCalendar_DIR}/Src ${CMAKE_BINARY_DIR}/LunarCalendar)
        else()
            message("1. Please download LunarCalendar source code from https://github.com/KangLin/LunarCalendar")
            message("   ag:")
            message("       git clone https://github.com/KangLin/LunarCalendar.git")
            message("2. Then set cmake value or environment variable LunarCalendar_DIR to download root directory.")
            message("   ag:")
            message(FATAL_ERROR "       cmake -DLunarCalendar_DIR= ")
        endif()

  + 在使用者的工程目录 CMakeLists.txt 中添加

        if(TARGET LunarCalendar)
            target_link_libraries(${PROJECT_NAME} LunarCalendar)
        endif()

#### 以库方式使用使用

+ cmake
  cmake 参数 LunarCalendar_DIR 指定安装根目录

      find_package(LunarCalendar)
      
      if(LunarCalendar_FOUND)
          target_link_libraries(${PROJECT_NAME} LunarCalendar)
      endif()

## 在 Qt 设计器中使用
复制安装目录（bin、lib）下的所有动态库文件到 ${Qt_Install_Root}/plugins/designer 下。
就可以在 qt 设计器的控件窗口中看到此插件。然后就与使用普通Qt控件一样。
