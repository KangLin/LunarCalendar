#include "LunarCalendarModel.h"
#include <QTextCharFormat>
#include <QDebug>
#include <QPalette>
#include <QApplication>
#include <QStyle>
#include "CalendarLunar.h"

CLunarCalendarModel::CLunarCalendarModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_Date(QDate::currentDate()),
      m_MinimumDate(QDate::fromJulianDay(1)),
      m_MaximumDate(9999, 12, 31),
      m_ShownYear(m_Date.year()),
      m_ShownMonth(m_Date.month()),
      m_ShowWeek(1),
      m_FirstDay(QLocale().firstDayOfWeek())      
{
    SetCalendarType(static_cast<CLunarCalendar::_CalendarType>(
                        CLunarCalendar::CalendarTypeLunar
                        | CLunarCalendar::CalendarTypeSolar));
    
    SetViewType(static_cast<CLunarCalendar::_VIEW_TYPE>(
                    CLunarCalendar::ViewTypeMonth));
    m_RowCount = 5;
    m_ColumnCount = 7;
    m_Locale = QLocale::system();
    m_FirstDay = m_Locale.firstDayOfWeek();
    InitHoliday();
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
    case SolarColorRole:
        if(Qt::Horizontal == orientation)
        {
            int day = section + m_FirstDay;
            if(day > 7)
                day %= 7;
            if(Qt::Saturday == day || Qt::Sunday == day)
                return ColorHighlight;
        }
        return ColorNormal;
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
    };
    return QVariant();
}

bool CLunarCalendarModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
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
    case Anniversary:
        return GetDay(row, column).szAnniversary;
    case SolarColorRole:
    {
        if(d.month() != m_ShownMonth
                && CLunarCalendar::ViewTypeMonth == m_viewType)
            return ColorDisable;

        if(d.dayOfWeek() == Qt::Saturday
                || Qt::Sunday == d.dayOfWeek()
                || d == QDate::currentDate()
                || !GetDay(row, column).szSolarHoliday.isEmpty())
            return ColorHighlight;
        
        return ColorNormal;
    }
    case SolarFontRole:
        if(GetDay(row, column).szSolarHoliday.isEmpty())
            return FontNormal;
        
        return FontBold;
    default:
        break;
    };
    return QVariant();
}

bool CLunarCalendarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
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

int CLunarCalendarModel::showWeek(int year, int week)
{
    if (m_ShownYear == year && m_ShowWeek == week)
        return 0;
    m_ShownYear = year;
    m_ShowWeek = week;
    return Show();
}

int CLunarCalendarModel::showMonth(int year, int month)
{
    if (m_ShownYear == year && m_ShownMonth == month)
        return 0;
    
    m_ShownYear = year;
    m_ShownMonth = month;
    return Show();
}

int CLunarCalendarModel::Show()
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
            if(d.isNull())
                break;
            _DAY day;
            day.Solar = d.day();
            day.szSolarHoliday = m_Holiday[d.month()][d.day()];
            
            //qDebug() << "exec dateForCell time:" << tOnceStart.msecsTo(QTime::currentTime());
            
            if(m_calendarType & CLunarCalendar::CalendarTypeLunar)
            {
                CCalendarLunar lunar(d);
                day.szLunar = lunar.GetLunarDay();
                
                day.szLunarHoliday = lunar.GetHoliday();
                if(day.szLunarHoliday.isEmpty())
                    day.szLunarHoliday = lunar.GetJieQi();
                
                day.szAnniversary = lunar.GetAnniversary();
            }
            if(day.szAnniversary.isEmpty())
                day.szAnniversary = m_Anniversary[d.month()][d.day()];
            
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

void CLunarCalendarModel::setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
{
    m_FirstDay = dayOfWeek;
}

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
    QDate endDate = date.addDays(date.dayOfYear());
    int y = 0;
    int nWeeks = endDate.weekNumber(&year);
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

CLunarCalendarModel::_DAY CLunarCalendarModel::GetDay(int row, int col) const
{
    return m_Day[row * 7 + col];
}

int CLunarCalendarModel::SetViewType(CLunarCalendar::_VIEW_TYPE type)
{
    m_viewType = type;
    return 0;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendarModel::GetViewType()
{
    return m_viewType;
}

int CLunarCalendarModel::SetCalendarType(CLunarCalendar::_CalendarType type)
{
    m_calendarType = type;
    Show();
    return 0;
}

CLunarCalendar::_CalendarType CLunarCalendarModel::GetCalendarType()
{
    return m_calendarType;
}
