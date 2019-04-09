#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LunarCalendar.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CLunarCalendar* pLunarCalendar = new CLunarCalendar(this);
    
    setCentralWidget(pLunarCalendar);
//    pLunarCalendar->SetShowToday(false);
//    pLunarCalendar->SetSelectedDate(QDate::currentDate());
//    pLunarCalendar->SetShowGrid(true);
//    pLunarCalendar->SetDateRange(QDate(2017, 3, 11), QDate(2019, 4, 20));
//    pLunarCalendar->SetShowWeekHead(false);
//    pLunarCalendar->SetShowWeeks(false);
//    pLunarCalendar->SetShowHead(false);
    pLunarCalendar->AddAnniversary(4, 1, "my birth");
    pLunarCalendar->AddAnniversary(4, 5, "you birth");

}

MainWindow::~MainWindow()
{
    delete ui;
}
