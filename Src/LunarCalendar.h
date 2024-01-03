/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef LUNARCALENDAR_H
#define LUNARCALENDAR_H

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QDate>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSharedPointer>
#include "lunarcalendar_export.h"

#if defined(__clang__) || defined(__GNUC__)
#define CPP_STANDARD __cplusplus
#elif defined(_MSC_VER)
#define CPP_STANDARD _MSVC_LANG
#endif
#if CPP_STANDARD >= 199711L
#define HAS_CPP_03 1
#endif
#if CPP_STANDARD >= 201103L
#define HAS_CPP_11 1
#endif
#if CPP_STANDARD >= 201402L
#define HAS_CPP_14 1
#endif
#if CPP_STANDARD >= 201703L
#define HAS_CPP_17 1
#endif

#if HAS_CPP_11
#include <functional>
#endif

/*!
 * \defgroup USE_DOC 使用文档
 * \details 使用文档
 * - \ref RoleDefinitions
 * - \ref API
 * - [使用文档](../Docs/User.md)
 * - \ref UssCLunarCalendar
 * 
 * \defgroup DEVELOPER_DOC 开发文档
 * \details 开发文档
 * - \ref RoleDefinitions
 * - [开发文档](../Docs/Developer.md)
 * - [使用文档](../Docs/User.md)
 * - \ref UssCLunarCalendar
 * - \ref API
 * - \ref INTERNAL_API
 * 
 * \defgroup API 应用程序接口
 * \details 应用程序接口
 * - \ref RoleDefinitions
 * - \ref UssCLunarCalendar
 * \ingroup USE_DOC DEVELOPER_DOC
 * 
 * \defgroup INTERNAL_API 内部使用接口
 * \details 内部使用接口
 * - \ref RoleDefinitions
 * \ingroup DEVELOPER_DOC
 */

