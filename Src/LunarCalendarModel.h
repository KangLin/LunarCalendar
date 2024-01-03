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
#include "DownloadFile.h"

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
 * 节日表 SQL 文件: Src/Resource/database/holidays.sql
 * 
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
        Anniversary,
        Tasks,
        TasksColorRole,
        TodayRole,
        WorkDayRole,
        WorkDayColorRole
    };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    int showMonth(int year, int month, bool bForce = false);
    int showWeek(int year, int week, bool bForce = false);
    int GetShowWeek();
    int GetShowMonth();
    int GetShowYear();
    int GetWeeksOfYear(int year = QDate::currentDate().day());
    int setDate(const QDate &d);
    QDate GetDate() const;
    int SetMinimumDate(const QDate &date);
    QDate GetMinimumDate();
    int SetMaximumDate(const QDate &date);
    QDate GetMaximumDate();
    int setRange(const QDate &min, const QDate &max);
    Qt::DayOfWeek firstDayOfWeek() const;
    //void setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);

    QDate dateForCell(int row, int column) const;
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
    CLunarCalendar::_CalendarType GetCalendarType();
    int SetViewType(CLunarCalendar::_VIEW_TYPE type);
    CLunarCalendar::_VIEW_TYPE GetViewType();
    
private Q_SLOTS:
    int slotUpdate();

private:
    void internalUpdate();
    QDate firstDateMonth() const;
    QDate endDateMonth() const;
    int WeeksOfMonth();
    QDate dateForCellMonth(int row, int column) const;
    void cellForDateMonth(const QDate &date, int *row, int *column) const;
    QDate dateForCellWeek(int row, int column) const;
    void cellForDateWeek(const QDate &date, int *row, int *column) const;
    int columnForFirstOfMonth(const QDate &date) const;
    int columnForDayOfWeek(Qt::DayOfWeek day) const;
    QTextCharFormat formatForCell(QDate d, int row, int col) const;
    QColor GetHeight() const;

    int InitHoliday();

private:
    QFont m_Font;
    
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
        WORK,       //!< 工作
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

        QStringList Anniversary;
        QStringList Tasks;
        uint TaskCounts;

        QString szImageBackgroup;
        __WORK_DAY WorkDay;
    };
    QVector<_DAY> m_Day;
    _DAY GetDay(int row, int col) const;
    
    bool m_bEnableHolidays;
    bool m_bEnableSolarTerm;
    QMap<int, QMap<int, QStringList> > m_SolarAnniversary;
    QMap<int, QMap<int, QStringList> > m_LunarAnniversary;

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
    QStringList GetHoliday(const QDate& d);
    QStringList GetLunarHoliday(int month, int day);

    // Chinese holidays sql
    QSharedPointer<RabbitCommon::CDownloadFile> m_DownloadChineseHolidaysSql;
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
    void slotDownloadChineseHolidaysFileFinished(const QString szFile);
};

#endif // CCALENDARMODEL_H
