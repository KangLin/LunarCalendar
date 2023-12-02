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
 * - \ref UssCLunarCalendar
 * - [使用文档](../Docs/User.md)
 * 
 * \defgroup DEVELOPER_DOC 开发文档
 * \details 开发文档
 * - \ref RoleDefinitions
 * - \ref UssCLunarCalendar
 * - \ref API
 * - \ref INTERNAL_API
 * - [开发文档](../Docs/Developer.md)
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
 *  |\image html Docs/image/ScreenShotUbunt.png| \image html Docs/image/ViewWeek.png |
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
 * 
 * \section RoleDefinitions 角色定义
 * - 开发者(Developer)：开发本项目的人员
 * - 使用者(User)：使用本项目进行二次开发的人员
 * - 客户(Client)：使用本项目最终程序的人员
 *
 * \section Tasks 任务
 * 
 * 任务可分为：
 *  - 年为周期。例如：节日；纪念日
 *  - 月为周期。例如：中国发工资；还贷款
 *  - 星期为周期。例如：开会；国外发工资
 *  - 以及其它周期性任务
 *  - 单个任务
 *
 * 由于任务类型众多，所以本项目只处理以年为周期的任务。其它类型任务由使用者自行处理。
 *
 * \subsection Holiday 节日（以年为周期的任务）
 * - 以年为周期的任务
 *   - 中国节假日，在左上角显示
 *   - 节日
 *     - 公历： CLunarCalendar::AddHoliday
 *     - 农历
 *     - 节气
 *   - 周年纪念日，在阳历上面中间用圆点表示。如果可能同时放在农历位置显示。
 *     - 公历: CLunarCalendar::AddAnniversary
 *     - 农历: CLunarCalendar::AddLunarAnniversary
 *
 * \subsection TasksDisplay 任务显示
 * - 中国节假日，在左上角显示。
 * - 节日，放在农历位置上显示。最多显示最前一个。高亮加粗体
 * - 周年纪念日、其它任务，在阳历上面中间用圆点表示。如果可能同时放在农历位置显示。
 *
 * \subsubsection HolidayPriority 农历位置显示优先级
 *   - 公历节日
 *   - 农历节日
 *   - 节气
 *   - 周年纪念日
 *     - 公历
 *     - 农历
 *   - 任务
 *   - 农历
 *
 * \image html Docs/image/Holiday.png
 *
 * \subsection UserDefinedTasks 使用者自定义任务
 * 
 * 使用 CLunarCalendar::SetTaskHandle 处理自定义任务
 * 
 * \snippet App/MainWindow.cpp User defined tasks
 * 
 * \section 文档
 * - \ref UssCLunarCalendar
 * - [开发文档](Modules.html)
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
 * - [可选]设置周年纪念日
 * \snippet App/MainWindow.cpp Add Anniversary
 * - [可选]设置自定义任务
 * \snippet App/MainWindow.cpp User defined tasks
 * - [可选]处理选择事件
 *   - 连接选择信号
 *     \snippet App/MainWindow.cpp sigSelectionChanged
 *   - 处理选择事件
 *     \snippet App/MainWindow.cpp slotUpdateCalendar
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
     * \brief 得到当前选择的日期的农历
     */
    QString SelectedLunar() const;
    //TODO:int SelectedLunar(int &year, int &month, int &day);
    /*!
     * \brief 得到给定日期的农历（公历转农历）
     * \param date: 日期
     * \param year: 农历年
     * \param month: 农历月
     * \param day: 农历日
     * \return 
     */
    static int GetLunar(const QDate date, int &year, int &month, int &day);
    
    //! @} 日期操作
    
    //! \name 节日、周年纪念日、任务操作
    //! @{
    
    /*!
     * \brief 增加公历假日
     * \details
     * \snippet App/MainWindow.cpp Add Holiday
     * \param month: 节日月份
     * \param day: 节日日期
     * \param szName: 节日名。不能为空或""
     * \image html Docs/image/Holiday.png
     */
    int AddHoliday(int month, int day, const QString &szName);
    /*!
     * \brief 设置周年纪念日（例如：公历生日）
     * \param month: 月
     * \param day: 日
     * \param szName: 纪念日名。不能为空或""
     * \image html Docs/image/Holiday.png
     */
    int AddAnniversary(int month, int day, const QString &szName);
    /*!
     * \brief 设置农历周年纪念日（例如：农历生日）
     * \param month: 农历月
     * \param day: 农历日
     * \param szName: 纪念日名。不能为空或""
     * \image html Docs/image/Holiday.png
     */
    int AddLunarAnniversary(int month, int day, const QString &szName);
    
    /*!
     * 自定义任务处理类
     * \see SetTaskHandle
     */
    class CGetTaskHandler
    {
    public:
        CGetTaskHandler() {}
        virtual ~CGetTaskHandler(){}
        /*!
         * 处理自定义任务
         * \param date: 日期
         * \param tasks: 任务列表。如果使用者有新任务，则加入到些列表中。
         *          \note tasks 加入空字符或""。表示只显示圆点，不显示内容。
         * \return 任务数。目前忽略
         */
        virtual uint onHandle(/*in*/const QDate& date, /*out*/QStringList& tasks) = 0;
    };
    /*!
     * \brief 处理使用者自定义任务
     * \param handler
     * \return 
     * \see CGetTaskHandler
     */
    int SetTaskHandle(QSharedPointer<CGetTaskHandler> handler);

#if HAS_CPP_11
    /*!
     * 处理使用者自定义任务
     *
     * \param cbHandler: 处理函数
     *      \param date: 要处理的日期
     *      \param tasks: 任务列表。如果使用者有新任务，则加入到些列表中。
     *          \note tasks 加入空字符或""。表示只显示圆点，不显示内容。
     *      \return 任务数。目前忽略
     * 
     * \snippet App/MainWindow.cpp User defined tasks
     * \note It is need c++ standard 11
     */
    virtual int SetTaskHandle(std::function<uint(/*in*/const QDate& date, /*out*/QStringList& tasks)> cbHandler);
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
    
    enum _HEAD_position
    {
        Not,   //! 无
        Top,   //! 上
        Down,  //! 下
        Left,  //! 左
        Right  //! 右
    };
    /*!
     * \brief 设置日历头的位置
     * \param pos
     * \return 
     */
    int SetHeadposition(_HEAD_position pos = Top);
    
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
    enum _CalendarType {
        CalendarTypeSolar = 0x01,  //! 阳历
        CalendarTypeLunar = 0x02   //! 农历
    };
    //! 设置日历类型（组合）
    int SetCalendarType(_CalendarType type);
    //! 得到日历类型
    _CalendarType GetCalendarType() const;
    
    //! 视图类型
    enum class _VIEW_TYPE {
        ViewTypeMonth,  //! 月
        ViewTypeWeek    //! 周
    };
    //!　设置视图类型
    int SetViewType(_VIEW_TYPE type);
    //! 得到视图类型
    _VIEW_TYPE GetViewType() const;
    
    //! @} 设置界面

    enum class _TOUCH_UP_DOWN_FUNCTION {
        TouchChangeView,
        TouchChangeMounth
    };
    int SetTouchUpDownFunction(_TOUCH_UP_DOWN_FUNCTION f);
    
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
    
    int Update();
    
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
     * 可以在相应的槽函数中调用 SelectedDate() 或　SelectedLunar() 得到选择的日期
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
