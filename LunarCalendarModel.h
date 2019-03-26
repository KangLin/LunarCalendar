#ifndef CCALENDARMODEL_H
#define CCALENDARMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QLocale>
#include <QTextCharFormat>

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
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    int showMonth(int year, int month);
    int setDate(const QDate &d);
    QDate GetDate();
    int setMinimumDate(const QDate &date);
    int setMaximumDate(const QDate &date);

    int setRange(const QDate &min, const QDate &max);
    
private:
    void internalUpdate();
    QDate dateForCell(int row, int column) const;
    QDate firstDate() const;
    QDate endDate() const;
    int WeeksOfMonth();
    int columnForFirstOfMonth(const QDate &date) const;
    int columnForDayOfWeek(Qt::DayOfWeek day) const;
    QTextCharFormat formatForCell(int row, int col) const;
    
private:
    QDate m_Date;
    QDate m_MinimumDate;
    QDate m_MaximumDate;
    int m_ShownYear;
    int m_ShownMonth;
    int m_ColumnCount, m_RowCount;
    Qt::DayOfWeek m_FirstDay;
    QLocale m_Locale;
};

#endif // CCALENDARMODEL_H
