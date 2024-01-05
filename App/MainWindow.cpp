// 作者：康林 <kl222@126.com>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLoggingCategory>
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

static Q_LOGGING_CATEGORY(Logger, "App.MainWindow")
    
//! [Implement the onHandle function]
uint CHandler::onHandle(/*in*/const QDate& d,
                        /*out*/QStringList& tasks) {

    // 纪念日（以年为周期）
    if(10 == d.month() && 25 == d.day())
    {
        tasks << "抗美援朝纪念日";
        return 0;
    }

    int lunarYear = 0;
    int lunarMonth = 0;
    int lunarDay = 0;
    CLunarCalendar::GetLunar(d, lunarYear, lunarMonth, lunarDay);    
    // 纪念日（以农历年为周期）
    if(8 == lunarMonth && 22 == lunarDay)
    {
        tasks << "结婚纪念日";
        return 0;
    }

    // 以月为周期
    if(21 == d.day())
    {
        // 表示只显示圆点，不显示内容
        return 1;
    }
    
    // 以周为周期
    if(Qt::Wednesday == d.dayOfWeek())
    {
        // 表示只显示圆点，不显示内容。建议用上面的方式
        tasks << QString();
        return 0;
    }
    
    // 单个任务
    if(2015 == d.year() && 10 == d.month() && 22 == d.day())
    {
        // 表示只显示圆点，不显示内容
        return 1;
    }
    
    // 没有任务
    return 0;
}
//! [Implement the onHandle function]

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    //! [Instance CLunarCalendar]
    m_pLunarCalendar = new CLunarCalendar(this);
    if(!m_pLunarCalendar)
        throw std::bad_alloc();
    //! [Instance CLunarCalendar]
    
    QMenu* pViewMenu = menuBar()->addMenu(tr("View"));
    QAction* pAction = pViewMenu->addAction(tr("Lunar"),
                                            this, SLOT(slotActionLunar(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    
    pAction = pViewMenu->addAction(tr("Solar"),
                                   this, SLOT(slotActionSolar(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
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

    //! [Set UI]
    pAction = pViewMenu->addAction(tr("Show Head"),
                                   m_pLunarCalendar, SLOT(ShowHead(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show Tools"),
                                   m_pLunarCalendar, SLOT(ShowTools(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show today"),
                                   m_pLunarCalendar, SLOT(ShowToday(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show date"),
                                   m_pLunarCalendar, SLOT(ShowDate(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show time"),
                                   m_pLunarCalendar, SLOT(ShowTime(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show week head"),
                                   m_pLunarCalendar, SLOT(ShowWeekHead(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show weeks"),
                                   m_pLunarCalendar, SLOT(ShowWeeks(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Show grid"),
                                   m_pLunarCalendar, SLOT(ShowGrid(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(false);
    pAction = pViewMenu->addAction(tr("Show tooltip"),
                                   m_pLunarCalendar, SLOT(EnableToolTip(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = pViewMenu->addAction(tr("Use backgroup image"),
                                   m_pLunarCalendar, SLOT(ShowBackgroupImage(bool)));
    pAction->setCheckable(true);
    pAction->setChecked(false);
    m_pLunarCalendar->ShowBackgroupImage(false);

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
    //! [Set UI]
    
    //! [Enable holidays and solar term]
    pViewMenu->addAction(tr("Clear holidays"), m_pLunarCalendar, SLOT(ClearHolidays()));
    pAction = pViewMenu->addAction(tr("Enable holidays"), m_pLunarCalendar, SLOT(EnableHolidays(bool)));
    pAction->setChecked(true);
    pAction->setCheckable(true);
    pAction = pViewMenu->addAction(tr("Enable solar term"), m_pLunarCalendar, SLOT(EnableSolarTerm(bool)));
    pAction->setChecked(true);
    pAction->setCheckable(true);
    //! [Enable holidays and solar term]

    pViewMenu->addSeparator();
    pViewMenu->addAction(RabbitCommon::CTools::Instance()->AddStyleMenu(pViewMenu));
    pViewMenu->addAction(tr("Genetate cache table"), this, SLOT(slotGenerateCalendarTable()));

#ifdef RABBITCOMMON
    QMenu* pHelp = menuBar()->addMenu(tr("Help"));
    CFrmUpdater updater;
    pHelp->addAction(updater.windowIcon(), tr("Update"),
                     this, SLOT(slotUpdate()));
    pHelp->addAction(windowIcon(), tr("About"), this, SLOT(slotAbout()));
    pHelp->addMenu(RabbitCommon::CTools::Instance()->GetLogMenu(this));
#endif

    //m_pLunarCalendar->setLocale(QLocale("zh_CN"));
    m_pLunarCalendar->SetSelectedDate(QDate(2015, 10, 8));

    //! [Instance CHandler]
    m_Hnadler = QSharedPointer<CHandler>(new CHandler());
    //! [Instance CHandler]
    //! [Set user defined tasks with CTaskHandler]
    m_pLunarCalendar->SetTaskHandle(m_Hnadler);
    //! [Set user defined tasks with CTaskHandler]

    //*! [User defined tasks]
    m_pLunarCalendar->SetTaskHandle([](const QDate& d,
                                       QStringList& tasks)->uint {
        int lunarYear = 0;
        int lunarMonth = 0;
        int lunarDay = 0;
        CLunarCalendar::GetLunar(d, lunarYear, lunarMonth, lunarDay);

        // 节日（以年为周期）
        if(d.month() == 10 && d.day() == 26)
        {
            tasks << "环卫工人节";
            return 0;
        }
        
        // 农历节日（以农历年为周期）
        if(9 == lunarMonth && 9 == lunarDay)
        {
            tasks << "老人节" << "老年节" << "登高节" << "踏秋";
            return 0;
        }
        
        // 纪念日（以年为周期）
        if(d.day() == 10 && d.month() == 10)
        {
            // 表示显示圆点，也显示内容
            tasks << "辛亥革命纪念日";
            return 0;
        }
          
        // 生日（纪念日：以农历年为周期）
        if(8 == lunarMonth && 23 == lunarDay)
        {
            tasks << "爸生日";
            return 0;
        }

        // 以月为周期
        if(d.day() == 11)
        {
            // 表示只显示圆点，不显示内容
            return 1;
        }

        // 以周为周期
        if(Qt::Monday == d.dayOfWeek())
        {
            // 表示只显示圆点，不显示内容。建议用上面的方式
            tasks << QString();
            return 0;
        }

        // 单个任务
        if(d.day() == 12 && d.month() == 10 && d.year() == 2015)
        {
            // 表示只显示圆点，不显示内容
            return 1;
        }

        // 没有任务
        return 0;});
    m_pLunarCalendar->Update();
    //! [User defined tasks] */

    //m_pLunarCalendar->SetCalendarType(CLunarCalendar::CalendarTypeLunar);
    //m_pLunarCalendar->SetViewType(CLunarCalendar::ViewTypeWeek);
    m_pLunarCalendar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setCentralWidget(m_pLunarCalendar);
    
    QFont font = m_pLunarCalendar->font();
    font.setItalic(true);
    font.setStrikeOut(true);
    int nFontSize = 12;
    font.setPointSize(nFontSize);
    //m_pLunarCalendar->setFont(font);

    //! [sigSelectionChanged]
    bool check = connect(m_pLunarCalendar, SIGNAL(sigSelectionChanged()),
                         this, SLOT(slotUpdateCalendar()));
    Q_ASSERT(check);
    //! [sigSelectionChanged]
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! \name help
//! @{
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
//! @}

//! [slotUpdateCalendar]
void MainWindow::slotUpdateCalendar()
{
    int y, m, d;
    m_pLunarCalendar->SelectedLunar(y, m, d);
    qDebug(Logger) << "\n"
                   << tr("Solar:") << m_pLunarCalendar->SelectedDate() << "\n"
                   << tr("Lunar:") << m_pLunarCalendar->SelectedLunar()
                   << "(" << y << "年" << m << "月" << d << "日" << ")\n"
                   << tr("SolarTerm:") << m_pLunarCalendar->SelectedSolarTerm() << "\n"
                   << tr("Year:") << m_pLunarCalendar->GetShowYear() << "\n"
                   << tr("Month:") << m_pLunarCalendar->GetShowMonth() << "\n"
                   << tr("MinimumDate:") << m_pLunarCalendar->MinimumDate() << "\n"
                   << tr("MaximumDate:") << m_pLunarCalendar->MaximumDate()
        ;
}
//! [slotUpdateCalendar]

void MainWindow::slotActionLunar(bool checked)
{
    if(checked)
        m_pLunarCalendar->SetCalendarType(
            static_cast<CLunarCalendar::_CalendarType>(
                static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeLunar)
                | static_cast<int>(m_pLunarCalendar->GetCalendarType())));
    else
        m_pLunarCalendar->SetCalendarType(
            static_cast<CLunarCalendar::_CalendarType>(
                ~static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeLunar)
                & static_cast<int>(m_pLunarCalendar->GetCalendarType())));
}

void MainWindow::slotActionSolar(bool checked)
{
    if(checked)
        m_pLunarCalendar->SetCalendarType(
            static_cast<CLunarCalendar::_CalendarType>(
                static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeSolar)
                | static_cast<int>(m_pLunarCalendar->GetCalendarType())));
    else
        m_pLunarCalendar->SetCalendarType(
            static_cast<CLunarCalendar::_CalendarType>(
                ~static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeSolar)
                & static_cast<int>(m_pLunarCalendar->GetCalendarType())));
}

void MainWindow::slotViewMonth()
{
    m_pLunarCalendar->SetViewType(CLunarCalendar::_VIEW_TYPE::ViewTypeMonth);
}

void MainWindow::slotViewWeek()
{
    m_pLunarCalendar->SetViewType(CLunarCalendar::_VIEW_TYPE::ViewTypeWeek);
}

void MainWindow::slotHeadpositionNot()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::_HEAD_position::Not);
}

void MainWindow::slotHeadpositionTop()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::_HEAD_position::Top);
}

void MainWindow::slotHeadpositionDown()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::_HEAD_position::Down);
}

void MainWindow::slotHeadpositionLeft()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::_HEAD_position::Left);
}

void MainWindow::slotHeadpositionRight()
{
    m_pLunarCalendar->SetHeadposition(CLunarCalendar::_HEAD_position::Right);
}

void MainWindow::slotGenerateCalendarTable()
{
    //TODO：自动生成指定日期内的农历缓存表，生成完后，把cache.dat放到Src\Resource\Data目录下
    //*
    m_pLunarCalendar->SetDateRange(QDate(2000, 1, 1), QDate(2119, 1, 1));
    QString szFileCache = qApp->applicationDirPath() + QDir::separator() + "cache.dat";
    m_pLunarCalendar->GenerateCalendarTable(szFileCache, 1, true, false);
    qInfo(Logger) << "Generate calendar cache table:" << szFileCache;
    m_pLunarCalendar->LoadCalendarTable(szFileCache);
    //*/
}
