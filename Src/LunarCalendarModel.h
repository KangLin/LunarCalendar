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
        ColorHighlight
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
    
    int AddHoliday(int month, int day, const QString &szName);
    int AddAnniversary(int month, int day, const QString &szName);
    int AddLunarAnniversary(int month, int day, const QString &szName);
    int SetTaskHandle(QSharedPointer<CLunarCalendar::CGetTaskHandler> handler);
    
    int SetCalendarType(CLunarCalendar::_CalendarType type);
    CLunarCalendar::_CalendarType GetCalendarType();
    int SetViewType(CLunarCalendar::_VIEW_TYPE type);
    CLunarCalendar::_VIEW_TYPE GetViewType();
    
private Q_SLOTS:
    int slotUpdate();

    void slotDownloadError(int nErr, const QString szError);
    void slotDownloadFileFinished(const QString szFile);

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

    int InitDatabase();
    void CheckUpdateDatabase();
    /**
     * @brief DownloadFile
     * @param url: Download url
     * @return 
     */
    int DownloadFile(const QVector<QUrl> &urls);

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
    enum __WORK_DAY{
        NO,
        WORK,
        REST
    };
    // cached day
    struct _DAY
    {
        int Solar;
        QString szSolarHoliday;
        int nLunarYear;
        int nLunarMonth;
        int nLunarDay;
        QString szLunar;
        QString szLunarHoliday;
        QString szAnniversary;
        QString szImageBackgroup;
        int nTasks;
        __WORK_DAY WorkDay;
    };
    QVector<_DAY> m_Day;
    _DAY GetDay(int row, int col) const;

    QMap<int, QMultiMap<int, QString> > m_Holiday;
    QMap<int, QMultiMap<int, QString> > m_Anniversary;
    
    CLunarCalendar::_VIEW_TYPE m_viewType;
    CLunarCalendar::_CalendarType m_calendarType;
    
    QSharedPointer<CLunarCalendar::CGetTaskHandler> m_GetTaskHandler;
    
    QSqlDatabase m_Database;
    QSharedPointer<RabbitCommon::CDownloadFile> m_Download;
    QFile m_UpdateSqlFile;
};

#endif // CCALENDARMODEL_H
