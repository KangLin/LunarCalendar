#ifndef CCALENDARMODEL_H
#define CCALENDARMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QLocale>
#include <QTextCharFormat>
#include <QVector>

class CLunarCalendarModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    explicit CLunarCalendarModel(QObject *parent = nullptr);
    
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    
    enum ROLE
    {
        SolarRole = Qt::UserRole + 1,
        LunarRole,
        LunarColorRole,
        LunarFontRole,
        Anniversary
    };
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    int showMonth(int year, int month);
    int GetShowMonth();
    int GetShowYear();
    int setDate(const QDate &d);
    QDate GetDate() const;
    int SetMinimumDate(const QDate &date);
    QDate GetMinimumDate();
    int SetMaximumDate(const QDate &date);
    QDate GetMaximumDate();
    int setRange(const QDate &min, const QDate &max);
    Qt::DayOfWeek firstDayOfWeek() const;
    void setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);

    QDate dateForCell(int row, int column) const;
    void cellForDate(const QDate &date, int *row, int *column) const;
    
    int AddHoliday(int month, int day, const QString &szName);
    int AddAnniversary(int month, int day, const QString &szName);
    
private:
    void internalUpdate();
    QDate firstDateMonth() const;
    QDate endDateMonth() const;
    int WeeksOfMonth();
    int columnForFirstOfMonth(const QDate &date) const;
    int columnForDayOfWeek(Qt::DayOfWeek day) const;
    QTextCharFormat formatForCell(QDate d, int row, int col) const;
    QColor GetHeight() const;
    int InitHoliday();
    
private:
    QDate m_Date;
    QDate m_MinimumDate;
    QDate m_MaximumDate;
    int m_ShownYear;
    int m_ShownMonth;
    int m_ColumnCount, m_RowCount;
    Qt::DayOfWeek m_FirstDay;
    QLocale m_Locale;
    struct _DAY
    {
        int Solar;
        QString szSolarHoliday;
        QString szLunar;
        QString szLunarHoliday;
        QString szAnniversary;
    };
    QVector<_DAY> m_Day;
    _DAY GetDay(int row, int col) const;
    
    QMap<int, QMap<int, QString> > m_Holiday;
    QMap<int, QMap<int, QString> > m_Anniversary;
};

#endif // CCALENDARMODEL_H
