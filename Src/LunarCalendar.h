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
 * - 节日
 *   - 中国节假日，在左上角显示
 *   - 周年纪念日，在上面中间用圆点表示
 *     - 农历
 *     - 公历
 *   - 节日
 *     - 公历
 *     - 农历
 *     - 节气
 * - 农历位置显示优先级
 *   - 公历节日
 *   - 节气
 *   - 农历节日
 *   - 周年纪念日
 *     - 公历
 *     - 农历
 *   - 农历
 * \image html Docs/image/Holiday.png
 * 
 * \ingroup API
 * 
 */
class LUNARCALENDAR_EXPORT CLunarCalendar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QDate selectedDate WRITE SetSelectedDate READ SelectedDate)
    Q_PROPERTY(QString selectLunar READ SelectedLunar)
    Q_PROPERTY(int yearShow READ YearShown)
    Q_PROPERTY(int monthShow READ MonthShown)
    //Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ FirstDayOfWeek WRITE SetFirstDayOfWeek)
    Q_PROPERTY(QDate minimumDate READ MinimumDate WRITE SetMinimumDate)
    Q_PROPERTY(QDate maximumDate READ MaximumDate WRITE SetMaximumDate)
    Q_PROPERTY(bool showGrid READ ShowGrid WRITE SetShowGrid)
    Q_PROPERTY(bool showBackgroupImage READ ShowBackgroupImage WRITE SetShowBackgroupImage)
    
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
     * \brief 得到当前选择的日期
     */
    QDate SelectedDate() const;
    /*!
     * \brief 设置当前选择的日期
     * \param date
     * \param bForce: TRUE, 应用到模型中
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
    
    /*!
     * \brief 得到显示的年份
     * \return 显示的年份
     */
    int YearShown() const;
    /*!
     * \brief 得到显示的月份
     * \return 得到显示的月份
     */
    int MonthShown() const;
    
    /*!
     * \brief 得到年周从哪天开始
     * \return 
     */
    Qt::DayOfWeek FirstDayOfWeek() const;
    //void SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);
    
    /*!
     * \brief 显示或隐藏网格
     */
    void SetShowGrid(bool show);
    /*!
     * \brief 得到是否显示网格
     */
    bool ShowGrid() const;
    
    /*!
     * \brief 显示或隐藏背景图片
     */
    void SetShowBackgroupImage(bool show);
    /*!
     * \brief 得到是否显示背景图片
     */
    bool ShowBackgroupImage();

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
        virtual int onHandle(QDate date) = 0;
    };
    int SetTaskHandle(QSharedPointer<CGetTaskHandler> handler);

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
