#ifndef LUNARCALENDAR_H
#define LUNARCALENDAR_H

#include <QWidget>

namespace Ui {
class CLunarCalendar;
}

class CLunarCalendar : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLunarCalendar(QWidget *parent = nullptr);
    virtual ~CLunarCalendar() override;
    
    QDate selectedDate() const;
public Q_SLOTS:
    void setSelectedDate(const QDate &date);

public:
    int yearShown() const;
    int monthShown() const;
    
    Qt::DayOfWeek firstDayOfWeek() const;
    void setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);
    
    void setShowGrid(bool show);
    
    QDate minimumDate() const;
    void setMinimumDate(const QDate &date);

    QDate maximumDate() const;
    void setMaximumDate(const QDate &date);
public Q_SLOTS:
    void setDateRange(const QDate &min, const QDate &max);

Q_SIGNALS:
    void sigSelectionChanged();

private slots:
    void on_tbNext_clicked();
    void on_tbPrevious_clicked();
    void on_pbToday_clicked();
    void on_cbMonth_currentIndexChanged(int index);
    void on_spYear_valueChanged(int);
    void on_tvMonth_pressed(const QModelIndex &index);
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    
private:
    int ChangeTitle();
    int ChangeMonth();
    int UpdateMonthMenu();
    int UpdateSelect();
    
private:
    Ui::CLunarCalendar *ui;
    int m_oldRow, m_oldCol;
};

#endif // LUNARCALENDAR_H
