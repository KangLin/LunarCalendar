#include "LunarCalendarModel.h"
#include <QTextCharFormat>
#include <QDebug>
#include <QPalette>

CLunarCalendarModel::CLunarCalendarModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_Date(QDate::currentDate()),
      m_MinimumDate(QDate::fromJulianDay(1)),
      m_MaximumDate(9999, 12, 31),
      m_ShownYear(m_Date.year()),
      m_ShownMonth(m_Date.month()),
      m_FirstDay(QLocale().firstDayOfWeek())      
{
    m_RowCount = 5;
    m_ColumnCount = 7;
    m_Locale = QLocale::system();
    m_FirstDay = m_Locale.firstDayOfWeek();
}

QVariant CLunarCalendarModel::headerData(int section, Qt::Orientation orientation, int role) const
{    
    if (role == Qt::TextAlignmentRole)
        return static_cast<int>(Qt::AlignCenter);
    
    if(Qt::DisplayRole != role)
        return QVariant();
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
    
    if (role == Qt::TextAlignmentRole)
        return (int) Qt::AlignCenter;

    int row = index.row();
    int column = index.column();

    if(role == Qt::DisplayRole) {
        QDate date = dateForCell(row, column);
        if (date.isValid())
            return date.day();
        return QString();
    }

    QTextCharFormat fmt = formatForCell(row, column);
    if (role == Qt::BackgroundRole)
        return fmt.background().color();
    if (role == Qt::ForegroundRole)
        return fmt.foreground().color();
    if (role == Qt::FontRole)
        return fmt.font();
    if (role == Qt::ToolTipRole)
        return fmt.toolTip();
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
    
    return Qt::ItemIsEditable; // FIXME: Implement me!
}

int CLunarCalendarModel::showMonth(int year, int month)
{
    if (m_ShownYear == year && m_ShownMonth == month)
        return 0;

    m_ShownYear = year;
    m_ShownMonth = month;
    m_RowCount = WeeksOfMonth();
    
    internalUpdate();
    return 0;
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
    if (row < 0 || row > m_RowCount - 1
        || column < 0 || column > m_ColumnCount - 1)
        return QDate();
    const QDate refDate = firstDate();
    if (!refDate.isValid())
        return QDate();

    const int columnForFirstOfShownMonth = columnForFirstOfMonth(refDate);
    if (columnForFirstOfShownMonth < 0)
        row -= 1;

    const int requestedDay = 7 * (row) + column - columnForFirstOfShownMonth - refDate.day() + 1;
    return refDate.addDays(requestedDay);
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
QDate CLunarCalendarModel::firstDate() const
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

QDate CLunarCalendarModel::endDate() const
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
        firstDate().weekNumber(&y);
        if(firstDate().year() != y)
            return endDate().weekNumber() + 1;
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
            return date.weekNumber() - firstDate().weekNumber() + 1 + 1;
        }
    }
    return endDate().weekNumber() - firstDate().weekNumber() + 1;
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

QTextCharFormat CLunarCalendarModel::formatForCell(int row, int col) const
{
    QPalette pal;
    QPalette::ColorGroup cg = QPalette::Active;
    
    if(dateForCell(row, col).month() != m_ShownMonth)
    {
        cg = QPalette::Disabled;
        //cg = QPalette::Inactive;
    }

    QTextCharFormat format;
    format.setBackground(pal.brush(cg, QPalette::Base));
    format.setForeground(pal.brush(cg, QPalette::Text));
    
    return format;
}
