#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LunarCalendar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new CLunarCalendar(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
