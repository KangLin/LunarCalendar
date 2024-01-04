/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "LunarCalendarModel.h"
#include "CalendarLunar.h"
#include "RabbitCommonDir.h"

#include <QStandardPaths>
#include <QTextCharFormat>
#include <QDebug>
#include <QPalette>
#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(Logger, "Rabbit.LunarCalendar.Model")
static Q_LOGGING_CATEGORY(LogDB, "Rabbit.LunarCalendar.Model.Database")

CLunarCalendarModel::CLunarCalendarModel(QObject *parent)
    : QAbstractTableModel(parent),
    m_Date(QDate::currentDate()),
    m_MinimumDate(-2000, 1, 1), //儒略日数（Julian Day Number，JDN）的计算是从格林威治标准时间的中午开始，包含一个整天的时间，起点的时间（0日）回溯至儒略历的公元前4713年1月1日中午12点（在格里历是公元前4714年11月24日）
    m_MaximumDate(3000, 12, 31),
    m_ShownYear(m_Date.year()),
    m_ShownMonth(m_Date.month()),
    m_ShowWeek(1),
    m_bEnableHolidays(true),
    m_bEnableSolarTerm(true),
    m_bEnableToolTip(true)
{
    SetCalendarType(static_cast<CLunarCalendar::_CalendarType>(
        static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeLunar)
        | static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeSolar)));

    SetViewType(static_cast<CLunarCalendar::_VIEW_TYPE>(
        CLunarCalendar::_VIEW_TYPE::ViewTypeMonth));

    m_RowCount = 5;
    m_ColumnCount = 7;
    m_Locale = QLocale::system();
    m_FirstDay = Qt::Monday; // m_Locale.firstDayOfWeek();
    InitHoliday();

    QString szSqlPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                            + QDir::separator() + "Rabbit" 
            + QDir::separator() + "LunarCalendar";
    QDir d;
    if(!d.exists(szSqlPath))
        d.mkpath(szSqlPath);
    QString szChineseHolidaysSql = szSqlPath + QDir::separator() + "chinese_holidays.sql";
    m_ChineseHolidaysSql.setFileName(szChineseHolidaysSql);
    QVector<QUrl> urls;
    urls << QUrl("https://sourceforge.net/p/lunarcalendar/code/ci/master/tree/Src/Resource/database/chinese_holidays.sql?format=raw")
         << QUrl("https://gitee.com/kl222/LunarCalendar/raw/master/Src/Resource/database/chinese_holidays.sql")
         << QUrl("https://github.com/KangLin/LunarCalendar/raw/master/Src/Resource/database/chinese_holidays.sql")
         << QUrl("https://gitlab.com/kl222/LunarCalendar/-/raw/master/Src/Resource/database/chinese_holidays.sql");
    DownloadChineseHolidaysSqlFile(urls);
    
    m_HolidaysSql.setFileName(szSqlPath + QDir::separator() + "holidays.sql");
    urls.clear();
    urls << QUrl("https://sourceforge.net/p/lunarcalendar/code/ci/master/tree/Src/Resource/database/holidays.sql?format=raw")
         << QUrl("https://gitee.com/kl222/LunarCalendar/raw/master/Src/Resource/database/holidays.sql")
         << QUrl("https://github.com/KangLin/LunarCalendar/raw/master/Src/Resource/database/holidays.sql")
         << QUrl("https://gitlab.com/kl222/LunarCalendar/-/raw/master/Src/Resource/database/holidays.sql");
    DownloadHolidaysSqlFile(urls);
    
    OpenDatabase();

    slotUpdate();
}

CLunarCalendarModel::~CLunarCalendarModel()
{
    if(m_Database.isOpen())
        m_Database.close();
}

QVariant CLunarCalendarModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignCenter);
    case Qt::ForegroundRole:
        if(Qt::Horizontal == orientation)
        {
            int day = section + m_FirstDay;
            if(day > 7)
                day %= 7;
            if(Qt::Saturday == day || Qt::Sunday == day)
                return GetHeight();
        }
        break;
    case Qt::BackgroundRole:
        {
            QPalette palette = QApplication::style()->standardPalette();
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
            return palette.window();
#else
            return palette.background();
#endif
        }
    case Qt::DisplayRole:
        switch (orientation) {
        case Qt::Horizontal:
        {
            int day = section + m_FirstDay;
            if(day > 7)
                day %= 7;
            return m_Locale.dayName(day, QLocale::NarrowFormat);
        }
        case Qt::Vertical:
            QDate date = dateForCell(section, columnForDayOfWeek(Qt::Monday));
            if (date.isValid())
                return date.weekNumber();
            else
                return QVariant();
        }
        break;
    default:
        break;
    };
    return QVariant();
}

bool CLunarCalendarModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int CLunarCalendarModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_RowCount;
}

int CLunarCalendarModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_ColumnCount;
}

/*!
 * 得到数据
 *
 * 数据初始化详见 \ref slotUpdate .
 * 得到数据：
 */
