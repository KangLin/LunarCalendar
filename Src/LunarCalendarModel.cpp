#include "LunarCalendarModel.h"
#include "CalendarLunar.h"
#include "RabbitCommonDir.h"
#include "RabbitCommonLog.h"

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

CLunarCalendarModel::CLunarCalendarModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_Date(QDate::currentDate()),
      m_MinimumDate(-2000, 1, 1), //儒略日数（Julian Day Number，JDN）的计算是从格林威治标准时间的中午开始，包含一个整天的时间，起点的时间（0日）回溯至儒略历的公元前4713年1月1日中午12点（在格里历是公元前4714年11月24日）
      m_MaximumDate(3000, 12, 31),
      m_ShownYear(m_Date.year()),
      m_ShownMonth(m_Date.month()),
      m_ShowWeek(1),
      m_pReply(nullptr)
{
    SetCalendarType(static_cast<CLunarCalendar::_CalendarType>(
                        CLunarCalendar::CalendarTypeLunar
                        | CLunarCalendar::CalendarTypeSolar));
    
    SetViewType(static_cast<CLunarCalendar::_VIEW_TYPE>(
                    CLunarCalendar::ViewTypeMonth));
    m_RowCount = 5;
    m_ColumnCount = 7;
    m_Locale = QLocale::system();
    m_FirstDay = Qt::Monday; // m_Locale.firstDayOfWeek();
    InitHoliday();
    
    QString szFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                            + QDir::separator() + "Rabbit" 
            + QDir::separator() + "LunarCalendar";
    QDir d;
    if(!d.exists(szFile))
        d.mkpath(szFile);
    szFile = szFile + QDir::separator() + "chinese_holidays.sql";
    m_UpdateSqlFile.setFileName(szFile);
    DownloadFile(QUrl("https://raw.githubusercontent.com/KangLin/LunarCalendar/master/Src/Resource/database/chinese_holidays.sql"));
    InitDatabase();
    
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
    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignCenter);
    case Qt::DisplayRole:
    case Qt::EditRole:
    case SolarRole:
        return d.day();
    case LunarRole:
        if(!GetDay(row, column).szSolarHoliday.isEmpty())
            return GetDay(row, column).szSolarHoliday;
        if(!GetDay(row, column).szLunarHoliday.isEmpty())
            return GetDay(row, column).szLunarHoliday;
        return GetDay(row, column).szLunar;
    case LunarColorRole:
    {
        if(d.month() != m_ShownMonth
                && CLunarCalendar::ViewTypeMonth == m_viewType)
            return ColorDisable;

        if(GetDay(row, column).szLunarHoliday.isEmpty())
            return ColorNormal;
        
        return ColorHighlight;
    }
    case LunarFontRole:
    {
        if(GetDay(row, column).szLunarHoliday.isEmpty())
            return FontNormal;
        
        return FontBold;
    }
    case BackgroupImage:
        return GetDay(row, column).szImageBackgroup;
    case Anniversary:
        return GetDay(row, column).szAnniversary;
    case Tasks:
        return GetDay(row, column).nTasks;
    case SolarColorRole:
    {
        if(d.month() != m_ShownMonth
                && CLunarCalendar::ViewTypeMonth == m_viewType)
            return ColorDisable;

        if(d.dayOfWeek() == Qt::Saturday
                || Qt::Sunday == d.dayOfWeek()
                //|| d == QDate::currentDate()
                || !GetDay(row, column).szSolarHoliday.isEmpty())
            return ColorHighlight;
        
        return ColorNormal;
    }
    case SolarFontRole:
        if(GetDay(row, column).szSolarHoliday.isEmpty())
            return FontNormal;
        
        return FontBold;
    case TodayRole:
        return d == QDate::currentDate();
    case WorkDayRole:
    {
        switch(GetDay(row, column).WorkDay)
        {
        case WORK:
             return "班";
        case REST:
             return "休";
        }
        return "";
    }
    case WorkDayColorRole:
        if(WORK == GetDay(row, column).WorkDay)
            return ColorHighlight;
        return ColorDisable;
    default:
        break;
    };
    return QVariant();
}

