#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pLunarCalendar = new CLunarCalendar(this);
    
    setCentralWidget(m_pLunarCalendar);
//    m_pLunarCalendar->SetShowToday(false);
//    m_pLunarCalendar->SetShowTools(false);
//    m_pLunarCalendar->SetSelectedDate(QDate::currentDate());
//    m_pLunarCalendar->SetShowGrid(true);
//    m_pLunarCalendar->SetDateRange(QDate(2017, 3, 11), QDate(2019, 4, 20));
//    m_pLunarCalendar->SetShowWeekHead(false);
//    m_pLunarCalendar->SetShowWeeks(false);
//    m_pLunarCalendar->SetShowHead(false);
    m_pLunarCalendar->AddAnniversary(4, 1, "my birth");
    m_pLunarCalendar->AddAnniversary(4, 5, "you birth");
    
    bool check = connect(m_pLunarCalendar, SIGNAL(sigSelectionChanged()),
            this, SLOT(slotUpdateCalendar()));
    Q_ASSERT(check);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotUpdateCalendar()
{
    qDebug() << m_pLunarCalendar->SelectedLunar();
}