QVariant CLunarCalendarModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();
    QDate d = dateForCell(row, column);
    if(!d.isValid())
        return QVariant();

    switch (role) {
    case Qt::FontRole:
        return m_Font;
    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignCenter);
    case Qt::DisplayRole:
    case Qt::EditRole:
    case ROLE::SolarRole:
        return d.day();
    case ROLE::SolarColorRole:
    {
        if(d.month() != m_ShownMonth
            && CLunarCalendar::_VIEW_TYPE::ViewTypeMonth == m_viewType)
            return _COLOR_ROLE::ColorDisable;

        if(d.dayOfWeek() == Qt::Saturday
            || Qt::Sunday == d.dayOfWeek()
            //|| d == QDate::currentDate()
            || !GetDay(row, column).SolarHoliday.isEmpty())
            return _COLOR_ROLE::ColorHighlight;

        return _COLOR_ROLE::ColorNormal;
    }
    case ROLE::SolarFontRole:
        if(GetDay(row, column).SolarHoliday.isEmpty())
            return _FONT_ROLE::FontNormal;

        return _FONT_ROLE::FontBold;
    case ROLE::TodayRole:
        return d == QDate::currentDate();
    case ROLE::LunarColorRole:
    {
        if(d.month() != m_ShownMonth
            && CLunarCalendar::_VIEW_TYPE::ViewTypeMonth == m_viewType)
            return _COLOR_ROLE::ColorDisable;

        if(GetDay(row, column).LunarHoliday.isEmpty())
            return _COLOR_ROLE::ColorNormal;

        return _COLOR_ROLE::ColorHighlight;
    }
    case ROLE::LunarFontRole:
    {
        if(GetDay(row, column).LunarHoliday.isEmpty())
            return _FONT_ROLE::FontNormal;

        return _FONT_ROLE::FontBold;
    }
    case ROLE::LunarRole:
    {
        //! - 农历位置取值: \ref HolidayPriority
        _DAY day = GetDay(row, column);
        if(!day.SolarHoliday.isEmpty()) {
            foreach(auto h, day.SolarHoliday) {
                if(!h.isEmpty() && "" != h)
                    return h;
            }
        }
        if(!day.LunarHoliday.isEmpty()) {
            foreach (auto h, day.LunarHoliday) {
                if(!h.isEmpty() && "" != h)
                    return h;
            }
        }
        if(!day.Tasks.isEmpty()) {
            foreach (auto h, day.Tasks) {
                if(!h.isEmpty() && "" != h)
                    return h;
            }
        }
        return day.szLunarDay;
    }
    case Qt::ToolTipRole:
    {
        /*!
         * - 工具提示： 显示日期的详细信息
         *   \see CLunarCalendar::EnableToolTip
         */
        if(!m_bEnableToolTip)
            break;
        int nTotals = 5;
        int nCounts = 0;
        QString szTip;
        _DAY day = GetDay(row, column);
        szTip = d.toString(m_Locale.dateFormat(QLocale::LongFormat));
        szTip += "\n" + day.szLunar;
        nCounts = 2;
        if(!day.SolarHoliday.isEmpty()) {
            foreach(auto h, day.SolarHoliday) {
                if(h.isEmpty() || "" == h)
                    break;
                if(nCounts > nTotals)
                    break;
                if(nCounts == nTotals)
                {
                    nCounts++;
                    szTip += "\n……";
                    break;
                }
                nCounts++;
                szTip += "\n" + h;
            }
        }
        if(!day.LunarHoliday.isEmpty()) {
            foreach (auto h, day.LunarHoliday) {
                if(h.isEmpty() || "" == h)
                    break;
                if(nCounts > nTotals)
                    break;
                if(nCounts == nTotals)
                {
                    nCounts++;
                    szTip += "\n……";
                    break;
                }
                nCounts++;
                szTip += "\n" + h;
            }
        }
        if(!day.Tasks.isEmpty()) {
            foreach (auto h, day.Tasks) {
                if(h.isEmpty() || "" == h)
                    break;
                if(nCounts > nTotals)
                    break;
                if(nCounts == nTotals)
                {
                    nCounts++;
                    szTip += "\n……";
                    break;
                }
                nCounts++;
                szTip += "\n" + h;
            }
        }
        return szTip;
    }
    case ROLE::Tasks:
    {
        /*!
         * - 任务取值： 包括周年纪念日、任务、任务计数之和
         */
        _DAY day = GetDay(row, column);
        return day.TaskCounts + day.Tasks.size();
    }
    case ROLE::TasksColorRole:
        if(d.month() != m_ShownMonth
            && CLunarCalendar::_VIEW_TYPE::ViewTypeMonth == m_viewType)
            return _COLOR_ROLE::ColorDisable;
        return _COLOR_ROLE::ColorHighlight;
    case ROLE::BackgroupImage:
        return GetDay(row, column).szImageBackgroup;
    case ROLE::WorkDayRole:
    {
        switch(GetDay(row, column).WorkDay)
        {
        case __WORK_DAY::WORK:
             return "班";
        case __WORK_DAY::REST:
             return "休";
        default:
             break;
        }
        return "";
    }
    case ROLE::WorkDayColorRole:
        if(d.month() != m_ShownMonth
            && CLunarCalendar::_VIEW_TYPE::ViewTypeMonth == m_viewType)
             return _COLOR_ROLE::ColorDisable;
        if(__WORK_DAY::WORK == GetDay(row, column).WorkDay)
            return _COLOR_ROLE::ColorHighlight;
        /*if(d.dayOfWeek() == Qt::Saturday
            || Qt::Sunday == d.dayOfWeek()
            //|| d == QDate::currentDate()
            || !GetDay(row, column).SolarHoliday.isEmpty())
             return ColorHighlight;//*/
        return _COLOR_ROLE::ColorNormal;
    default:
        qDebug(Logger) << "index:" << index << "role:" << role;
        break;
    };
    return QVariant();
}

bool CLunarCalendarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) == value)
    {
        qDebug(Logger) << role << value << "value is same.";
        return false;
    }

    switch(role){
    case Qt::FontRole:
        m_Font = value.value<QFont>();
        break;
    default:
        break;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags CLunarCalendarModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    QDate date = dateForCell(index.row(), index.column());
    if (!date.isValid())
        return QAbstractTableModel::flags(index);
    if (date < m_MinimumDate)
        return Qt::NoItemFlags;
    if (date > m_MaximumDate)
        return Qt::NoItemFlags;
    return QAbstractTableModel::flags(index);
}

