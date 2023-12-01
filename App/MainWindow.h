// 作者：康林 <kl222@126.com>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LunarCalendar.h"

namespace Ui {
class MainWindow;
}

/*!
 * \ingroup Example
 */
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
    void slotHeadpositionNot();
    void slotHeadpositionTop();
    void slotHeadpositionDown();
    void slotHeadpositionLeft();
    void slotHeadpositionRight();
    
private:
    Ui::MainWindow *ui;
    
    //! [Instance CLunarCalendar]
    CLunarCalendar* m_pLunarCalendar;
    //! [Instance CLunarCalendar]
};

#endif // MAINWINDOW_H
