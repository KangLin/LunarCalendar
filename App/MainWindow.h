// 作者：康林 <kl222@126.com>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "LunarCalendar.h"

namespace Ui {
class MainWindow;
}

//! [Define CTaskHandler derived class]
class CHandler : public CLunarCalendar::CTaskHandler
{
    virtual uint onHandle(/*in*/const QDate& d, /*out*/QStringList& tasks) override final;
};
//! [Define CTaskHandler derived class]

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
    void slotGenerateCalendarTable();
    
private:
    Ui::MainWindow *ui;
    
    //! [Instance CLunarCalendar]
    CLunarCalendar* m_pLunarCalendar;
    //! [Instance CLunarCalendar]
    
    //! [Defined CHandler variable]
    QSharedPointer<CHandler> m_Hnadler;
    //! [Defined CHandler variable]
};

#endif // MAINWINDOW_H
