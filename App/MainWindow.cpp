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
    //pLunarCalendar->setShowToday(false);
    //pLunarCalendar->setSelectedDate(QDate::currentDate());
    //pLunarCalendar->setShowGrid(true);
    //pLunarCalendar->setDateRange(QDate(2017, 3, 11), QDate(2019, 4, 20));
    //pLunarCalendar->setShowWeekHead(false);
    //pLunarCalendar->setShowWeeks(false);
    //pLunarCalendar->setShowHead(false);
    pLunarCalendar->AddAnniversary(4, 1, "my birth");
    pLunarCalendar->AddAnniversary(4, 5, "you birth");

}

MainWindow::~MainWindow()
{
    delete ui;
}
