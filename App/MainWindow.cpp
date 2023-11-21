// 作者：康林 <kl222@126.com>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDate>
#include <QIcon>
#include <QDir>
#include <QApplication>
#include <QActionGroup>
#ifdef RABBITCOMMON
    #include "DlgAbout.h"
    #include "FrmUpdater.h"
    #include "RabbitCommonTools.h"
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

    QActionGroup* pViewHeadposition = new QActionGroup(this);
    QMenu* pViewHeadpositionMenu = pViewMenu->addMenu(tr("Head position"));
    QAction* pHeadNot = pViewHeadpositionMenu->addAction(tr("Not"),
                                             this, SLOT(slotHeadpositionNot()));
    pHeadNot->setCheckable(true);
    QAction* pHeadTop = pViewHeadpositionMenu->addAction(tr("Top"),
                                             this, SLOT(slotHeadpositionTop()));
    pHeadTop->setCheckable(true);
    pHeadTop->setChecked(true);
    QAction* pHeadDown = pViewHeadpositionMenu->addAction(tr("Down"),
                                            this, SLOT(slotHeadpositionDown()));
    pHeadDown->setCheckable(true);
    QAction* pHeadLeft = pViewHeadpositionMenu->addAction(tr("Left"),
                                            this, SLOT(slotHeadpositionLeft()));
    pHeadLeft->setCheckable(true);
    QAction* pHeadRight = pViewHeadpositionMenu->addAction(tr("Right"),
                                           this, SLOT(slotHeadpositionRight()));
    pHeadRight->setCheckable(true);
    pViewHeadposition->addAction(pHeadNot);
    pViewHeadposition->addAction(pHeadTop);
    pViewHeadposition->addAction(pHeadDown);
    pViewHeadposition->addAction(pHeadLeft);
    pViewHeadposition->addAction(pHeadRight);

    pViewMenu->addSeparator();
    pViewMenu->addAction(RabbitCommon::CTools::Instance()->AddStyleMenu(pViewMenu));

#ifdef RABBITCOMMON
    QMenu* pHelp = menuBar()->addMenu(tr("Help"));
    CFrmUpdater updater;
    pHelp->addAction(updater.windowIcon(), tr("Update"),
                     this, SLOT(slotUpdate()));
    pHelp->addAction(windowIcon(), tr("About"), this, SLOT(slotAbout()));
    pHelp->addMenu(RabbitCommon::CTools::Instance()->GetLogMenu(this));
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
//    m_pLunarCalendar->AddAnniversary(10, 19, "my birth");
//    m_pLunarCalendar->AddAnniversary(10, 25, "you birth");
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
    QIcon icon = QIcon::fromTheme("calendar");
    if(icon.isNull()) return;
    auto sizeList = icon.availableSizes();
    if(sizeList.isEmpty()) return;
    QPixmap p = icon.pixmap(*sizeList.begin());
    about.m_AppIcon = p.toImage();
    about.m_szHomePage = "https://github.com/KangLin/LunarCalendar";
    about.m_szBuildTime = QString("%1/%2").arg(__DATE__, __TIME__);
    about.m_szCopyrightStartTime = "2019";
    about.m_szVersionRevision = LunarCalendar_REVISION;
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
    QIcon icon = QIcon::fromTheme("calendar");
    if(icon.isNull()) return;
    auto sizeList = icon.availableSizes();
    if(sizeList.isEmpty()) return;
    QPixmap p = icon.pixmap(*sizeList.begin());
    pUpdate->SetTitle(p.toImage());
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

void MainWindow::slotHeadpositionNot()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::Not);
}

void MainWindow::slotHeadpositionTop()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::Top);
}

void MainWindow::slotHeadpositionDown()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::Down);
}

void MainWindow::slotHeadpositionLeft()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::Left);
}

void MainWindow::slotHeadpositionRight()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::Right);
}