/*!
 * \mainpage 农历日历
 * \details 一个 Qt 界面的农历日历库，支持 QSS (换肤功能），任务显示。
 * 
 * \section LunarUI 农历日历界面
 * 
 * - 日历
 * 
 *  |                  月视图                   |                 周视图               |
 *  |:----------------------------------------:|:-----------------------------------:|  
 *  |\image html Docs/image/ScreenShot.png     | \image html Docs/image/ViewWeek.png |
 *   
 * - 日历头
 * \image html Docs/image/Head.png
 * - 日期工具按钮
 * \image html Docs/image/Tool.png
 * - 今日工具按钮
 * \image html Docs/image/Tool.png
 * - 选择日期
 * \image html Docs/image/Date.png
 * - 当前时间
 * \image html Docs/image/Date.png
 * - 周
 * \image html Docs/image/Week.png
 * - 日历显示
 *   - 中国节假日，在左上角显示。
 *   - 上面中间圆点表示任务
 *   - 中间为阳历
 *   - 底下为农历、节气或是任务。 \ref HolidayPriority
 *     - 节日，放在农历位置上显示。最多显示最前一个。高亮加粗体
 *     - 节气
 *     - 周年纪念日、其它任务，在阳历上面中间用圆点表示。如果可能同时放在农历位置显示。
 *   - 详细提示（ToolTip）
 * \image html Docs/image/Task.png
 *
 * \section RoleDefinitions 角色定义
 * - 开发者(Developer)：开发本项目的人员
 * - 使用者(User)：使用本项目进行二次开发的人员
 * - 客户(Client)：使用本项目最终程序的人员
 *
 * \section Tasks 任务
 *
 * 任务可分为：
 *  - 年为周期。例如：节日；周年纪念日
 *  - 月为周期。例如：中国发工资；还贷款
 *  - 星期为周期。例如：开会；国外发工资
 *  - 其它周期性任务。例如：女性月经
 *  - 非周期性任务。例如：中国节假日（一群闲得蛋疼的人搞出来的玩意）
 *  - 单个任务
 *
 * 由于任务类型众多，所以本项目任务由使用者自行处理，
 * 参见： \ref UserDefinedTasks 。
 *
 * \subsection Holiday 节日、周年纪念日（以年为周期的任务）
 * - 中国节假日（一群闲得蛋疼的人搞出来的非周期性的玩意），在左上角显示。
 *   在项目内维护。详见： \ref ChineseHolidaysTable 。
 * - 节日。
 *   - 公历
 *   - 农历
 *   - 节气
 *
 *   在项目内维护。详见： \ref HolidaysTabl 。
 *   如果使用者不需要默认的，使用者也可以自己维护，
 *   \see \ref UserDefinedTasks CLunarCalendar::EnableHolidays
 *   CLunarCalendar::EnableSolarTerm 。
 * - 周年纪念日，在阳历上面中间用圆点表示。如果可能同时放在农历位置显示。
 *   参见： \ref UserDefinedTasks 
 *   - 公历
 *   - 农历
 *
 * \image html Docs/image/Task.png
 *
 * \subsection HolidayPriority 农历位置显示类型优先级
 * 
 * - 公历节日
 * - 农历节日
 * - 节气
 * - 周年纪念日
 * - 任务
 * - 农历
 *
 * \image html Docs/image/Task.png
 *
 * \subsection TaskInterfaces 任务接口
 * \subsubsection UserDefinedTasks 使用者自定义任务接口
 *
 * 只需要使用下列方法之一：
 *
 * - 使用 CLunarCalendar::SetTaskHandle() 处理自定义任务
 *
 *   - 声明 CLunarCalendar::CTaskHandler 的派生类
 *     \snippet App/MainWindow.h Define CTaskHandler derived class
 *   - 实现 CLunarCalendar::CTaskHandler::onHandle 处理函数
 *     \snippet App/MainWindow.cpp Implement the onHandle function
 *     **注意：** 
 *     - 处理函数应尽快返回。不要在函数中做过多复杂的处理。防止阻塞 UI 线程。
 *     - 如果不需要在任务显示在农历位置，则只要返回任务数。不要设置 tasks 参数。
 *     - 如果需要任务显示在农历位置，则需要设置 tasks 参数。最前面的一个显示。
 *     参见： \ref HolidayPriority
 *   - 定义变量
 *     \snippet App/MainWindow.h Defined CHandler variable
 *   - 实例化对象
 *     \snippet App/MainWindow.cpp Instance CHandler
 *   - 用 CLunarCalendar::SetTaskHandle 设置处理类
 *     \snippet App/MainWindow.cpp Set user defined tasks with CTaskHandler
 *
 * - 使用 CLunarCalendar::SetTaskHandle(std::function<uint(const QDate& date, QStringList& tasks)> cbHandler) 处理自定义任务。
 * 需要标准C++11及以后才支持。
 *
 *   \snippet App/MainWindow.cpp User defined tasks
 *
 * **注意：** 
 * - 回调函数应尽快返回。不要在回调函数中做过多复杂的处理。防止阻塞 UI 线程。
 * - 如果不需要在任务显示在农历位置，则只要返回任务数。不要设置 tasks 参数。
 * - 如果需要任务显示在农历位置，则需要设置 tasks 参数。最前面的一个显示。
 *   参见： \ref HolidayPriority
 *
 * \section 文档
 * - [使用文档](../Docs/User.md)
 * - [开发文档](../Docs/Developer.md)
 * - [主题](topics.html)
 * - [主页](https://github.com/KangLin/LunarCalendar)
 * - [说明](../README_zh_CN.md)
 * - \ref Example
 */

