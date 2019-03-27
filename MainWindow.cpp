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
    pLunarCalendar->setShowGrid(true);
    //pLunarCalendar->setDateRange(QDate(2017, 3, 11), QDate(2018, 3, 20));
}

MainWindow::~MainWindow()
{
    delete ui;
}