int CLunarCalendarModel::showWeek(int year, int week, bool bForce)
{
    if (m_ShownYear == year && m_ShowWeek == week && !bForce)
        return 0;
    m_ShownYear = year;
    m_ShowWeek = week;
    return slotUpdate();
}

int CLunarCalendarModel::showMonth(int year, int month, bool bForce)
{
    if (m_ShownYear == year && m_ShownMonth == month && !bForce)
        return 0;
    
    m_ShownYear = year;
    m_ShownMonth = month;
    return slotUpdate();
}

/*!
 * \brief 更新模型数据
 * 
 * \see data
 */
int CLunarCalendarModel::slotUpdate()
{
    switch(m_viewType)
    {
    case CLunarCalendar::_VIEW_TYPE::ViewTypeWeek:
        m_RowCount = 1;
        break;
    case CLunarCalendar::_VIEW_TYPE::ViewTypeMonth:
        m_RowCount = WeeksOfMonth();
        break;
    }
    
    //QTime tStart = QTime::currentTime();
    m_Day.clear();
    QDate d;
    int row = 0;
    do{
        for(int col = 0; col < 7; col++)
        {
            //QTime tOnceStart = QTime::currentTime();
            
            d = dateForCell(row, col);
            if(!d.isValid())
                break;
            _DAY day = {0};
            day.Solar = d.day();
            
            if(m_bEnableHolidays) {
                auto szHoliday = GetHoliday(d);
                if(!szHoliday.isEmpty())
                    day.SolarHoliday << szHoliday;
            }

            //qDebug() << "exec dateForCell time:" << tOnceStart.msecsTo(QTime::currentTime());
            
            if(static_cast<int>(m_calendarType)
                & static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeLunar))
            {
                CCalendarLunar lunar(d);
                day.nLunarMonth = lunar.GetMonth();
                day.nLunarDay = lunar.GetDay();
                day.szLunar = lunar.GetLunar();
                day.szLunarDay = lunar.GetLunarDay();
                
                if(m_bEnableHolidays){
                    // 除夕
                    if(12 == day.nLunarMonth && 29 <= day.nLunarDay) {
                        CCalendarLunar l(d.addDays(1));
                        if(1 == l.GetMonth())
                            day.LunarHoliday << "除夕";
                    }
                    auto szLunarHoliday = GetLunarHoliday(lunar.GetMonth(), lunar.GetDay());
                    if(!szLunarHoliday.isEmpty())
                        day.LunarHoliday << szLunarHoliday;
                }
                if(m_bEnableSolarTerm && !lunar.GetJieQi().isEmpty()) {
                    day.LunarHoliday << lunar.GetJieQi();
                }

                day.szImageBackgroup = lunar.GetJieQiImage();    
            }

            if(m_GetTaskHandler)
                day.TaskCounts += m_GetTaskHandler->onHandle(d, day.Tasks);

#if HAS_CPP_11
            if(m_cbTaskHandler)
                day.TaskCounts += m_cbTaskHandler(d, day.Tasks);
#endif
            
            day.WorkDay = GetChineseHolidays(d);
            m_Day.push_back(day);

            //qDebug() << "once time:" << tOnceStart.msecsTo(QTime::currentTime());
        }

        row++;
    } while(d.isValid());
    //qDebug() << "showMonth init totle time:" << tStart.msecsTo(QTime::currentTime());

    internalUpdate();
    return 0;
}

int CLunarCalendarModel::GetShowYear()
{
    return m_ShownYear;
}

int CLunarCalendarModel::GetShowMonth()
{
    return m_ShownMonth;
}

int CLunarCalendarModel::GetShowWeek()
{
    return m_ShowWeek;
}

int CLunarCalendarModel::setDate(const QDate &d)
{
    if(!d.isValid())
        return -1;
    m_Date = d;
    if (m_Date < m_MinimumDate)
        m_Date = m_MinimumDate;
    else if (m_Date > m_MaximumDate)
        m_Date = m_MaximumDate;
    //TODO:这里可以不必要，因为会触发信号void sigSelectionChanged();
    /*if(m_GetTaskHandler)
    {
        CCalendarLunar l(m_Date);
        m_GetTaskHandler->onHandle(m_Date);
    }//*/
    return 0;
}

int CLunarCalendarModel::SetMinimumDate(const QDate &d)
{
    if (!d.isValid() || d == m_MinimumDate)
        return 0;

    m_MinimumDate = d;
    if (m_MaximumDate < m_MinimumDate)
        m_MaximumDate = m_MinimumDate;
    if (m_Date < m_MinimumDate)
        m_Date = m_MinimumDate;
    
    internalUpdate();
    return 0;
}

QDate CLunarCalendarModel::GetMinimumDate()
{
    return m_MinimumDate;
}

int CLunarCalendarModel::SetMaximumDate(const QDate &d)
{
    if (!d.isValid() || d == m_MaximumDate)
        return 0;

    m_MaximumDate = d;
    if (m_MinimumDate > m_MaximumDate)
        m_MinimumDate = m_MaximumDate;
    if (m_Date > m_MaximumDate)
        m_Date = m_MaximumDate;
    internalUpdate();
    return 0;
}

QDate CLunarCalendarModel::GetMaximumDate()
{
    return m_MaximumDate;
}

Qt::DayOfWeek CLunarCalendarModel::firstDayOfWeek() const
{
    return m_FirstDay;
}

//void CLunarCalendarModel::setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
//{
//    m_FirstDay = dayOfWeek;
//}

