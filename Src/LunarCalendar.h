/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

/*!
 * \defgroup API 应用程序接口
 * \defgroup INTERNAL_API 内部使用接口
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

/*!
 * \brief 农历界面类
 * \details
 * \section LunarUI 农历界面
 * - 日历
 * \image html Docs/image/ScreenShotUbunt.png
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
 * \section Holiday 节日
 *   - 中国节假日，在左上角显示
 *   - 节日
 *     - 公历
 *     - 农历
 *     - 节气
 *   - 周年纪念日，在上面中间用圆点表示
 *     - 农历
 *     - 公历
 * \image html Docs/image/Holiday.png
 * 
 * \subsection HolidayPriority 农历位置显示优先级
 *   - 公历节日
 *   - 农历节日
 *   - 节气
 *   - 周年纪念日
 *     - 公历
 *     - 农历
 *   - 农历
 * 
 * \ingroup API
 * 
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
    
    /*!
     * \brief 初始化程序资源，仅在在程序开始处, QApplication a(argc, argv); 之后调用一次.
     */
    static void InitResource();
    /*!
     * \brief 释放程序资源。仅在程序退出前调用一次
     */
    static void CLeanResource();

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

    /*!
     * \brief 增加公历假日
     * \param month: 节日月份
     * \param day: 节日日期
     * \param szName: 节日名
     * \image html Docs/image/Holiday.png
     */
    int AddHoliday(int month, int day, const QString &szName);
    /*!
     * \brief 设置周年纪念日（例如：公历生日）
     * \param month: 月
     * \param day: 日
     * \param szName: 纪念日名
     * \image html Docs/image/Holiday.png
     */
    int AddAnniversary(int month, int day, const QString &szName);
    /*!
     * \brief 设置农历周年纪念日（例如：农历生日）
     * \param month: 农历月
     * \param day: 农历日
     * \param szName: 纪念日名
     * \image html Docs/image/Holiday.png
     */
    int AddLunarAnniversary(int month, int day, const QString &szName);

    class CGetTaskHandler
    {
    public:
        CGetTaskHandler() {}
        virtual ~CGetTaskHandler(){}
        /*!
         * \param date: 日期
         * \return 返回任务数。当任务数大于0时，会在日历上显示圆点提示
         */
        virtual uint onHandle(QDate date) = 0;
    };
    int SetTaskHandle(QSharedPointer<CGetTaskHandler> handler);

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
     * \brief 显示或隐藏背景图片
     */
    void ShowBackgroupImage(bool show);

public:

    enum _CalendarType{
        CalendarTypeSolar = 0x01,  //! 阳历
        CalendarTypeLunar = 0x02   //! 农历
    };
    //! 设置日历类型（组合）
    int SetCalendarType(_CalendarType type);
    //! 得到日历类型
    _CalendarType GetCalendarType() const;
    enum _VIEW_TYPE{
        ViewTypeMonth,  //! 月
        ViewTypeWeek    //! 周
    };
    //!　设置视图类型
    int SetViewType(_VIEW_TYPE type);
    //! 得到视图类型
    _VIEW_TYPE GetViewType() const;
    
    enum _TOUCH_UP_DOWN_FUNCTION{
        TouchChangeView,
        TouchChangeMounth
    };
    int SetTouchUpDownFunction(_TOUCH_UP_DOWN_FUNCTION f);
    
    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
    
    int Update();
    
    //////// 下列功能仅由开发者使用。普通用户不要使用。
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
    
Q_SIGNALS:
    /*!
     * \brief 当前选择日期改变时触发
     * 可以在相应的槽函数中调用 SelectedDate() 或　SelectedLunar() 得到选择的日期
     */
    void sigSelectionChanged();

public Q_SLOTS:
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