bool CLunarCalendarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
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

int CLunarCalendarModel::slotUpdate()
{
    switch(m_viewType)
    {
    case CLunarCalendar::ViewTypeWeek:
        m_RowCount = 1;
        break;
    case CLunarCalendar::ViewTypeMonth:
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
            _DAY day;
            day.Solar = d.day();
            day.szSolarHoliday = m_Holiday[d.month()][d.day()];
            
            //qDebug() << "exec dateForCell time:" << tOnceStart.msecsTo(QTime::currentTime());
            
            if(m_calendarType & CLunarCalendar::CalendarTypeLunar)
            {
                CCalendarLunar lunar(d);
                day.nLunarMonth = lunar.GetMonth();
                day.nLunarDay = lunar.GetDay();
                day.szLunar = lunar.GetLunarDay();
                
                day.szLunarHoliday = lunar.GetHoliday();
                if(day.szLunarHoliday.isEmpty())
                    day.szLunarHoliday = lunar.GetJieQi();
                
                day.szAnniversary = lunar.GetAnniversary();
                day.szImageBackgroup = lunar.GetJieQiImage();    
            }
            if(day.szAnniversary.isEmpty())
                day.szAnniversary = m_Anniversary[d.month()][d.day()];
            
            if(m_GetTaskHandler)
                day.nTasks = m_GetTaskHandler->onHandle(d);
            else
                day.nTasks = 0;
            
            day.WorkDay = NO;
            if(m_Database.isOpen())
            {
                QSqlQuery query(m_Database);
                QString szSql = "select * from chinese_holidays where date='" + d.toString("yyyy-MM-dd") + "'";
                //qDebug() << "Sql:" << szSql;
                if(query.exec(szSql))
                {
                    while(query.next())
                    {
                        QVariant v = query.value("iswork");
                        //qDebug() << query.value("date") << "isWork:" << v;
                        if(v.isValid())
                        {
                            if(v.toInt() == 1)
                                day.WorkDay = WORK;
                            else
                                day.WorkDay = REST;
                        }
                    }
                }
            }
            m_Day.push_back(day);
            
            //qDebug() << "once time:" << tOnceStart.msecsTo(QTime::currentTime());
        }
        row++;
    }while(d.isValid());
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
    case CLunarCalendar::ViewTypeMonth:
        return dateForCellMonth(row, column);
    case CLunarCalendar::ViewTypeWeek:
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
    case CLunarCalendar::ViewTypeMonth:
        return cellForDateMonth(date, row, column);
    case CLunarCalendar::ViewTypeWeek:
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

