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
    ~CLunarCalendar();
    
private slots:
    void on_tbNext_clicked();
    void on_tbPrevious_clicked();
    void on_pbToday_clicked();
    
    void on_cbMonth_currentIndexChanged(int index);
    
    void on_spYear_valueChanged(int);
    
private:
    int ChangeTitle(const QDate &date);
    int ChangeMonth();
    
private:
    Ui::CLunarCalendar *ui;
};

#endif // LUNARCALENDAR_H
