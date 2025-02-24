- v1.0.5
  - 修复自动化编译打包没有打包库deb文件错误

- v1.0.4
  - 使用 [RabbitCommon v2.2.1](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.1)
  - 删除 vcpkg.json 中 baseline

- v1.0.3
  - 使用 [RabbitCommon v2.1.0](https://github.com/KangLin/RabbitCommon/releases/tag/v2.1.0)

- v1.0.2
  - 修改自动化编译

- v1.0.1
  - 删除了 debian 脚本
  - 修改 debian 打包
  - 重命名 RabbitCommon::CDownloadFile 为 RabbitCommon::CDownload

- v1.0.0
  - 删除：
    - CLunarCalendar::AddAnniversary
    - CLunarCalendar::AddHoliday
  - 由公有函数变为私有函数：
    - CLunarCalendar::InitResource
  - 使用节日数据库 SQL 文件
  - 修复当前选择字体反显颜色
  - 修改日历项目位置
  - 称动 Doxyfile.in -> Docs/doxygen/Doxyfile.in
  - 修复：
    - 详细提示错误
    - 中国节假日仅当阳历显示时才显示。任务为阳历或阴历显示时显示

- v0.3.2
  - 农历位置超过 3 个字符，则用 ... 表示
  - 修复任务颜色
  - 增加设置字体
  - 重构数据流程
  - 增加节日数据库 SQL 文件
  - 设置 ToolTip 显示长度(5)，超过则用 …… 表示
  - 增加废弃警告:
    - CLunarCalendar::AddAnniversary
    - CLunarCalendar::AddHoliday
  - 增加：
    - CLunarCalendar::EnableHolidays
    - CLunarCalendar::EnableSolarTerm
    - CLunarCalendar::SelectedSolarTerm

- v0.3.1
  - 自动化编译
    - 为 android 包增加签名

- v0.3.0
  - 修改　org.Rabbit.Calendar.desktop　中的 MimeType　值
  - 修改节日为列表
  - 修改农历假日为列表
  - 增加 ToolTipRole
  - 修改周年为列表
  - 重命名 YearShown 为 GetShowYear
  - 重命名 MonthShown 为 GetShowMonth
  - 增加 Qt 设置师插件
  - 增加 CPack 打包
  - 修复 pkg-config ，由 RabbitCommonUtils.cmake 中的 ADD_TARGET 产生
  - 更新 sxtwl 到 2
  
- v0.2.8
  - [增加 2024 年中国节假日](https://www.gov.cn/zhengce/content/202310/content_6911527.htm)
  - 在应用程序中增加日志菜单
  - 增加帮助文档 (doxygen)
  - 修改中国节假日中工作日颜色。用高显提示周未为工作日

- v0.2.7
  - 修复 DockDebugLog 崩溃. 详见: [RabbitCommon](https://github.com/KangLin/RabbitCommon/commit/af2e434e39af35876113436ead2a946efcf74199)

- v0.2.6
  - CI: 修改 android.yml
  - 使用 find_package(RabbitCommon)

- v0.2.5
  - 修复 NSIS 错误
  - 增加 find_package(RabbitCommon)

- v0.2.4
  - 删除 pro 管理项目
  - 使用 RabbitCommon v2

- v0.2.3
  - 修改自动编译

- v0.2.2
  - 修改图标主题位置
  - 修改自动化编译

- v0.2.1
  - [增加 2021 年中国节假日](https://www.gov.cn/gongbao/content/2020/content_5567750.htm)
  - [增加 2022 年中国节假日](http://www.gov.cn/zhengce/content/2021-10/25/content_5644835.htm)
  - [增加 2023 年中国节假日](http://www.gov.cn/zhengce/content/2022-12/08/content_5730844.htm)
  - 修改 github action
  - 在 Windows 下，安装 MSVC 系统库。详见: RabbitCommon
  - 修复除 0 引起的崩溃。影响前面所有发行版本。此错误是 RabbitCommon 引起的。
    详见：RabbitCommon: 7ed300a206d8a36c30003b6c19ad181682d0ff5f
  - 支持 Qt6
  - 更新文件下载
  - CI: 增加 github actions
  - 更新 RabbitCommon 更新软件
  - 使用图标样式

- v0.2.0
  - 修改 CI，增加修改日志

- v0.1.9
  - 修改ci
  - 修改关于对话框大小
  - 修改自动化编译，用 ubuntu 系统自带 Qt
  - 修改 2020 年春节假期

- v0.1.8
  - 修改中国假日数据库位置为用户数据库目录

- v0.1.7
  - 增加中国假日自动更新

- v0.1.6
  - 修改更新
  - 增加 android 签名

- v0.1.5
  - 修改数据库初始化BUG

- v0.1.4
  - 修改 cmake 编译 Android 版本
  - 增加 2020 年中国节假日

- v0.1.3
  - 增加中国法定假日

- v0.1.2
  - 修改今天按钮size

- v0.1.1
  - 增加 ANDROID 翻译资源
  - 增加触屏事件
  - 增加今天图标
  - 修复键盘改变日期BUG

- v0.1.0
  - 使用 RabbitCommon v1.0.0

- v0.0.13
  - 修改 install.sh

- v0.0.12
  - 修改更新功能

- v0.0.11
  - 仅测试 linux 下自动更新功能

- v0.0.10
  - 修改tar打包内容
  
- v0.0.9
  - 增加 update_linux_appimage.xml

- v0.0.8
  - 重新使用 v 前缀

- 0.0.7
  - 修改纪念日，任务提示，显示任务数
  
- 0.0.6
  - 增加 android 包
  
- 0.0.5
  - 增加linux tar包
  
- 0.0.4
  - 增月历背景图
  - 增加DEB打包
  - 增加 SetTaskHandle 接口
  - 增加工具位置设置
  - 增加 minimumSizeHint 
  
- v0.0.3
  - 增加周视图
  - 更新默认时间范围
  - 农历性能优化：增加农历缓存表

- v0.0.2
  - 支持 Android
  - 增加翻译资源
  - 增加关于
  - 增加自动更新
  - 增加日历类型
  
- v0.0.1
  - 中国农历
