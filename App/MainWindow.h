#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LunarCalendar.h"

#ifdef RABBITCOMMON
    #include "FrmUpdater/FrmUpdater.h"
#endif

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
    void slotViewBackgroup(bool checked);
    void slotHeadPostionNot();
    void slotHeadPostionTop();
    void slotHeadPostionDown();
    void slotHeadPostionLeft();
    void slotHeadPostionRight();
    
private:
    Ui::MainWindow *ui;
    CFrmUpdater m_frmUpdate;
    CLunarCalendar* m_pLunarCalendar;
};

#endif // MAINWINDOW_H