int CLunarCalendarModel::setRange(const QDate &min, const QDate &max)
{
    m_MinimumDate = min;
    m_MaximumDate = max;
    if (m_MinimumDate > m_MaximumDate)
        qSwap(m_MinimumDate, m_MaximumDate);
    if (m_Date < m_MinimumDate)
        m_Date = m_MinimumDate;
    if (m_Date > m_MaximumDate)
        m_Date = m_MaximumDate;
    internalUpdate();
    return 0;
}

QDate CLunarCalendarModel::GetDate() const
{
    return m_Date;
}

void CLunarCalendarModel::internalUpdate()
{
    QModelIndex begin = index(0, 0);
    QModelIndex end = index(m_RowCount - 1, m_ColumnCount - 1);
    emit dataChanged(begin, end);
    emit headerDataChanged(Qt::Vertical, 0, m_RowCount - 1);
    emit headerDataChanged(Qt::Horizontal, 0, m_ColumnCount - 1);
}

QDate CLunarCalendarModel::dateForCell(int row, int column) const
{
    switch (m_viewType) {
    case CLunarCalendar::_VIEW_TYPE::ViewTypeMonth:
        return dateForCellMonth(row, column);
    case CLunarCalendar::_VIEW_TYPE::ViewTypeWeek:
        return dateForCellWeek(row, column);
    }
    return QDate();
}

QDate CLunarCalendarModel::dateForCellMonth(int row, int column) const
{
    if (row < 0 || row > m_RowCount - 1
        || column < 0 || column > m_ColumnCount - 1)
        return QDate();
    const QDate refDate = firstDateMonth();
    if (!refDate.isValid())
        return QDate();

    const int columnForFirstOfShownMonth = columnForFirstOfMonth(refDate);
    if (columnForFirstOfShownMonth < 0)
        row -= 1;

    const int requestedDay = 7 * (row) + column - columnForFirstOfShownMonth - refDate.day() + 1;
    return refDate.addDays(requestedDay);
}

QDate CLunarCalendarModel::dateForCellWeek(int row, int column) const
{
    if (row != 0 || column < 0 || column > m_ColumnCount - 1)
        return QDate();

    QDate firstOfYear(m_ShownYear, 1, 1);
    QDate date;
    int year = 0;
    int weeks = m_ShowWeek;
    
    if(1 != firstOfYear.weekNumber(&year))
        weeks++;
    
    date = firstOfYear.addDays((weeks -1 )* 7);
    int col = columnForDayOfWeek(static_cast<Qt::DayOfWeek>(date.dayOfWeek()));
    QDate firstDayOfWeek = date.addDays(-col);
    return firstDayOfWeek.addDays(column);
}

void CLunarCalendarModel::cellForDate(const QDate &date, int *row, int *column) const
{
    if (!row && !column)
        return;
    
    if (row)
        *row = -1;
    if (column)
        *column = -1;
    
    switch (m_viewType) {
    case CLunarCalendar::_VIEW_TYPE::ViewTypeMonth:
        return cellForDateMonth(date, row, column);
    case CLunarCalendar::_VIEW_TYPE::ViewTypeWeek:
        return cellForDateWeek(date, row, column);
    }
}

void CLunarCalendarModel::cellForDateMonth(const QDate &date, int *row, int *column) const
{
    if (!row && !column)
        return;

    if (row)
        *row = -1;
    if (column)
        *column = -1;

    const QDate refDate = firstDateMonth();
    if (!refDate.isValid())
        return;

    const int columnForFirstOfShownMonth = columnForFirstOfMonth(refDate);
    const int requestedPosition = refDate.daysTo(date) + columnForFirstOfShownMonth + refDate.day() - 1;

    int c = requestedPosition % 7;
    int r = requestedPosition / 7;
    if (c < 0) {
        c += 7;
        r -= 1;
    }

    if (columnForFirstOfShownMonth < 0)
        r += 1;

    if (r < 0 || r > m_RowCount - 1 || c < 0 || c > m_ColumnCount - 1)
        return;

    if (row)
        *row = r;
    if (column)
        *column = c;
}

void CLunarCalendarModel::cellForDateWeek(const QDate &date, int *row, int *column) const
{
    if (!row && !column)
        return;

    if (row)
        *row = -1;
    if (column)
        *column = -1;
    
    int year;
    int weeks = date.weekNumber(&year);
    if(weeks != m_ShowWeek)
        return;
    if(date.year() != m_ShownYear && date.year() != year)
        return;
    
    *row = 0;
    *column = columnForDayOfWeek(static_cast<Qt::DayOfWeek>(date.dayOfWeek()));
}

/*!
This simple algorithm tries to generate a valid date from the month shown.
Some months don't contain a first day (e.g. Jan of -4713 year,
so QDate (-4713, 1, 1) would be invalid). In that case we try to generate
another valid date for that month. Later, returned date's day is the number of cells
calendar widget will reserve for days before referenceDate. (E.g. if returned date's
day is 16, that day will be placed in 3rd or 4th row, not in the 1st or 2nd row).
Depending on referenceData we can change behaviour of Oct 1582. If referenceDate is 1st
of Oct we render 1 Oct in 1st or 2nd row. If referenceDate is 17 of Oct we show always 16
dates before 17 of Oct, and since this month contains the hole 5-14 Oct, the first of Oct
will be rendered in 2nd or 3rd row, showing more dates from previous month.
*/
QDate CLunarCalendarModel::firstDateMonth() const
{
    int refDay = 1;
    while (refDay <= 31) {
        QDate refDate(m_ShownYear, m_ShownMonth, refDay);
        if (refDate.isValid())
            return refDate;
        refDay += 1;
    }
    return QDate();
}

QDate CLunarCalendarModel::endDateMonth() const
{
    int day = 31;
    while(day >= 1)
    {
        QDate date(m_ShownYear, m_ShownMonth, day);
        if(date.isValid())
            return date;
        day--;
    }
    return QDate();
}