/*!
 * \brief 农历日历类
 * \details 显示阳历、农历、节日、任务
 * \ingroup API
 * 
 * \section UssCLunarCalendar CLunarCalendar 类的使用
 * 
 * - 初始化资源
 * \snippet App/main.cpp CLunarCalendar::InitResource()
 * - 释放资源
 * \snippet App/main.cpp CLunarCalendar::CLeanResource()
 * - 实例化 CLunarCalendar 对象
 * \snippet App/MainWindow.h Instance CLunarCalendar
 * \snippet App/MainWindow.cpp Instance CLunarCalendar
 * - [可选]设置界面 
 * \snippet App/MainWindow.cpp Set UI
 * - [可选]设置自定义任务  
 *   使用下列方法之一：
 *   - 使用 CLunarCalendar::SetTaskHandle(std::function<uint(const QDate& date, QStringList& tasks)> cbHandler) 处理自定义任务。 需要标准C++11及以后才支持。
 *   请遵守 \ref HolidayPriority 。
 *     \snippet App/MainWindow.cpp User defined tasks
 *   - 使用 CLunarCalendar::SetTaskHandle() 处理自定义任务。
 *     请遵守 \ref HolidayPriority 。
 *     - 定义 CTaskHandler 的派生类
 *       \snippet App/MainWindow.h Define CTaskHandler derived class
 *     - 实现 onHandle 处理函数
 *       \snippet App/MainWindow.cpp Implement the onHandle function
 *     - 定义 CHandler 变量
 *       \snippet App/MainWindow.h Defined CHandler variable
 *     - 实例化 CHandler
 *       \snippet App/MainWindow.cpp Instance CHandler
 *     - 用 SetTaskHandle 设置处理类
 *       \snippet App/MainWindow.cpp Set user defined tasks with CTaskHandler
 * - [可选]处理选择事件
 *   - 连接选择信号
 *     \snippet App/MainWindow.cpp sigSelectionChanged
 *   - 处理选择事件
 *     \snippet App/MainWindow.cpp slotUpdateCalendar
 * - [可选] 允许节日、节气
 * \snippet App/MainWindow.cpp Enable holidays and solar term
 */
class LUNARCALENDAR_EXPORT CLunarCalendar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QDate selectedDate WRITE SetSelectedDate READ SelectedDate)
    Q_PROPERTY(QString selectLunar READ SelectedLunar)
    Q_PROPERTY(int showYear READ GetShowYear)
    Q_PROPERTY(int showMonth READ GetShowMonth)
    //Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ FirstDayOfWeek WRITE SetFirstDayOfWeek)
    Q_PROPERTY(QDate minimumDate READ MinimumDate WRITE SetMinimumDate)
    Q_PROPERTY(QDate maximumDate READ MaximumDate WRITE SetMaximumDate)

public:
    explicit CLunarCalendar(QWidget *parent = nullptr);
    virtual ~CLunarCalendar() override;
    
    enum class _CalendarType;
    
    //! \name 资源操作
    //! @{
    /*!
     * \brief 初始化程序资源，仅在在程序开始处, QApplication a(argc, argv); 之后调用一次.
     * \snippet App/main.cpp CLunarCalendar::InitResource()
     */
    static void InitResource();
    /*!
     * \brief 释放程序资源。仅在程序退出前调用一次
     * \snippet App/main.cpp CLunarCalendar::CLeanResource()
     */
    static void CLeanResource();
    //! @} 资源操作
    
    //! \name 日期操作
    //! @{
    
    /*!
     * \brief 得到当前选择的日期。
     *        当前选择的日期在指定的最小日期 MinimumDate() 与最大日期 MaximumDate() 的范围内。
     *        默认为系统当前日期。
     * \see MinimumDate()
     * \see MaximumDate()
     */
    QDate SelectedDate() const;
    /*!
     * \brief 设置当前选择的日期
     * \param date: 将选择的日期。
     * \note 选择的日期必须在指定的最小日期 MinimumDate() 与最大日期 MaximumDate() 的范围内。
     *       如果超过范围。则会自动调整到范围内。
     * \param bForce:
     *          - true: 更新模型中的日期
     *          - false: 当前设置的日期与模型中的日期不相同时，才更新模型中的日期
     * \see SetDateRange()
     * \see SetMinimumDate() MinimumDate()
     * \see SetMaximumDate() MaximumDate()
     */
    void SetSelectedDate(const QDate &date, bool bForce = false);
    /*!
     * \brief 得到当前选择的日期的农历的字符串
     */
    QString SelectedLunar() const;
    /*!
     * \brief 得到当前选的节气
     * \return 如果有，则返回节气。否则返回空。
     */
    QString SelectedSolarTerm() const;
    /*!
     * \brief 得到当前选择的日期的农历
     * \param year 农历年
     * \param month 农历月
     * \param day 农历日
     */
    int SelectedLunar(/*out*/int &year, /*out*/int &month, /*out*/int &day);
    /*!
     * \brief 得到给定日期的农历（公历转农历）
     * \param date: 日期
     * \param year: 农历年
     * \param month: 农历月
     * \param day: 农历日
     * \return 
     */
    static int GetLunar(/*in*/const QDate date, /*out*/int &year, /*out*/int &month, /*out*/int &day);
    
    //! @} 日期操作
    
    //! \name 节日、周年纪念日、任务操作
    //! @{
    
