#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDate>
#include <QIcon>
#include <QDir>
#include <QApplication>
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
    QAction* pViewMonth = pViewMenu->addAction(tr("Month"),
                                            this, SLOT(slotViewMonth()));
    pViewMonth->setCheckable(true);
    pViewMonth->setChecked(true);
    pViewTypeGroup->addAction(pViewMonth);
    QAction* pViewWeek = pViewMenu->addAction(tr("Week"),
                                            this, SLOT(slotViewWeek()));
    pViewWeek->setCheckable(true);
    pViewTypeGroup->addAction(pViewWeek);
    pViewMenu->addSeparator();
    
    QAction* pViewBackgroup = pViewMenu->addAction(tr("Use backgroup image"),
                                       this, SLOT(slotViewBackgroup(bool)));
    pViewBackgroup->setCheckable(true);
    pViewBackgroup->setChecked(false);
    m_pLunarCalendar->SetShowBackgroupImage(false);

    QActionGroup* pViewHeadPostion = new QActionGroup(this);
    QMenu* pViewHeadPostionMenu = pViewMenu->addMenu(tr("Head postion"));
    QAction* pHeadNot = pViewHeadPostionMenu->addAction(tr("Not"),
                                             this, SLOT(slotHeadPostionNot()));
    pHeadNot->setCheckable(true);
    QAction* pHeadTop = pViewHeadPostionMenu->addAction(tr("Top"),
                                             this, SLOT(slotHeadPostionTop()));
    pHeadTop->setCheckable(true);
    pHeadTop->setChecked(true);
    QAction* pHeadDown = pViewHeadPostionMenu->addAction(tr("Down"),
                                            this, SLOT(slotHeadPostionDown()));
    pHeadDown->setCheckable(true);
    QAction* pHeadLeft = pViewHeadPostionMenu->addAction(tr("Left"),
                                            this, SLOT(slotHeadPostionLeft()));
    pHeadLeft->setCheckable(true);
    QAction* pHeadRight = pViewHeadPostionMenu->addAction(tr("Right"),
                                           this, SLOT(slotHeadPostionRight()));
    pHeadRight->setCheckable(true);
    pViewHeadPostion->addAction(pHeadNot);
    pViewHeadPostion->addAction(pHeadTop);
    pViewHeadPostion->addAction(pHeadDown);
    pViewHeadPostion->addAction(pHeadLeft);
    pViewHeadPostion->addAction(pHeadRight);
    
#ifdef RABBITCOMMON
    QMenu* pHelp = menuBar()->addMenu(tr("Help"));
    CFrmUpdater updater;
    pHelp->addAction(updater.windowIcon(), tr("Update"),
                     this, SLOT(slotUpdate()));
    pHelp->addAction(windowIcon(), tr("About"), this, SLOT(slotAbout()));
#endif

    //m_pLunarCalendar->setLocale(QLocale("zh_CN"));
//    m_pLunarCalendar->ShowToday(false);
//    m_pLunarCalendar->ShowTools(false);
//    m_pLunarCalendar->SetSelectedDate(QDate::currentDate());
//    m_pLunarCalendar->SetShowGrid(true);
    
    //TODO：自动生成指定日期内的农历缓存表，生成完后，把cache.dat放到Src\Resource\Data目录下
//    m_pLunarCalendar->SetDateRange(QDate(2000, 1, 1), QDate(2119, 1, 1));
//    m_pLunarCalendar->GenerateCalendarTable(
//                qApp->applicationDirPath() + QDir::separator() + "cache.dat",
//                8, true, false);
        
//    m_pLunarCalendar->LoadCalendarTable(qApp->applicationDirPath() + QDir::separator() + "cache.dat");
//    m_pLunarCalendar->ShowWeekHead(false);
//    m_pLunarCalendar->ShowWeeks(false);
//    m_pLunarCalendar->ShowHead(false);
//    m_pLunarCalendar->ShowTime(false);
//    m_pLunarCalendar->AddAnniversary(8, 19, "my birth");
//    m_pLunarCalendar->AddAnniversary(4, 25, "you birth");
//    m_pLunarCalendar->SetCalendarType(CLunarCalendar::CalendarTypeLunar);
//    m_pLunarCalendar->SetViewType(CLunarCalendar::ViewTypeWeek);
    //m_pLunarCalendar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setCentralWidget(m_pLunarCalendar);
    
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
    about.m_AppIcon = QImage(":/image/Calendar");
    about.m_szHomePage = "https://github.com/KangLin/LunarCalendar";
    #if defined (Q_OS_ANDROID)
        about.showMaximized();
    #endif
        about.exec();
#endif
}

void MainWindow::slotUpdate()
{
#ifdef RABBITCOMMON
    CFrmUpdater* pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/image/Calendar"));
    #if defined (Q_OS_ANDROID)
        pUpdate->showMaximized();
    #else
        pUpdate->show();
    #endif
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

void MainWindow::slotViewBackgroup(bool checked)
{
    m_pLunarCalendar->SetShowBackgroupImage(checked);
}

void MainWindow::slotHeadPostionNot()
{
    m_pLunarCalendar->SetHeadPostion(CLunarCalendar::Not);
}

void MainWindow::slotHeadPostionTop()
{
    m_pLunarCalendar->SetHeadPostion(CLunarCalendar::Top);
}

void MainWindow::slotHeadPostionDown()
{
    m_pLunarCalendar->SetHeadPostion(CLunarCalendar::Down);
}

void MainWindow::slotHeadPostionLeft()
{
    m_pLunarCalendar->SetHeadPostion(CLunarCalendar::Left);
}

void MainWindow::slotHeadPostionRight()
{
    m_pLunarCalendar->SetHeadPostion(CLunarCalendar::Right);
}