int CLunarCalendarModel::WeeksOfMonth()
{
    if(1 == m_ShownMonth)
    {
        int y;
        firstDateMonth().weekNumber(&y);
        if(firstDateMonth().year() != y)
            return endDateMonth().weekNumber() + 1;
    }
    if(12 == m_ShownMonth)
    {
        QDate date;
        bool bNext = false;
        int day = 31;
        while(day >= 1)
        {
            QDate d(m_ShownYear, m_ShownMonth, day);
            if(d.isValid())
            {
                int y;
                d.weekNumber(&y);
                if(d.year() == y)
                {
                    date = d;
                    break;
                } else
                    bNext = true;
            }
            day--;
        }
        if(bNext)
        {
            return date.weekNumber() - firstDateMonth().weekNumber() + 1 + 1;
        }
    }
    return endDateMonth().weekNumber() - firstDateMonth().weekNumber() + 1;
}

int CLunarCalendarModel::GetWeeksOfYear(int year)
{
    QDate date(year, 1, 1);
    QDate endDate = date.addDays(date.daysInYear());
    int y = 0;
    int nWeeks = endDate.weekNumber(&y);
    if(m_ShownYear != y)
        nWeeks = endDate.addDays(-7).weekNumber();
    return nWeeks;
}

int CLunarCalendarModel::columnForFirstOfMonth(const QDate &date) const
{
    return (columnForDayOfWeek(static_cast<Qt::DayOfWeek>(date.dayOfWeek()))
            - (date.day() % 7) + 8) % 7;
}

int CLunarCalendarModel::columnForDayOfWeek(Qt::DayOfWeek day) const
{
    if (day < 1 || unsigned(day) > unsigned(7))
        return -1;
    int column = static_cast<int>(day) - static_cast<int>(m_FirstDay);
    if (column < 0)
        column += 7;
    return column;
}

QTextCharFormat CLunarCalendarModel::formatForCell(QDate d, int row, int col) const
{
    QTextCharFormat format;
    QPalette pal = QApplication::style()->standardPalette();
    QPalette::ColorGroup cg = QPalette::Active;

    if(d.month() != m_ShownMonth)
    {
        cg = QPalette::Disabled;
    }

    format.setBackground(pal.brush(cg, QPalette::Window));
    format.setForeground(pal.brush(cg, QPalette::Text));
    
    if(d.dayOfWeek() == Qt::Saturday
            || Qt::Sunday == d.dayOfWeek()
            || d == QDate::currentDate()
            || !GetDay(row, col).SolarHoliday.isEmpty())
    {
        format.setForeground(QBrush(GetHeight()));
    }
    if(!GetDay(row, col).SolarHoliday.isEmpty())
    {
        QFont font = format.font();
        font.setBold(true);
        format.setFont(font);
    }
    return format;
}

QColor CLunarCalendarModel::GetHeight() const
{
    //return QColor(Qt::red);
    QPalette pal = QApplication::style()->standardPalette();
    QPalette::ColorGroup cg = QPalette::Active;
    return pal.color(cg, QPalette::Highlight);
}

bool CLunarCalendarModel::EnableHolidays(bool bEnable)
{
    bool old = m_bEnableHolidays;
    m_bEnableHolidays = bEnable;
    return old;
}

bool CLunarCalendarModel::EnableSolarTerm(bool bEnable)
{
    bool old = m_bEnableSolarTerm;
    m_bEnableSolarTerm = bEnable;
    return old;
}

int CLunarCalendarModel::SetTaskHandle(QSharedPointer<CLunarCalendar::CTaskHandler> handler)
{
    m_GetTaskHandler = handler;
    return 0;
}

#if HAS_CPP_11
/*!
     * \note It is need c++ standard 11
     */
int CLunarCalendarModel::SetTaskHandle(
    std::function<uint (/*in*/const QDate &,
                       /*out*/QStringList &)> cbHandler)
{
    m_cbTaskHandler = cbHandler;
    return 0;
}
#endif

CLunarCalendarModel::_DAY CLunarCalendarModel::GetDay(int row, int col) const
{
    return m_Day[row * 7 + col];
}

