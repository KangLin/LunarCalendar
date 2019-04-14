#ifndef LUNARCALENDAR_H
#define LUNARCALENDAR_H

#include <QWidget>
#include "lunarcalendar_export.h"
#include <QMap>

namespace Ui {
class CLunarCalendar;
}

class LUNARCALENDAR_EXPORT CLunarCalendar : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLunarCalendar(QWidget *parent = nullptr);
    virtual ~CLunarCalendar() override;
    
    static void InitResource();
    
    QDate SelectedDate() const;
    void SetSelectedDate(const QDate &date);
    
    QString SelectedLunar();
    
    int YearShown() const;
    int MonthShown() const;
    
    Qt::DayOfWeek FirstDayOfWeek() const;
    void SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek);
    
    void SetShowGrid(bool show);
    void SetShowToday(bool bShow);
    void SetShowWeekHead(bool bShow);
    void SetShowWeeks(bool bShow);
    void SetShowHead(bool bShow);
    void SetShowTools(bool bShow);
    
    QDate MinimumDate() const;
    void SetMinimumDate(const QDate &date);

    QDate MaximumDate() const;
    void SetMaximumDate(const QDate &date);
    void SetDateRange(const QDate &min, const QDate &max);

    int AddHoliday(int month, int day, const QString &szName);
    int AddAnniversary(int month, int day, const QString &szName);

Q_SIGNALS:
    void sigSelectionChanged();

private slots:
    void on_tbNext_clicked();
    void on_tbPrevious_clicked();
    void on_pbToday_clicked();
    void on_cbMonth_currentIndexChanged(int index);
    void on_spYear_valueChanged(int value);
    void on_tvMonth_pressed(const QModelIndex &index);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    int ShowSelectTitle();
    int ChangeMonth();
    int UpdateMonthMenu();
    int UpdateSelect();

private:
    Ui::CLunarCalendar *ui;
    int m_oldRow, m_oldCol;
    bool m_bShowToday;
    bool m_bUpdate;
};

#endif // LUNARCALENDAR_H
