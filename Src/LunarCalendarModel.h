/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef CCALENDARMODEL_H
#define CCALENDARMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QLocale>
#include <QTextCharFormat>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QNetworkReply>
#include <QSslError>
#include <QFile>

#include "LunarCalendar.h"
#include "Download.h"

/*!
 * \brief 农历模型
 * \ingroup INTERNAL_API
 *
 * \section database 数据库
 * \subsection ChineseHolidaysTable 中国节假日表
 * 
 * - 中国节假日表 SQL 文件: Src/Resource/database/chinese_holidays.sql
 * 
 *   当有新的中国节假日时，修此表，加入新的即可。
 * \subsection HolidaysTabl 节日表
 * 
 * - 节日表 SQL 文件: Src/Resource/database/holidays.sql
 * - holiday_filter 可以控制节日的显示
 *   + 显示所有节日。过滤器设置为空
 *   + 显示基本的节日(level=1)
 *     \code
 *     INSERT INTO "holiday_filter" ("table_name", "filter") VALUES ("holidays", "and level=1");
 *     \endcode
 *   + 显示 level=1 或者 level=2 的节日
 *     \code
 *     INSERT INTO "holiday_filter" ("table_name", "filter") VALUES ("holidays","and (level=1 or level=2)");
 *     \endcode
 *     
 * \see GetHoliday 
 */
class CLunarCalendarModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    explicit CLunarCalendarModel(QObject *parent = nullptr);
    virtual ~CLunarCalendarModel();
    
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    
    enum _COLOR_ROLE
    {
        ColorNormal,
        ColorDisable,
        ColorHighlight,
        ColorRed
    };
    enum _FONT_ROLE
    {
        FontBold,
        FontNormal,
    };
    
    enum ROLE
    {
        SolarRole = Qt::UserRole + 1,
        SolarColorRole,
        SolarFontRole,
        LunarRole,
        LunarColorRole,
        LunarFontRole,
        BackgroupImage,
        Tasks,
        TasksColorRole,
        TodayRole,
        WorkDayRole,
        WorkDayColorRole,
        CalendarTypeRole
    };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    int showMonth(int year, int month, bool bForce = false);
    int showWeek(int year, int week, bool bForce = false);
    const int GetShowWeek() const;
    const int GetShowMonth() const;
    const int GetShowYear() const;
    const int GetWeeksOfYear(int year = QDate::currentDate().day()) const;
    int setDate(const QDate &d);
    const QDate GetDate() const;
    int SetMinimumDate(const QDate &date);
    const QDate GetMinimumDate() const;
    int SetMaximumDate(const QDate &date);
    const QDate GetMaximumDate() const;
    int setRange(const QDate &min, const QDate &max);
    const Qt::DayOfWeek firstDayOfWeek() const;
    //void setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);

    const QDate dateForCell(int row, int column) const;
    void cellForDate(const QDate &date, int *row, int *column) const;

    void EnableToolTip(bool enable);    
    bool EnableHolidays(bool bEnable = true);
    bool EnableSolarTerm(bool bEnable = true);

    int SetTaskHandle(QSharedPointer<CLunarCalendar::CTaskHandler> handler);
#if HAS_CPP_11
    /*!
     * \note It is need c++ standard 11
     */
    virtual int SetTaskHandle(std::function<uint (const QDate &, QStringList &)> cbHandler);
#endif
    int SetCalendarType(CLunarCalendar::_CalendarType type);
    const CLunarCalendar::_CalendarType GetCalendarType() const;
    int SetViewType(CLunarCalendar::_VIEW_TYPE type);
    const CLunarCalendar::_VIEW_TYPE GetViewType() const;
    
private Q_SLOTS:
    int slotUpdate();

private:
    void internalUpdate();
    const QDate firstDateMonth() const;
    const QDate endDateMonth() const;
    const int WeeksOfMonth() const;
    const QDate dateForCellMonth(int row, int column) const;
    void cellForDateMonth(const QDate &date, int *row, int *column) const;
    const QDate dateForCellWeek(int row, int column) const;
    void cellForDateWeek(const QDate &date, int *row, int *column) const;
    const int columnForFirstOfMonth(const QDate &date) const;
    const int columnForDayOfWeek(Qt::DayOfWeek day) const;
    const QTextCharFormat formatForCell(QDate d, int row, int col) const;
    const QColor GetHeight() const;

    int InitHoliday();

private:    
    QDate m_Date;
    QDate m_MinimumDate;
    QDate m_MaximumDate;
    int m_ShownYear;
    int m_ShownMonth;
    int m_ShowWeek;
    int m_ColumnCount, m_RowCount;
    Qt::DayOfWeek m_FirstDay;
    QLocale m_Locale;
    enum class __WORK_DAY {
        NO = 0,    //!< 无
        WORK,      //!< 工作
        REST,      //!< 休息
    };
    // cached day
    struct _DAY
    {
        int Solar;

        int nLunarYear;
        int nLunarMonth;
        int nLunarDay;
        QString szLunar;
        QString szLunarDay;

        QStringList SolarHoliday;
        QStringList LunarHoliday;

        QStringList Tasks;
        uint TaskCounts;

        QString szImageBackgroup;
        __WORK_DAY WorkDay;
    };
    QVector<_DAY> m_Day;
    _DAY GetDay(int row, int col) const;

    bool m_bEnableHolidays;
    bool m_bEnableSolarTerm;

    CLunarCalendar::_VIEW_TYPE m_viewType;
    CLunarCalendar::_CalendarType m_calendarType;

    bool m_bEnableToolTip;
    
    QSharedPointer<CLunarCalendar::CTaskHandler> m_GetTaskHandler;
#if HAS_CPP_11
    std::function<uint(/*in*/const QDate& date,
                       /*out*/QStringList& tasks)> m_cbTaskHandler;
#endif

    QSqlDatabase m_Database;
    int OpenDatabase();
    int ExecSqlFile(const QString& szFile);
    int InitTableHolidays();
    /*!
     * \brief 得到指定日期的节日
     * \param d
     * \return 返回节日列表
     */
    const QStringList GetHoliday(const QDate& d) const;
    const QStringList GetLunarHoliday(int month, int day) const;
    QSharedPointer<RabbitCommon::CDownload> m_DownloadHolidaysSql;
    QFile m_HolidaysSql;
    int DownloadHolidaysSqlFile(const QVector<QUrl> &urls);
    void CheckUpdateHolidaysTable();
private Q_SLOTS:
    void slotDownloadHolidaysSqlFileError(int nErr, const QString szError);
    void slotDownloadHolidaysSqlFileFinished(const QString szFile);
    
private:
    // Chinese holidays sql
    QSharedPointer<RabbitCommon::CDownload> m_DownloadChineseHolidaysSql;
    QFile m_ChineseHolidaysSql;
    int InitTableChineseHolidays();
    void CheckUpdateChineseHolidaysTable();
    /**
     * @brief Download chinese holidays sql file
     * @param url: Download url
     * @return 
     */
    int DownloadChineseHolidaysSqlFile(const QVector<QUrl> &urls);
    __WORK_DAY GetChineseHolidays(const QDate& d);

private Q_SLOTS:
    void slotDownloadChineseHolidaysSqlFileError(int nErr, const QString szError);
    void slotDownloadChineseHolidaysSqlFileFinished(const QString szFile);
};

#endif // CCALENDARMODEL_H
