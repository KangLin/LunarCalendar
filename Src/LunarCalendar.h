#ifndef LUNARCALENDAR_H
#define LUNARCALENDAR_H

#include <QWidget>
#include <QMap>
#include <QDate>
#include "lunarcalendar_export.h"

namespace Ui {
class CLunarCalendar;
}

class LUNARCALENDAR_EXPORT CLunarCalendar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QDate selectedDate WRITE SetSelectedDate READ SelectedDate)
    Q_PROPERTY(QString selectLunar READ SelectedLunar)
    Q_PROPERTY(int yearShow READ YearShown)
    Q_PROPERTY(int monthShow READ MonthShown)
    Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ FirstDayOfWeek WRITE SetFirstDayOfWeek)
    Q_PROPERTY(QDate minimumDate READ MinimumDate WRITE SetMinimumDate)
    Q_PROPERTY(QDate maximumDate READ MaximumDate WRITE SetMaximumDate)
    
public:
    explicit CLunarCalendar(QWidget *parent = nullptr);
    virtual ~CLunarCalendar() override;
    
    static void InitResource();
    static void CLeanResource();
    
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
    int AddLunarAnniversary(int month, int day, const QString &szName);
    
    enum _CalendarType{
        CalendarTypeSolar = 0x01,
        CalendarTypeLunar = 0x02
    };
    int SetCalendarType(_CalendarType type);
    _CalendarType GetCalendarType();
    enum _VIEW_TYPE{
        ViewTypeMonth,
        ViewTypeWeek
    };
    int SetViewType(_VIEW_TYPE type);
    _VIEW_TYPE GetViewType();
    
    int LoadCalendarTable(const QString& szFile);
    int GenerateCalendarTable(const QString& szFile, int nThreadNumber = 2);
    
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
    int UpdateViewModel();
    int UpdateMonthMenu();
    int EnableMonthMenu();
    int UpdateSelect();

private:
    Ui::CLunarCalendar *ui;
    int m_oldRow, m_oldCol;
    bool m_bShowToday;
    //int m_nUpdate;
};

#endif // LUNARCALENDAR_H
