#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LunarCalendar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private Q_SLOTS:
    void slotUpdateCalendar();
    void slotAbout();
    void slotUpdate();
    void slotActionLunar(bool checked);
    void slotActionSolar(bool checked);
    void slotViewMonth();
    void slotViewWeek();
    
private:
    Ui::MainWindow *ui;
public:
    CLunarCalendar* m_pLunarCalendar;
};

#endif // MAINWINDOW_H