public Q_SLOTS:
    /*!
     * \brief 允许或禁用节日
     * \param bUse
     * \return 
     */
    bool EnableHolidays(bool bEnable = true);
    /*!
     * \brief 允许或禁用节气
     * \param bEnable
     * \return 
     */
    bool EnableSolarTerm(bool bEnable = true);

public:
    /*!
     * \ref UserDefinedTasks 类
     * \details
     * 用法：
     * - 定义其派生类
     * \snippet App/MainWindow.h Define CTaskHandler derived class
     * - 实现接口 CTaskHandler::onHandle
     * \snippet App/MainWindow.cpp Implement the onHandle function
     * - 定义变量
     * \snippet App/MainWindow.h Defined CHandler variable
     * - 实例化对象
     * \snippet App/MainWindow.cpp Instance CHandler
     * - 用 SetTaskHandle 设置处理类
     * \snippet App/MainWindow.cpp Set user defined tasks with CTaskHandler
     * 
     * \see SetTaskHandle \ref UserDefinedTasks \ref HolidayPriority
     */
    class CTaskHandler
    {
    public:
        CTaskHandler() {}
        virtual ~CTaskHandler(){}
        /*!
         * \ref UserDefinedTasks 。请遵守 \ref HolidayPriority 。
         * \param date: 日期
         * \param tasks: 任务列表。如果使用者有新任务，并需要在农历位置处显示内容，则加入到此列表中。
         *          \note
         *            - 加入空字符或"":表示只显示圆点，不显示内容。
         *            - 不设置此值。只返回任务数。表示只显示圆点，不显示内容。
         *            - 请遵守 \ref HolidayPriority 。
         * \return 未加入到 tasks 的任务数。不包括 tasks 中的任务数。
         * \note 在 tasks 中增加的新任务，则不计入返回值中，
         *       如果没有其它的未加入 tasks 的任务，则返回 0。
         *       否则返回未加入到 tasks 中的任务数。
         * \details 例子：
         * \snippet App/MainWindow.cpp Implement the onHandle function
         * \see \ref UserDefinedTasks \ref HolidayPriority SetTaskHandle
         *
         * \image html Docs/image/Task.png
         */
        virtual uint onHandle(/*in*/const QDate& date,
                              /*out*/QStringList& tasks) = 0;
    };
    /*!
     * \brief 设置 \ref UserDefinedTasks
     * \param handler 任务处理类（CTaskHandler）
     * \details 使用方法：
     * - 声明 CTaskHandler 派生类
     * \snippet App/MainWindow.h Define CTaskHandler derived class
     * - 实现接口 CTaskHandler::onHandle
     * \snippet App/MainWindow.cpp Implement the onHandle function
     * - 定义变量
     * \snippet App/MainWindow.h Defined CHandler variable
     * - 实例化对象
     * \snippet App/MainWindow.cpp Instance CHandler
     * - 用 SetTaskHandle 设置处理类
     * \snippet App/MainWindow.cpp Set user defined tasks with CTaskHandler
     *
     * \see \ref UserDefinedTasks \ref HolidayPriority CTaskHandler CTaskHandler::onHandle
     *
     * \image html Docs/image/Task.png
     */
    int SetTaskHandle(QSharedPointer<CTaskHandler> handler);

