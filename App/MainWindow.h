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
    
private:
    Ui::MainWindow *ui;
public:
    CLunarCalendar* m_pLunarCalendar;
};

#endif // MAINWINDOW_H