/*
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
            || !GetDay(row, col).szSolarHoliday.isEmpty())
    {
        format.setForeground(QBrush(GetHeight()));
    }
    if(!GetDay(row, col).szSolarHoliday.isEmpty())
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

int CLunarCalendarModel::AddHoliday(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    m_Holiday[month].insertMulti(day, szName);
    int row, col;
    QDate date(m_ShownYear, month, day);
    cellForDate(date, &row, &col);
    if(-1 == row || -1 == col || m_Day.isEmpty())
        return -2;
    m_Day[row * 7 + col].szSolarHoliday = szName;            
    return 0;
}

int CLunarCalendarModel::AddAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    m_Anniversary[month].insertMulti(day, szName);
    int row, col;
    QDate date(m_ShownYear, month, day);
    cellForDate(date, &row, &col);
    if(-1 == row || -1 == col || m_Day.isEmpty())
        return -2;
    m_Day[row * 7 + col].szAnniversary = szName;            
    return 0;
}

int CLunarCalendarModel::AddLunarAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;

    CCalendarLunar l;
    return l.AddAnniversary(month, day, szName);
}

int CLunarCalendarModel::SetTaskHandle(QSharedPointer<CLunarCalendar::CGetTaskHandler> handler)
{
    m_GetTaskHandler = handler;
    return 0;
}

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

CLunarCalendar::_CalendarType CLunarCalendarModel::GetCalendarType()
{
    return m_calendarType;
}

int CLunarCalendarModel::InitHoliday()
{
    AddHoliday(1, 1, "元旦");
    AddHoliday(3, 8, "妇女节");
    AddHoliday(5, 1, "劳动节");
    AddHoliday(6, 1, "儿童节");
    AddHoliday(8, 1, "建军节");
    AddHoliday(10, 1, "国庆节");
    return 0;
}

int CLunarCalendarModel::InitDatabase()
{
    QString szFile;
    szFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                           + QDir::separator() + "db.sqlite";
    m_Database = QSqlDatabase::addDatabase("QSQLITE");
    m_Database.setDatabaseName(szFile);
    QDir d;
    if(!d.exists(szFile))
    {
        if(!m_Database.open())
        {
            qCritical() << "Open database fail:"
                        << m_Database.lastError().text()
                        << "database file:" << szFile;
            
            return m_Database.lastError().number();
        }
#if defined (_DEBUG) || defined(DEBUG)
        QFile file(":/database/ChineseHolidays");
#else
        QString szUserSql = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                + QDir::separator() + "chinese_holidays.sql";
        if(!QFile::exists(szUserSql))
        {
            QFile file(RabbitCommon::CDir::Instance()->GetDirDatabase(true)
                       + QDir::separator() + "chinese_holidays.sql");
            file.copy(szUserSql);
        }
        QFile file(szUserSql);
#endif
        if(file.open(QFile::ReadOnly))
        {
            QSqlQuery query(m_Database);
            QString szSql(file.readAll());
            QStringList sql = szSql.split(";");
            for(int i = 0; i < sql.size(); i++)
            {
                qDebug() << sql[i];
                if(!query.exec(sql[i]) && m_Database.lastError().type() != QSqlError::NoError)
                {
                    qCritical() << "Create database fail: " << m_Database.lastError();
                    file.close();
                    m_Database.close();
                    QDir d;
                    d.remove(szFile);
                    return -2;
                }
            }
            file.close();
        }
        m_Database.close();
    }
    
    if(!m_Database.open())
    {
        qCritical() << "Open datebase fail";
        return -3;
    }

    return 0;
}

void CLunarCalendarModel::CheckUpdateDatabase()
{
    bool bRet = false;
    QString szFile = RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                        + QDir::separator() + "chinese_holidays.sql"; 

    QFile sql(szFile);
    
    do{
        if(sql.exists())
        {            
            if(!m_UpdateSqlFile.isOpen())
                m_UpdateSqlFile.open(QIODevice::ReadOnly);
            
            if(!sql.isOpen())
                sql.open(QIODevice::ReadOnly);
            
            if(m_UpdateSqlFile.size() == 0) break;
//            if(m_UpdateSqlFile.size() <= sql.size())
//            {
//                LOG_MODEL_ERROR("LunarCalendar", "update sql file size is little old sql");
//                break;
//            }
            
            QCryptographicHash md5Update(QCryptographicHash::Md5);
            if(!md5Update.addData(&m_UpdateSqlFile))
            {
                LOG_MODEL_ERROR("LunarCalendar", "Update sql file md5sum fail");
                break;
            }
            
            QCryptographicHash md5Sql(QCryptographicHash::Md5);
            if(md5Sql.addData(&sql))
            {
                if(md5Update.result() == md5Sql.result())
                {
                    LOG_MODEL_DEBUG("LunarCalendar", "The file is same");
                    break;
                }
            }
        }
        
        bRet = true;
    }while(0);
    
    if(sql.isOpen())
        sql.close();
    if(m_UpdateSqlFile.isOpen())
        m_UpdateSqlFile.close();
    
    if(!bRet) return;
    
    m_UpdateSqlFile.copy(szFile);
    if(m_Database.isOpen())
        m_Database.close();

    QFile::remove(RabbitCommon::CDir::Instance()->GetDirUserDatabase()
                               + QDir::separator() + "db.sqlite");
    InitDatabase();
    
}

void CLunarCalendarModel::slotReadyRead()
{
    if(m_UpdateSqlFile.isOpen() && m_pReply)
    {
        QByteArray d = m_pReply->readAll();
        m_UpdateSqlFile.write(d);
    }
}

void CLunarCalendarModel::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)
    LOG_MODEL_DEBUG("LunarCalendar", "Is download [%d] %d/%d",
                    bytesReceived * 100 / bytesTotal,
                    bytesReceived,
                    bytesTotal);
}

void CLunarCalendarModel::slotError(QNetworkReply::NetworkError e)
{
    qDebug() << "CLunarCalendarModel::slotError: " << e;
    if(m_pReply)
    {
        LOG_MODEL_ERROR("LunarCalendar",  "Reply error: %s",
                        m_pReply->errorString().toStdString().c_str());
        m_pReply->disconnect();
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    m_UpdateSqlFile.close();
}

void CLunarCalendarModel::slotSslError(const QList<QSslError> e)
{
    qDebug() << "CLunarCalendarModel::slotSslError: " << e;
    QString sErr;
    foreach(QSslError s, e)
        sErr += s.errorString() + " ";
    qDebug() << "Download fail:" << sErr;
    if(m_pReply)
    {
        m_pReply->disconnect();
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    
    m_UpdateSqlFile.close();
}

void CLunarCalendarModel::slotFinished()
{
    LOG_MODEL_DEBUG("LunarCalendar", "CLunarCalendarModel::slotFinished(): %s",
                    m_UpdateSqlFile.fileName().toStdString().c_str());
    
    QVariant redirectionTarget;
    if(m_pReply)
       redirectionTarget = m_pReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirectionTarget.isValid())
    {
        if(m_pReply)
        {
            m_pReply->disconnect();
            m_pReply->deleteLater();
            m_pReply = nullptr;
        }
        QUrl u = redirectionTarget.toUrl();  
        if(u.isValid())
        {
            qDebug() << "CLunarCalendarModel::slotFinished():redirectionTarget:url:" << u;
            DownloadFile(u);
        }
        return;
    }
    
    if(m_pReply)
    {
        m_pReply->disconnect();
        m_pReply->deleteLater();
        m_pReply = nullptr;
    }
    
    m_UpdateSqlFile.close();
    
    CheckUpdateDatabase();
    internalUpdate();
}

int CLunarCalendarModel::DownloadFile(const QUrl &url)
{
    int nRet = 0;

    if(m_UpdateSqlFile.isOpen())
        m_UpdateSqlFile.close();
    if(!m_UpdateSqlFile.open(QIODevice::WriteOnly))
    {
        LOG_MODEL_ERROR("LunarCalendar", "Open file fail: %s",
                        m_UpdateSqlFile.fileName().toStdString().c_str());
        return -1;
    }

    QNetworkRequest request(url);
    //https://blog.csdn.net/itjobtxq/article/details/8244509
    /*QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(config);
    */
    m_pReply = m_NetManager.get(request);
    if(!m_pReply)
        return -1;
    
    bool check = false;
    check = connect(m_pReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    Q_ASSERT(check);
    check = connect(m_pReply, SIGNAL(downloadProgress(qint64, qint64)),
                    this, SLOT(slotDownloadProgress(qint64, qint64)));
    Q_ASSERT(check);
    check = connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)),
                    this, SLOT(slotError(QNetworkReply::NetworkError)));
    Q_ASSERT(check);
    check = connect(m_pReply, SIGNAL(sslErrors(const QList<QSslError>)),
                    this, SLOT(slotSslError(const QList<QSslError>)));
    check = connect(m_pReply, SIGNAL(finished()),
                    this, SLOT(slotFinished()));
    
    return nRet;
}