#if HAS_CPP_11
    /*!
     * 设置 \ref UserDefinedTasks
     *
     * \param date: 要处理的日期
     *  \param tasks: 任务列表。如果使用者有新任务，并需要在农历位置处显示内容，则加入到此列表中。
     *  \note 
     *    - 加入空字符或"":表示只显示圆点，不显示内容。
     *    - 不设置此值。只返回任务数。表示只显示圆点，不显示内容。
     *    - 请遵守 \ref HolidayPriority 。
     *  \return 未加入到 tasks 的任务数。不包括 tasks 中的任务数。
     *  \note 在 tasks 中增加的新任务，则不计入返回值中，
     *       如果没有其它的未加入 tasks 的任务，则返回 0。
     *       否则返回未加入到 tasks 中的任务数。
     *
     * \details 例子：
     * \snippet App/MainWindow.cpp User defined tasks
     *
     * \note 需要 c++ 标准 11
     * 
     * \see 
     * - \ref UserDefinedTasks
     * - \ref HolidayPriority
     *
     * \image html Docs/image/Task.png
     */
    virtual int SetTaskHandle(
        std::function<uint(/*in*/const QDate& date,
                           /*out*/QStringList& tasks)> cbHandler);
#endif
    
    //! @} 节日、周年纪念日、任务操作
    
    //! \name 日期操作
    //! @{
    
    /*!
     * \brief 得到日历的支持最小日期
     */
    QDate MinimumDate() const;
    /*!
     * \brief 设置日历的支持最小日期
     */
    void SetMinimumDate(const QDate &date);
    /*!
     * \brief 得到日历的支持最大日期
     */
    QDate MaximumDate() const;
    /*!
     * \brief 设置日历的支持最大日期
     */
    void SetMaximumDate(const QDate &date);
    /*!
     * \brief 设置日历的支持日期的范围
     */
    void SetDateRange(const QDate &min, const QDate &max);
    
    /*!
     * \brief 得到当前界面显示的年份
     * \return 显示的年份
     */
    int GetShowYear() const;
    /*!
     * \brief 得到当前界面显示的月份
     * \return 得到显示的月份
     */
    int GetShowMonth() const;
    /*!
     * \brief 得到年周从哪天开始
     * \return 
     */
    Qt::DayOfWeek FirstDayOfWeek() const;
    //void SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);
    
    //! @} 日期操作
    
    //! \name 设置界面
    //! @{
    
    enum class _HEAD_position {
        Not = 0,   //!< 无
        Top,       //!< 上
        Down,      //!< 下
        Left,      //!< 左
        Right      //!< 右
    };
    /*!
     * \brief 设置日历头的位置
     * \param pos
     * \return 
     */
    int SetHeadposition(_HEAD_position pos = _HEAD_position::Top);
    
public Q_SLOTS:
    
    /*!
     * 显示或隐藏日历头
     * \image html Docs/image/Head.png
     */
    void ShowHead(bool bShow);
    /*!
     * 显示或隐藏日期工具按钮
     * \image html Docs/image/Tool.png
     */
    void ShowTools(bool bShow);
    /*!
     * 显示或隐藏今日工具按钮
     * \image html Docs/image/Tool.png
     */
    void ShowToday(bool bShow);
    /*!
     * 显示或隐藏选择日期
     * \image html Docs/image/Date.png
     */
    void ShowDate(bool bShow);
    /*!
     * 显示或隐藏当前时间
     * \image html Docs/image/Date.png
     */
    void ShowTime(bool bShow);
    
    /*!
     * 显示或隐藏周
     * \image html Docs/image/Week.png
     */
    void ShowWeekHead(bool bShow);
    /*!
     * 显示或隐藏第几周
     * \image html Docs/image/Week.png
     */
    void ShowWeeks(bool bShow);
    
    /*!
     * \brief 显示或隐藏网格
     */
    void ShowGrid(bool show);

    /*!
     * \brief 显示或隐藏工具提示
     */
    void EnableToolTip(bool show);
    
    /*!
     * \brief 显示或隐藏背景图片
     */
    void ShowBackgroupImage(bool show);
    
