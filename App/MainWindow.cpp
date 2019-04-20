#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDate>
#include <QIcon>
#include <QDir>
#include <QApplication>
#ifdef RABBITCOMMON
    #include "DlgAbout/DlgAbout.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pLunarCalendar = new CLunarCalendar(this);
    
    QMenu* pViewMenu = menuBar()->addMenu(tr("View"));
    QAction* pActionLunar = pViewMenu->addAction(tr("Lunar"),
                                                 this, SLOT(slotActionLunar(bool)));
    pActionLunar->setCheckable(true);
    pActionLunar->setChecked(true);
    
    QAction* pActionSolar = pViewMenu->addAction(tr("Solar"),
                                                 this, SLOT(slotActionSolar(bool)));
    pActionSolar->setCheckable(true);
    pActionSolar->setChecked(true);
    pViewMenu->addSeparator();
    
    QActionGroup* pViewTypeGroup = new QActionGroup(this);
    QAction* pViewMonth = pViewMenu->addAction(tr("Month"), this, SLOT(slotViewMonth()));
    pViewMonth->setCheckable(true);
    pViewMonth->setChecked(true);
    pViewTypeGroup->addAction(pViewMonth);
    QAction* pViewWeek = pViewMenu->addAction(tr("Week"), this, SLOT(slotViewWeek()));
    pViewWeek->setCheckable(true);
    pViewTypeGroup->addAction(pViewWeek);
    
#ifdef RABBITCOMMON
    QMenu* pHelp = menuBar()->addMenu(tr("Help"));
    m_frmUpdate.SetTitle(QPixmap(":/image/Calendar"));
    pHelp->addAction(m_frmUpdate.windowIcon(), tr("Update"), this, SLOT(slotUpdate()));
    pHelp->addAction(windowIcon(), tr("About"), this, SLOT(slotAbout()));
#endif
    
    setCentralWidget(m_pLunarCalendar);
//    m_pLunarCalendar->SetShowToday(false);
//    m_pLunarCalendar->SetShowTools(false);
//    m_pLunarCalendar->SetSelectedDate(QDate::currentDate());
//    m_pLunarCalendar->SetShowGrid(true);
//    m_pLunarCalendar->SetDateRange(QDate(2000, 1, 1), QDate(2120, 1, 1));
//    m_pLunarCalendar->GenerateCalendarTable(qApp->applicationDirPath() + QDir::separator() + "cache.dat", 2, false);
//    m_pLunarCalendar->LoadCalendarTable(qApp->applicationDirPath() + QDir::separator() + "cache.dat");
//    m_pLunarCalendar->SetShowWeekHead(false);
//    m_pLunarCalendar->SetShowWeeks(false);
//    m_pLunarCalendar->SetShowHead(false);
//    m_pLunarCalendar->AddAnniversary(4, 1, "my birth");
//    m_pLunarCalendar->AddAnniversary(4, 25, "you birth");
    //m_pLunarCalendar->SetCalendarType(CLunarCalendar::CalendarTypeLunar);
    //m_pLunarCalendar->SetViewType(CLunarCalendar::ViewTypeWeek);
    
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
    about.m_AppIcon = QPixmap(":/image/Calendar");
    about.m_szHomePage = "https://github.com/KangLin/LunarCalendar";
    about.exec();
#endif
}

void MainWindow::slotUpdate()
{
#ifdef RABBITCOMMON
    
    m_frmUpdate.show();
#endif
}

void MainWindow::slotUpdateCalendar()
{
    qDebug() << m_pLunarCalendar->SelectedLunar();
}

void MainWindow::slotActionLunar(bool checked)
{
    if(checked)
        m_pLunarCalendar->SetCalendarType(
                static_cast<CLunarCalendar::_CalendarType>(
                          CLunarCalendar::CalendarTypeLunar
                   | m_pLunarCalendar->GetCalendarType()));
    else
        m_pLunarCalendar->SetCalendarType(
                static_cast<CLunarCalendar::_CalendarType>(
                          ~CLunarCalendar::CalendarTypeLunar
                   & m_pLunarCalendar->GetCalendarType()));
}

void MainWindow::slotActionSolar(bool checked)
{
    if(checked)
        m_pLunarCalendar->SetCalendarType(
                static_cast<CLunarCalendar::_CalendarType>(
                          CLunarCalendar::CalendarTypeSolar
                   | m_pLunarCalendar->GetCalendarType()));
    else
        m_pLunarCalendar->SetCalendarType(
                static_cast<CLunarCalendar::_CalendarType>(
                          ~CLunarCalendar::CalendarTypeSolar
                   & m_pLunarCalendar->GetCalendarType()));
}

void MainWindow::slotViewMonth()
{
    m_pLunarCalendar->SetViewType(CLunarCalendar::ViewTypeMonth);
}

void MainWindow::slotViewWeek()
{
    m_pLunarCalendar->SetViewType(CLunarCalendar::ViewTypeWeek);
}