int CLunarCalendarModel::SetViewType(CLunarCalendar::_VIEW_TYPE type)
{
    m_viewType = type;
    slotUpdate();
    return 0;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendarModel::GetViewType()
{
    return m_viewType;
}

int CLunarCalendarModel::SetCalendarType(CLunarCalendar::_CalendarType type)
{
    m_calendarType = type;
    slotUpdate();
    return 0;
}

void CLunarCalendarModel::EnableToolTip(bool enable) {
    m_bEnableToolTip = enable;
}

CLunarCalendar::_CalendarType CLunarCalendarModel::GetCalendarType()
{
    return m_calendarType;
}

int CLunarCalendarModel::InitHoliday()
{
        
    /*
    https://baike.baidu.com/item/%E4%B8%AD%E5%9B%BD%E4%BC%A0%E7%BB%9F%E8%8A%82%E6%97%A5/396100?fromModule=disambiguation
    https://baike.baidu.com/item/中国传统节日
    
    节日名称 节日时间
    春节 正月初一
    元宵节（上元节）正月十五
    龙抬头 二月初二
    社日节（土地诞） 二月初二
    花朝 南方一般2月12日，北方2月15日。但是新化叫“花烛节”为2月15日。难道祖先是从北方迁移过来的。
    上巳节 三月初三
    寒食节 清明节前一天
    清明节 阳历4月5日前后
    端午节 五月初五
    天贶节 六月初六，晒衣节， 晒伏节“六月六，晒红绿。” “姑姑节”“六月六，请姑姑”。在古代还是另外一个节日，名叫天贶（赐赠的意思）节，六月六也是佛寺的一个节日，叫做翻经节，祭祀山神。
    七夕节 七月初七
    七月半（中元节） 七月十四/十五
    中秋节 八月十五
    重阳节 九月初九
    寒衣节 十月初一
    下元节 十月十五
    冬至节 阳历12月22日前后
    腊八节 腊月初八
    祭灶节（小年） 腊月廿三或廿四
    岁除（除夕）腊月廿九或三十
    */
   
    return 0;
}

int CLunarCalendarModel::OpenDatabase()
{
    int nRet = 0;
    QString szDatabaseFile;
    szDatabaseFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                           + QDir::separator() + "holidays.sqlite";
    m_Database = QSqlDatabase::addDatabase("QSQLITE");
    m_Database.setDatabaseName(szDatabaseFile);
    QDir d;
    if(d.exists(szDatabaseFile))
        qDebug(LogDB) << "Database file is exists:" << szDatabaseFile;
    else
    {
        qDebug(LogDB) << "Database file isn't exists:" << szDatabaseFile;
        if(!m_Database.open())
        {
            qCritical(LogDB) << "Open database fail:" << m_Database.lastError()
                              << "database file: " << szDatabaseFile;
            return m_Database.lastError().type();
        }

        nRet = InitTableHolidays();

        nRet = InitTableChineseHolidays();

        m_Database.close();
    }

    if(!m_Database.open())
    {
        qCritical(LogDB) << "Open datebase fail. database name:"
                         << m_Database.databaseName()
                         << "database file:" << szDatabaseFile;
        return -3;
    }

    return nRet;
}

int CLunarCalendarModel::ExecSqlFile(const QString& szFile)
{
    QFile sqlFile(szFile);
    if(!sqlFile.open(QFile::ReadOnly))
    {
        qCritical(LogDB) << "Open sql file fail:" << szFile;
        return -1;
    }

    QSqlQuery query(m_Database);
    QString szSql(sqlFile.readAll());
    QStringList sql = szSql.split(";");
    for(int i = 0; i < sql.size(); i++)
    {
        qDebug(LogDB) << "sql:" << sql[i];
        if(!query.exec(sql[i])
            && m_Database.lastError().type() != QSqlError::NoError)
        {
            qCritical(LogDB) << "Exec sql fail:" << m_Database.lastError();
            sqlFile.close();
            return -2;
        }
    }

    sqlFile.close();

    return 0;
}

int CLunarCalendarModel::InitTableHolidays()
{
    if(!m_Database.isOpen())
    {
        qCritical(LogDB) << "The database isn't open." << m_Database.databaseName();
        return -1;
    }
    
    QString szSqlFile = ":/database/Holidays";
#if !(defined (_DEBUG) || defined(DEBUG))
    szSqlFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                + QDir::separator() + "holidays.sql";
    if(!QFile::exists(szSqlFile))
    {
        QFile file(RabbitCommon::CDir::Instance()->GetDirDatabase(true)
                   + QDir::separator() + "holidays.sql");
        if(file.copy(szSqlFile))
            qInfo(Logger) << "Copy holidays sql file success. from"
                          << file.fileName() << "to" << szSqlFile;
        else
            qCritical(Logger) << "Copy holidays sql file fail. from"
                              << file.fileName() << "to" << szSqlFile;
    }
#endif
    
    return ExecSqlFile(szSqlFile);
}

/*!
 * 初始化中国节假日表
 */
int CLunarCalendarModel::InitTableChineseHolidays()
{
    if(!m_Database.isOpen())
    {
        qCritical(LogDB) << "The database isn't open." << m_Database.databaseName();
        return -1;
    }

    QString szSqlFile = ":/database/ChineseHolidays";
#if !(defined (_DEBUG) || defined(DEBUG))
    szSqlFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                        + QDir::separator() + "chinese_holidays.sql";
    if(!QFile::exists(szSqlFile))
    {
        QFile file(RabbitCommon::CDir::Instance()->GetDirDatabase(true)
                   + QDir::separator() + "chinese_holidays.sql");
        if(file.copy(szSqlFile))
            qInfo(Logger) << "Copy chinese holidays sql file success. from"
                           << file.fileName() << "to" << szSqlFile;
        else
            qCritical(Logger) << "Copy chinese holidays sql file fail. from"
                              << file.fileName() << "to" << szSqlFile;
    }
#endif

    return ExecSqlFile(szSqlFile);
}

void CLunarCalendarModel::CheckUpdateChineseHolidaysTable()
{
    bool bSame = true;
    QString szNativeSqlFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                        + QDir::separator() + "chinese_holidays.sql"; 

    QFile sqlNative(szNativeSqlFile);

    do{
        if(m_ChineseHolidaysSql.size() == 0)
            break;

        if(!m_ChineseHolidaysSql.isOpen())
            if(!m_ChineseHolidaysSql.open(QIODevice::ReadOnly))
            {
                qCritical(LogDB) << "Open update sql file fail."
                                 << m_ChineseHolidaysSql.fileName();
                break;
            }

        QCryptographicHash md5Update(QCryptographicHash::Md5);
        if(!md5Update.addData(&m_ChineseHolidaysSql))
        {
            qCritical(Logger) << "Update sql file md5sum fail";
            break;
        }

        if(!sqlNative.isOpen())
            if(!sqlNative.open(QIODevice::ReadOnly))
            {
                qCritical(LogDB) << "Open native sql file fail."
                                 << sqlNative.fileName();
                bSame = false;
                break;
            }

        QCryptographicHash md5Native(QCryptographicHash::Md5);
        if(md5Native.addData(&sqlNative))
        {
            if(md5Update.result() == md5Native.result())
            {
                qInfo(Logger) << "The files is same:"
                               << sqlNative.fileName()
                               << m_ChineseHolidaysSql.fileName();
                break;
            }
            bSame = false;
        }
    }while(0);

    if(sqlNative.isOpen())
        sqlNative.close();
    if(m_ChineseHolidaysSql.isOpen())
        m_ChineseHolidaysSql.close();

    if(bSame) return;

    qDebug(Logger) << "Update chinese_holidays.sql:"
                   << sqlNative.fileName() << "from"
                   << m_ChineseHolidaysSql.fileName();

    if(QFile::exists(szNativeSqlFile))
        QFile::remove(szNativeSqlFile);
    if(m_ChineseHolidaysSql.copy(szNativeSqlFile))
        qInfo(Logger) << "Update chinese_holidays.sql success. from"
                      << m_ChineseHolidaysSql.fileName()
                      << "to" << sqlNative.fileName();
    else
        qCritical(Logger) << "Update chinese_holidays.sql fail. from"
                      << m_ChineseHolidaysSql.fileName()
                      << "to" << sqlNative.fileName();

    InitTableChineseHolidays();
}

void CLunarCalendarModel::slotDownloadChineseHolidaysSqlFileError(int nErr, const QString szError)
{
    qCritical(Logger) << "Download chinese holidays sql file error:" << nErr << szError;
    QString szMsg = szError;
    if(szMsg.isEmpty()) szMsg = tr("Download chinese holidays sql file fail");
    m_ChineseHolidaysSql.close();
}

void CLunarCalendarModel::slotDownloadChineseHolidaysSqlFileFinished(const QString szFile)
{
    if(m_ChineseHolidaysSql.isOpen())
        m_ChineseHolidaysSql.close();

    QString f = m_ChineseHolidaysSql.fileName();
    if(QFile::exists(f))
        QFile::remove(f);
    if(QFile::rename(szFile, f))
        qInfo(Logger) << "Download chinese_holidays.sql success: rename"
                       << szFile << "to" << f;
    else
        qCritical(Logger) << "Download chinese_holidays.sql success. but rename fail from"
                       << szFile << "to" << f;
    CheckUpdateChineseHolidaysTable();
    internalUpdate();
}

int CLunarCalendarModel::DownloadChineseHolidaysSqlFile(const QVector<QUrl> &urls)
{
    int nRet = 0;

    if(m_ChineseHolidaysSql.isOpen())
        m_ChineseHolidaysSql.close();
    if(!urls.isEmpty())
    {
        m_DownloadChineseHolidaysSql = QSharedPointer<RabbitCommon::CDownloadFile>(
                    new RabbitCommon::CDownloadFile(urls));
        bool check = connect(m_DownloadChineseHolidaysSql.data(), SIGNAL(sigFinished(const QString)),
                this, SLOT(slotDownloadChineseHolidaysSqlFileFinished(const QString)));
        Q_ASSERT(check);
        check = connect(m_DownloadChineseHolidaysSql.data(), SIGNAL(sigError(int, const QString)),
                        this, SLOT(slotDownloadChineseHolidaysSqlFileError(int, const QString)));
        Q_ASSERT(check);
    }
    return nRet;
}

CLunarCalendarModel::__WORK_DAY CLunarCalendarModel::GetChineseHolidays(const QDate &d)
{
    __WORK_DAY day = __WORK_DAY::NO;
    if(!m_Database.isOpen())
    {
        qCritical(LogDB) << "The dababase isn't open."
                          << m_Database.databaseName();
        return day;
    }

    QSqlQuery query(m_Database);
    QString szSql = "select * from chinese_holidays where date='"
                    + d.toString("yyyy-MM-dd") + "'";
    //qDebug(LogDB) << "Sql:" << szSql;
    if(query.exec(szSql))
    {
        while(query.next())
        {
            QVariant v = query.value("iswork");
            //qDebug(LogDB) << query.value("date") << "isWork:" << v;
            if(v.isValid())
            {
                if(v.toInt() == 1)
                    day = __WORK_DAY::WORK;
                else
                    day = __WORK_DAY::REST;
            }
        }
    } else
        qCritical(LogDB) << "Get chinese holiday fail."
                          << query.lastError() << szSql;
    
    return day;
}

QStringList CLunarCalendarModel::GetHoliday(const QDate &d)
{
    QStringList lstHolidays;
    if(!m_Database.isOpen())
    {
        qCritical(LogDB) << "The dababase isn't open."
                          << m_Database.databaseName();
        return QStringList();
    }
    
    QString szFilter; //! \ref HolidaysTabl
    QSqlQuery query(m_Database);
    QString szSql = "select * from holiday_filter where table_name='holidays'";
    //qDebug(LogDB) << "Sql:" << szSql;
    if(query.exec(szSql))
    {
        while(query.next())
        {
            QVariant v = query.value("filter");
            if(v.isValid())
            {
                szFilter = v.toString();
            }
        }
    } else
        qWarning(LogDB) << "Get holiday filter fail."
                          << query.lastError() << szSql;
    
    szSql = "select * from holidays where month="
                    + QString::number(d.month())
                    + " and day=" + QString::number(d.day());
    if(!szFilter.isEmpty())
        szSql += " " + szFilter;
    //qDebug(LogDB) << "Sql:" << szSql;
    if(query.exec(szSql))
    {
        while(query.next())
        {
            QVariant v = query.value("name");
            if(v.isValid())
            {
                QString szHoliday = v.toString();
                if(!szHoliday.isEmpty())
                    lstHolidays << szHoliday;
            }
        }
    } else
        qCritical(LogDB) << "Get holiday fail."
                          << query.lastError() << szSql;

    return lstHolidays;
}

QStringList CLunarCalendarModel::GetLunarHoliday(int month, int day)
{
    QStringList lstHolidays;
    if(!m_Database.isOpen())
    {
        qCritical(LogDB) << "The dababase isn't open."
                          << m_Database.databaseName();
        return QStringList();
    }
    
    QString szFilter; //! \ref HolidaysTabl
    QSqlQuery query(m_Database);
    QString szSql = "select * from holiday_filter where table_name='holidays_lunar'";
    //qDebug(LogDB) << "Sql:" << szSql;
    if(query.exec(szSql))
    {
        while(query.next())
        {
            QVariant v = query.value("filter");
            if(v.isValid())
            {
                szFilter = v.toString();
            }
        }
    } else
        qWarning(LogDB) << "Get holiday filter fail."
                         << query.lastError() << szSql;
    szSql = "select * from holidays_lunar where month="
                    + QString::number(month)
                    + " and day=" + QString::number(day);
    if(!szFilter.isEmpty())
        szSql += " " + szFilter;
    //qDebug(LogDB) << "Sql:" << szSql;
    if(query.exec(szSql))
    {
        while(query.next())
        {
            QVariant v = query.value("name");
            if(v.isValid())
            {
                QString szHoliday = v.toString();
                if(!szHoliday.isEmpty())
                    lstHolidays << szHoliday;
            }
        }
    } else
        qCritical(LogDB) << "Get lunar holiday fail."
                          << query.lastError() << szSql;
    
    return lstHolidays;
}

int CLunarCalendarModel::DownloadHolidaysSqlFile(const QVector<QUrl> &urls)
{
    int nRet = 0;
    
    if(m_ChineseHolidaysSql.isOpen())
        m_ChineseHolidaysSql.close();
    if(!urls.isEmpty())
    {
        m_DownloadHolidaysSql = QSharedPointer<RabbitCommon::CDownloadFile>(
            new RabbitCommon::CDownloadFile(urls));
        bool check = connect(m_DownloadHolidaysSql.data(), SIGNAL(sigFinished(const QString)),
                             this, SLOT(slotDownloadHolidaysSqlFileFinished(const QString)));
        Q_ASSERT(check);
        check = connect(m_DownloadHolidaysSql.data(), SIGNAL(sigError(int, const QString)),
                        this, SLOT(slotDownloadHolidaysSqlFileError(int, const QString)));
        Q_ASSERT(check);
    }
    return nRet;
}

void CLunarCalendarModel::slotDownloadHolidaysSqlFileError(int nErr, const QString szError)
{
    qCritical(Logger) << "Download holidays sql file error:" << nErr << szError;
    QString szMsg = szError;
    if(szMsg.isEmpty()) szMsg = tr("Download holidays sql file fail");
    m_ChineseHolidaysSql.close();
}

void CLunarCalendarModel::slotDownloadHolidaysSqlFileFinished(const QString szFile)
{
    if(m_ChineseHolidaysSql.isOpen())
        m_ChineseHolidaysSql.close();
    
    QString f = m_ChineseHolidaysSql.fileName();
    if(QFile::exists(f))
        QFile::remove(f);
    if(QFile::rename(szFile, f))
        qInfo(Logger) << "Download holidays.sql success: rename"
                      << szFile << "to" << f;
    else
        qCritical(Logger) << "Download holidays.sql success. but rename fail from"
                          << szFile << "to" << f;
    CheckUpdateHolidaysTable();
    internalUpdate();
}

void CLunarCalendarModel::CheckUpdateHolidaysTable()
{
    bool bSame = true;
    QString szNativeSqlFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                              + QDir::separator() + "holidays.sql"; 
    
    QFile sqlNative(szNativeSqlFile);
    
    do{
        if(m_HolidaysSql.size() == 0)
            break;
        
        if(!m_HolidaysSql.isOpen())
            if(!m_HolidaysSql.open(QIODevice::ReadOnly))
            {
                qCritical(LogDB) << "Open update sql file fail."
                                 << m_HolidaysSql.fileName();
                break;
            }
        
        QCryptographicHash md5Update(QCryptographicHash::Md5);
        if(!md5Update.addData(&m_HolidaysSql))
        {
            qCritical(Logger) << "Update sql file md5sum fail";
            break;
        }
        
        if(!sqlNative.isOpen())
            if(!sqlNative.open(QIODevice::ReadOnly))
            {
                qCritical(LogDB) << "Open native sql file fail."
                                 << sqlNative.fileName();
                bSame = false;
                break;
            }
        
        QCryptographicHash md5Native(QCryptographicHash::Md5);
        if(md5Native.addData(&sqlNative))
        {
            if(md5Update.result() == md5Native.result())
            {
                qInfo(Logger) << "The files is same:"
                              << sqlNative.fileName()
                              << m_HolidaysSql.fileName();
                break;
            }
            bSame = false;
        }
    }while(0);
    
    if(sqlNative.isOpen())
        sqlNative.close();
    if(m_HolidaysSql.isOpen())
        m_HolidaysSql.close();
    
    if(bSame) return;
    
    qDebug(Logger) << "Update holidays.sql:"
                   << sqlNative.fileName() << "from"
                   << m_HolidaysSql.fileName();
    
    if(QFile::exists(szNativeSqlFile))
        QFile::remove(szNativeSqlFile);
    if(m_HolidaysSql.copy(szNativeSqlFile))
        qInfo(Logger) << "Update holidays.sql success. from"
                      << m_HolidaysSql.fileName()
                      << "to" << sqlNative.fileName();
    else
        qCritical(Logger) << "Update holidays.sql fail. from"
                          << m_HolidaysSql.fileName()
                          << "to" << sqlNative.fileName();
    
    InitTableHolidays();
}