public:
    
    //! 日历类型
    enum class _CalendarType {
        CalendarTypeSolar = 0x01,  //!< 阳历
        CalendarTypeLunar = 0x02   //!< 农历
    };
    //! 设置日历类型（组合）
    int SetCalendarType(_CalendarType type);
    //! 得到日历类型
    _CalendarType GetCalendarType() const;
    
    //! 视图类型
    enum class _VIEW_TYPE {
        ViewTypeMonth,  //!< 月
        ViewTypeWeek    //!< 周
    };
    //!　设置视图类型
    int SetViewType(_VIEW_TYPE type);
    //! 得到视图类型
    _VIEW_TYPE GetViewType() const;
    
    /*!
     * \brief 更新数据和视图
     * \return 
     */
    int Update();

    void setFont(const QFont& font);

    //! @} 设置界面

    enum class _TOUCH_UP_DOWN_FUNCTION {
        TouchChangeView,
        TouchChangeMounth
    };
    int SetTouchUpDownFunction(_TOUCH_UP_DOWN_FUNCTION f);
    
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
    
    //! \name 下列功能仅由开发者使用。客户不要使用。
    //! \ref RoleDefinitions
    //! @{
    
    /*!
     * \brief 从缓存文件中得到农历
     * \param szFile: 缓存文件
     * \return 成功返回　0　；其它失败
     * \note 仅由开发者使用。普通用户不要使用。
     * \see CLunarTable
     */
    int LoadCalendarTable(const QString& szFile);
    /*!
     * \brief 产生日历缓存表
     * \param szFile: 缓存文件
     * \param nThreadNumber: 产生的线程数
     * \param bClearCache: 是否清除以前的缓存
     * \param bSaveAllDate: 是否保存所有日期
     * \return 成功返回 0　；其它失败
     * \note 仅由开发者使用。普通用户不要使用。
     * \see CLunarTable
     */
    int GenerateCalendarTable(const QString& szFile,
                              int nThreadNumber = 2,
                              bool bClearCache = false,
                              bool bSaveAllDate = true);
    
    //! @}  上面功能仅由开发者使用。客户不要使用。

Q_SIGNALS:
    /*!
     * \brief 当前选择日期改变时触发
     * 可以在相应的槽函数中调用下列函数得到选择的日期：
     *  - SelectedDate()
     *  - SelectedLunar()
     *  - SelectedLunar(int &year, int &month, int &day) 
     */
    void sigSelectionChanged();
    
private Q_SLOTS:
    void soltShowToday();
    
private slots:
    void on_tbNextYear_clicked();
    void on_tbPreviousYear_clicked();
    void on_tbNextMonth_clicked();
    void on_tbPreviousMonth_clicked();
    void on_cbMonth_currentIndexChanged(int index);
    void on_cbYear_currentIndexChanged(int index);
    void on_tvMonth_pressed(const QModelIndex &index);
    
    void slotTimeout();
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    
private:
    int ShowSelectTitle();
    int UpdateViewModel(bool bForce = false);
    int UpdateMonthMenu();
    int EnableMonthMenu();
    int SetBackgroup(const QString& szFile);
    int SetYearRange(int min, int max);
    
private:
    QComboBox m_cmbYear;
    QToolButton m_tbPreYear, m_tbNextYear;
    QComboBox m_cmbMonth;
    QToolButton m_tbPreMonth, m_tbNextMonth;
    QPushButton m_pbToday;
    QLabel m_lbDate, m_lbTime;
    QTableView m_View;
    
    QHBoxLayout* m_pToolLayout;
    QVBoxLayout* m_pHeadLayout;
    QGridLayout* m_pMainLayout;
    _HEAD_position m_Headposition;
    
    int m_oldRow, m_oldCol;
    bool m_bShowToday;
    QTimer m_Timer;
    bool m_bShowBackgroupImage;
    
    _TOUCH_UP_DOWN_FUNCTION m_TouchFunction;
    
};

#endif // LUNARCALENDAR_H
