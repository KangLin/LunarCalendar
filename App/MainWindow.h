// 作者：康林 <kl222@126.com>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LunarCalendar.h"

/*!
 * \addtogroup Example 示例程序
 * \ingroup USE_DOC
 * \details
 *
 * - 初始化资源
 * \snippet App/main.cpp CLunarCalendar::InitResource()
 * - 释放资源
 * \snippet App/main.cpp CLunarCalendar::CLeanResource()
 * - 设置界面 
 * \snippet App/MainWindow.cpp Set UI
 * - 设置周年纪念日
 * \snippet App/MainWindow.cpp Add Anniversary
 * - 设置自定义任务
 * \snippet App/MainWindow.cpp User defined tasks
 * - 处理选择事件
 *   - 连接选择信号
 *     \snippet App/MainWindow.cpp sigSelectionChanged
 *   - 处理选择事件
 *     \snippet App/MainWindow.cpp slotUpdateCalendar
 */

namespace Ui {
class MainWindow;
}

/*!
 * \ingroup Example
 *
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
    CLunarCalendar* m_pLunarCalendar;
};

#endif // MAINWINDOW_H
