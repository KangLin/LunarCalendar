#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDate>
#include <QIcon>

#ifdef RABBITCOMMON
    #include "DlgAbout/DlgAbout.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pLunarCalendar = new CLunarCalendar(this);
    
#ifdef RABBITCOMMON
    menuBar()->addAction(tr("Update"), this, SLOT(slotUpdate()));
    menuBar()->addAction(tr("About"), this, SLOT(slotAbout()));
#endif
    
    setCentralWidget(m_pLunarCalendar);
//    m_pLunarCalendar->SetShowToday(false);
//    m_pLunarCalendar->SetShowTools(false);
//    m_pLunarCalendar->SetSelectedDate(QDate::currentDate());
//    m_pLunarCalendar->SetShowGrid(true);
//    m_pLunarCalendar->SetDateRange(QDate(2017, 3, 11), QDate(2019, 4, 20));
//    m_pLunarCalendar->SetShowWeekHead(false);
//    m_pLunarCalendar->SetShowWeeks(false);
//    m_pLunarCalendar->SetShowHead(false);
    //m_pLunarCalendar->AddAnniversary(4, 1, "my birth");
    //m_pLunarCalendar->AddAnniversary(4, 25, "you birth");
    //m_pLunarCalendar->SetCalendarType(CLunarCalendar::CalendarTypeLunar);
    
    bool check = connect(m_pLunarCalendar, SIGNAL(sigSelectionChanged()),
            this, SLOT(slotUpdateCalendar()));
    Q_ASSERT(check);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAbout()
{
#ifdef RABBITCOMMON
    CDlgAbout about(this);
    about.m_AppIcon = windowIcon().pixmap(windowIcon().availableSizes().at(0));
    about.m_szHomePage = "https://github.com/KangLin/LunarCalendar";
    about.exec();
#endif
}

void MainWindow::slotUpdate()
{
#ifdef RABBITCOMMON
    CFrmUpdater *fu = new CFrmUpdater();
    fu->SetTitle(qApp->applicationDisplayName(),
                 windowIcon().pixmap(windowIcon().availableSizes().at(0)));
    fu->show();
#endif
}

void MainWindow::slotUpdateCalendar()
{
    qDebug() << m_pLunarCalendar->SelectedLunar();
}
