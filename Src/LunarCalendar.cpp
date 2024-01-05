/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QLoggingCategory>
#include <QHeaderView>
#include <QScrollBar>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QModelIndex>
#include <QTranslator>
#include <QApplication>
#include <QDir>
#include <QStyleOption>
#include <QPainter>
#include <QLineEdit>
#include <algorithm>
#include <QGestureEvent>
#include <QSwipeGesture>
#include <QMouseEvent>
#include <QEvent>

#include "CalendarLunar.h"
#include "LunarCalendarDelegate.h"
#include "CalendarLunar.h"
#include "LunarTable.h"
#include "RabbitCommonDir.h"

static Q_LOGGING_CATEGORY(Logger, "Rabbit.LunarCalendar");

class CLunarCalendarPrivate
{
public:
    CLunarCalendarPrivate()
    {
        if(!qApp) {
            qCritical(Logger) << "Please instantiate the CLunarCalendar object after QApplication a(argc, argv) in main()";
            Q_ASSERT(false);
            return;
        }
        QString szFile = RabbitCommon::CDir::Instance()->GetDirTranslations()
                         + "/LunarCalendar_" + QLocale::system().name() + ".qm";
        if(m_Translator.load(szFile))
            qApp->installTranslator(&m_Translator);
        else
            qCritical(Logger) << "Load translator fail: %s" << szFile;
    }
    virtual ~CLunarCalendarPrivate()
    {
        qApp->removeTranslator(&m_Translator);
    }
private:
    QTranslator m_Translator;
};

static CLunarCalendarPrivate* g_pLunarCalendarPrivate = nullptr;

CLunarCalendar::CLunarCalendar(QWidget *parent) :
    QWidget(parent),
    m_cmbYear(this),
    m_tbPreYear(this),
    m_tbNextYear(this),
    m_cmbMonth(this),
    m_tbPreMonth(this),
    m_tbNextMonth(this),
    m_pbToday(this),
    m_lbDate(this),
    m_lbTime(this),
    m_View(this),
    m_pToolLayout(nullptr),
    m_pHeadLayout(nullptr),
    m_pMainLayout(nullptr),
    m_Headposition(_HEAD_position::Top),
    m_oldRow(0),
    m_oldCol(0),
    m_bShowToday(true),
    m_bShowBackgroupImage(false),
    m_TouchFunction(_TOUCH_UP_DOWN_FUNCTION::TouchChangeMounth)
{
    InitResource();
    setWindowTitle(tr("Lunar calendar"));

    //setLocale(QLocale("zh_CN"));

    m_tbPreYear.setArrowType(Qt::UpArrow);
    m_tbPreYear.setToolTip(tr("Previous year"));
    m_tbPreYear.setStatusTip(tr("Previous year"));
    m_tbNextYear.setArrowType(Qt::DownArrow);
    m_tbNextYear.setToolTip(tr("Next year"));
    m_tbNextYear.setStatusTip(tr("Next year"));
    m_tbPreMonth.setArrowType(Qt::UpArrow);
    m_tbPreMonth.setToolTip(tr("Previous month"));
    m_tbPreMonth.setStatusTip(tr("Previous month"));
    m_tbNextMonth.setArrowType(Qt::DownArrow);
    m_tbNextMonth.setToolTip(tr("Next month"));
    m_tbNextMonth.setStatusTip(tr("Next month"));

    //    QLineEdit *yearEdit = new QLineEdit;
    //    yearEdit->setAlignment(Qt::AlignCenter);
    //    m_cmbYear.setLineEdit(yearEdit);
    //    QLineEdit *monthEdit = new QLineEdit;
    //    monthEdit->setReadOnly(true);
    //    monthEdit->setAlignment(Qt::AlignCenter);
    //    m_cmbMonth.setLineEdit(monthEdit);

    //m_pbToday.setText(tr("Today"));
    m_pbToday.setIcon(QIcon::fromTheme("today"));
    m_pbToday.setToolTip(tr("Today"));
    m_pbToday.setStatusTip(tr("Today"));
    m_pbToday.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_lbDate.setAlignment(Qt::AlignCenter);
    m_lbTime.setAlignment(Qt::AlignCenter);
//    m_lbDate.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    m_lbTime.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);
    m_View.setModel(pModel);
    m_View.setItemDelegate(new CLunarCalendarDelegate(&m_View));
    /* TODO: 未能生效
    if(m_View.horizontalHeader())
        m_View.horizontalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    if(m_View.verticalHeader())
        m_View.verticalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    //*/
    m_View.setAttribute(Qt::WA_AcceptTouchEvents);
    ShowGrid(false);

    m_View.setFocusPolicy(Qt::WheelFocus);
    m_View.setSelectionBehavior(QAbstractItemView::SelectItems);
    m_View.setSelectionMode(QAbstractItemView::SingleSelection);
    m_View.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_View.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*
    m_View.horizontalHeader()->setMinimumSize(cell.size());
    m_View.horizontalHeader()->setMinimumSectionSize(cell.width());
    m_View.verticalHeader()->setMinimumSize(cell.size());
    m_View.verticalHeader()->setMinimumSectionSize(cell.height());
    */
    m_View.horizontalHeader()->setSectionsClickable(false);
    m_View.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_View.verticalHeader()->setSectionsClickable(false);
    m_View.setFrameStyle(QFrame::NoFrame);
    //m_View.grabGesture(Qt::PanGesture);
    m_View.grabGesture(Qt::SwipeGesture);
    m_View.installEventFilter(this);
    m_View.setAttribute(Qt::WA_AcceptTouchEvents, true);
    m_View.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_View.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_View.setAutoScroll(false);
    //m_View.setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //m_View.setAlternatingRowColors(true); //设置奇偶行颜色
//    qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);
//    qApp->setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);

    QFont font = m_View.font();
    float nFontSize = 0, nPixSize = 0;
    nFontSize = font.pointSize();
    nPixSize = font.pixelSize();
    qDebug(Logger) << "Font point size:" << nFontSize
                   << "pixel size:" << nPixSize;
    if(nFontSize > 0)
        font.setPointSize(nFontSize * 2 / 3);
    if(nPixSize > 0)
        font.setPixelSize(nPixSize * 2 / 3);
#if defined(Q_OS_ANDROID)
    m_View.setFont(font);
#endif
    /*
    font = m_lbDate.font();
    font.setPointSize(nFontSize);
    m_lbDate.setFont(font);
    
    font = m_lbTime.font();
    font.setPointSize(nFontSize);
    m_lbTime.setFont(font);
    
    font = m_pbToday.font();
    font.setPointSize(nFontSize);
    m_pbToday.setFont(font);
    
    font = m_cmbYear.font();
    font.setPointSize(nFontSize);
    m_cmbYear.setFont(font);
    
    font = m_cmbMonth.font();
    font.setPointSize(nFontSize);
    m_cmbMonth.setFont(font);
    //*/

    //m_View.setStyleSheet("background-color:rgba(0,0,0,0)"); //设置背景透明
    //setStyleSheet("border-image:url(d:/temp/4.jpg)"); //设置背景图片
    
    for(int i = 0; i < 12; i++)
    {
        m_cmbMonth.addItem(locale().monthName(i + 1), i + 1);
    }
    m_cmbMonth.setToolTip(tr("Month"));
    m_cmbMonth.setStatusTip(tr("Month"));
    
    m_cmbMonth.setCurrentIndex(m_cmbMonth.findData(pModel->GetDate().month()));
    SetSelectedDate(pModel->GetDate());

    SetYearRange(pModel->GetMinimumDate().year(),
                 pModel->GetMaximumDate().year());
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetDate().year()));
    m_cmbYear.setEditable(true);
    m_cmbYear.setToolTip(tr("Year"));
    m_cmbYear.setStatusTip(tr("Year"));
    
//    m_View.sizePolicy().setVerticalPolicy(QSizePolicy::Preferred);
//    m_View.sizePolicy().setHorizontalPolicy(QSizePolicy::Preferred);
    
    m_pToolLayout = new QHBoxLayout();
    m_pToolLayout->addWidget(&m_tbPreYear);
    m_pToolLayout->addWidget(&m_cmbYear);
    m_pToolLayout->addWidget(&m_tbNextYear);
    m_pToolLayout->addWidget(&m_tbPreMonth);
    m_pToolLayout->addWidget(&m_cmbMonth);
    m_pToolLayout->addWidget(&m_tbNextMonth);
    m_pToolLayout->addWidget(&m_pbToday);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_pToolLayout->setMargin(0);
#else
    m_pToolLayout->setContentsMargins(0, 0, 0, 0);
#endif
    m_pToolLayout->setSpacing(0);

    SetHeadposition();

    bool check = connect(&m_Timer, SIGNAL(timeout()),
                         this, SLOT(slotTimeout()));
    Q_ASSERT(check);
    check = connect(&m_tbPreYear, SIGNAL(clicked()),
                    this, SLOT(on_tbPreviousYear_clicked()));
    Q_ASSERT(check);
    check = connect(&m_tbNextYear, SIGNAL(clicked()),
                    this, SLOT(on_tbNextYear_clicked()));
    Q_ASSERT(check);
    check = connect(&m_tbPreMonth, SIGNAL(clicked()),
                    this, SLOT(on_tbPreviousMonth_clicked()));
    Q_ASSERT(check);
    check = connect(&m_tbNextMonth, SIGNAL(clicked()),
                    this, SLOT(on_tbNextMonth_clicked()));
    Q_ASSERT(check);
    check = connect(&m_pbToday, SIGNAL(clicked()),
                    this, SLOT(soltShowToday()));
    Q_ASSERT(check);
//    check = connect(&m_cmbYear, SIGNAL(currentIndexChanged(int)),
//                    this, SLOT(on_cbYear_currentIndexChanged(int)));
//    Q_ASSERT(check);
    check = connect(&m_cmbMonth, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_cbMonth_currentIndexChanged(int)));
    Q_ASSERT(check);
    check = connect(&m_View, SIGNAL(pressed(const QModelIndex&)),
                    this, SLOT(on_tvMonth_pressed(const QModelIndex&)));
    Q_ASSERT(check);

    updateGeometry();
}

CLunarCalendar::~CLunarCalendar()
{}

void CLunarCalendar::InitResource()
{
    if(g_pLunarCalendarPrivate) return;

    g_pLunarCalendarPrivate = new CLunarCalendarPrivate();

    Q_INIT_RESOURCE(ResourceLunarCalendar);
#if _DEBUG
    Q_INIT_RESOURCE(ResourceSql);
    Q_INIT_RESOURCE(translations_LunarCalendar);
#endif
}

void CLunarCalendar::CLeanResource()
{
    if(g_pLunarCalendarPrivate)
    {
        delete g_pLunarCalendarPrivate;
        g_pLunarCalendarPrivate = nullptr;
    }

    Q_CLEANUP_RESOURCE(ResourceLunarCalendar);    
#if _DEBUG
    Q_CLEANUP_RESOURCE(ResourceSql);
    Q_CLEANUP_RESOURCE(translations_LunarCalendar);
#endif
}

int CLunarCalendar::SetHeadposition(_HEAD_position pos)
{
    m_Headposition = pos;
    if(m_pMainLayout)
    {
        m_pMainLayout->removeWidget(&m_View);
        m_pMainLayout->removeItem(m_pHeadLayout);
        delete m_pMainLayout;
        m_pMainLayout = nullptr;
    }
    if(m_pHeadLayout)
    {
        m_pHeadLayout->removeItem(m_pToolLayout);
        m_pHeadLayout->removeWidget(&m_lbDate);
        m_pHeadLayout->removeWidget(&m_lbTime);
        delete m_pHeadLayout;
        m_pHeadLayout = nullptr;
    }

    m_pMainLayout = new QGridLayout(this);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_pMainLayout->setMargin(0);
#else
    m_pMainLayout->setContentsMargins(0, 0, 0, 0);
#endif
    m_pMainLayout->setSpacing(0);
    setLayout(m_pMainLayout);

    switch (pos) {
    case _HEAD_position::Not:
        ShowHead(false);
        m_pMainLayout->addWidget(&m_View);
        break;
    case _HEAD_position::Top:
        ShowHead(true);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_pHeadLayout->setMargin(0);
#else
        m_pHeadLayout->setContentsMargins(0, 0, 0, 0);
#endif
        m_pHeadLayout->setSpacing(0);

        m_pMainLayout->addLayout(m_pHeadLayout, 0, 0);
        m_pMainLayout->addWidget(&m_View);
        break;
    case _HEAD_position::Down:
        ShowHead(true);
        m_pMainLayout->addWidget(&m_View);
        
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->addLayout(m_pToolLayout);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_pHeadLayout->setMargin(0);
#else
        m_pHeadLayout->setContentsMargins(0, 0, 0, 0);
#endif
        m_pHeadLayout->setSpacing(0);
        
        m_pMainLayout->addLayout(m_pHeadLayout, 1, 0);
        break;
    case _HEAD_position::Left:
        ShowHead(true);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addStretch();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->addStretch();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_pHeadLayout->setMargin(0);
#else
        m_pHeadLayout->setContentsMargins(0, 0, 0, 0);
#endif
        m_pHeadLayout->setSpacing(0);
        
        m_pMainLayout->addLayout(m_pHeadLayout, 0, 0);
        m_pMainLayout->addWidget(&m_View, 0, 1);
        break;
    case _HEAD_position::Right:
        ShowHead(true);
        m_pMainLayout->addWidget(&m_View);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addStretch();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->addStretch();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_pHeadLayout->setMargin(0);
#else
        m_pHeadLayout->setContentsMargins(0, 0, 0, 0);
#endif
        m_pHeadLayout->setSpacing(0);
        
        m_pMainLayout->addLayout(m_pHeadLayout, 0, 1);
        break;
    }
    return 0;
}

int CLunarCalendar::ShowSelectTitle()
{
    QDate d;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;
    d = pModel->GetDate();
    if(d.isNull()) return -2;
    QString szDate;
    if(static_cast<int>(_CalendarType::CalendarTypeSolar)
        & static_cast<int>(GetCalendarType()))
        szDate = d.toString(locale().dateFormat(QLocale::LongFormat));
    if(static_cast<int>(_CalendarType::CalendarTypeLunar)
        & static_cast<int>(GetCalendarType()))
        szDate += " " + SelectedLunar();
    m_lbDate.setText(szDate);
    return 0;
}

void CLunarCalendar::on_cbYear_currentIndexChanged(int index)
{
    qDebug(Logger) << "CLunarCalendar::on_cbYear_currentIndexChanged";
    Q_UNUSED(index);
    UpdateViewModel();
    UpdateMonthMenu();
}

void CLunarCalendar::on_tbNextYear_clicked()
{
    int nIndex = m_cmbYear.currentIndex() + 1;
    if(m_cmbYear.count() <= nIndex)
        return;
    m_cmbYear.setCurrentIndex(nIndex);
}

void CLunarCalendar::on_tbPreviousYear_clicked()
{
    int nIndex = m_cmbYear.currentIndex() - 1;
    if(0 > nIndex)
        return;
    m_cmbYear.setCurrentIndex(nIndex);
}

void CLunarCalendar::on_tbNextMonth_clicked()
{
    if(!m_tbNextMonth.isEnabled())
        return;
    int i = m_cmbMonth.currentIndex() + 1;
    if(i > m_cmbMonth.count() - 1)
    {
        i = 0;
        m_cmbYear.setCurrentIndex(
                    m_cmbYear.findData(m_cmbYear.currentData().toInt() + 1));
    }
    m_cmbMonth.setCurrentIndex(i);
}

void CLunarCalendar::on_tbPreviousMonth_clicked()
{
    if(!m_tbPreMonth.isEnabled())
        return;
    int i = m_cmbMonth.currentIndex() - 1;
    if(i < 0)
    {
        m_cmbYear.setCurrentIndex(
                    m_cmbYear.findData(m_cmbYear.currentData().toInt() - 1));
        i = m_cmbMonth.count() - 1;
    }
    m_cmbMonth.setCurrentIndex(i);
}

void CLunarCalendar::on_cbMonth_currentIndexChanged(int index)
{
    qDebug(Logger) << "CLunarCalendar::on_cbMonth_currentIndexChanged:" << index;
    Q_UNUSED(index);
    UpdateViewModel();
    EnableMonthMenu();
}

void CLunarCalendar::soltShowToday()
{
//    m_cmbYear.setCurrentIndex(m_cmbYear.findData(QDate::currentDate().year()));
//    int nIndex = m_cmbMonth.findData(QDate::currentDate().month());
//    if(nIndex > -1)
//        m_cmbMonth.setCurrentIndex(nIndex);
    SetSelectedDate(QDate::currentDate());
}

/*!
 * \brief CLunarCalendar::UpdateViewModel
 * \param bForce true：强制更新数据。
 *               false:当不是当前选择年月，才更新数据
 * \return 
 */
int CLunarCalendar::UpdateViewModel(bool bForce)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -2;
    
    if (pModel->GetShowYear() != m_cmbYear.currentData().toInt()
        || pModel->GetShowMonth() != m_cmbMonth.currentData().toInt()) {
        //m_View.selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
        m_View.selectionModel()->clear();
    }
    switch (GetViewType())
    {
    case _VIEW_TYPE::ViewTypeMonth:
    {
        pModel->showMonth(m_cmbYear.currentData().toInt(),
                          m_cmbMonth.currentData().toInt(),
                          bForce);
        if(pModel && m_bShowBackgroupImage)
        {
            QString szBackgrpup = ":/image/" + QString::number(pModel->GetShowMonth());
            if(static_cast<int>(_CalendarType::CalendarTypeLunar)
                & static_cast<int>(GetCalendarType()))
                szBackgrpup += "_zh_CN";
            SetBackgroup(szBackgrpup);    
        }
        
        break;
    }
    case _VIEW_TYPE::ViewTypeWeek:
        pModel->showWeek(m_cmbYear.currentData().toInt(),
                         m_cmbMonth.currentData().toInt(),
                         bForce);
        break;
    }
    
    ShowSelectTitle();
    
    updateGeometry();
    return 0;
}

void CLunarCalendar::ShowGrid(bool show)
{
    m_View.setShowGrid(show);
}
/*
bool CLunarCalendar::ShowGrid() const
{
    return m_View.showGrid();
}//*/

void CLunarCalendar::ShowBackgroupImage(bool show)
{
    m_bShowBackgroupImage = show;
    if(m_bShowBackgroupImage)
        UpdateViewModel();
    else
        m_View.setStyleSheet("border-image:none");
        
    return;
}
/*
bool CLunarCalendar::ShowBackgroupImage()
{
    return m_bShowBackgroupImage;
}//*/

void CLunarCalendar::EnableToolTip(bool enable)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    return pModel->EnableToolTip(enable);
}

void CLunarCalendar::ShowToday(bool bShow)
{
    if(m_bShowToday == bShow)
        return;
    m_bShowToday = bShow;
    m_pbToday.setVisible(m_bShowToday);
}

void CLunarCalendar::ShowWeeks(bool bShow)
{
    m_View.verticalHeader()->setVisible(bShow);
    updateGeometry();
}

void CLunarCalendar::ShowWeekHead(bool bShow)
{
    m_View.horizontalHeader()->setVisible(bShow);
    updateGeometry();
}

void CLunarCalendar::ShowHead(bool bShow)
{
    ShowTools(bShow);
    ShowDate(bShow);
    ShowTime(bShow);
}

void CLunarCalendar::ShowTools(bool bShow)
{
    m_tbPreYear.setVisible(bShow);
    m_tbNextYear.setVisible(bShow);
    m_cmbYear.setVisible(bShow);
    m_tbNextMonth.setVisible(bShow);
    m_tbPreMonth.setVisible(bShow);
    m_cmbMonth.setVisible(bShow);
    ShowToday(bShow);
    updateGeometry();
}

void CLunarCalendar::ShowDate(bool bShow)
{
    m_lbDate.setVisible(bShow);
    updateGeometry();
}

QDate CLunarCalendar::SelectedDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QDate();
    return pModel->GetDate();
}

void CLunarCalendar::SetSelectedDate(const QDate &date, bool bForce)
{
    if (!date.isValid())
        return;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    if(bForce || pModel->GetDate() != date)
    {
        pModel->setDate(date);
    }
    
    QDate newDate = pModel->GetDate();
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(newDate.year()));
    switch (GetViewType()) {
    case _VIEW_TYPE::ViewTypeMonth:
        m_cmbMonth.setCurrentIndex(m_cmbMonth.findData(newDate.month()));
        break;
    case _VIEW_TYPE::ViewTypeWeek:
    {
        int week = 1;
        int year = 0;
        week = newDate.weekNumber(&year);
        if(year != newDate.year())
        {
            m_cmbYear.setCurrentIndex(m_cmbYear.findData(newDate.year()));
        }
        m_cmbMonth.setCurrentIndex(m_cmbMonth.findData(week));
    }
        break;
    }
    
    UpdateViewModel();

    int row, col;
    pModel->cellForDate(pModel->GetDate(), &row, &col);
    if(row >= 0 && col >= 0)
    {
        CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
        if(pModel && m_bShowBackgroupImage && GetViewType() == _VIEW_TYPE::ViewTypeMonth)
        {
            QString szJiQi = pModel->data(pModel->index(row, col),
                                CLunarCalendarModel::ROLE::BackgroupImage).toString();
            if(!szJiQi.isEmpty())
                SetBackgroup(szJiQi);
        }
        
        m_View.selectionModel()->clear();
        m_View.selectionModel()->setCurrentIndex(pModel->index(row, col),
                                           QItemSelectionModel::SelectCurrent);
        //m_View.setCurrentIndex(pModel->index(row, col));
        //m_View.selectionModel()->select(pModel->index(row, col),
        //                                    QItemSelectionModel::SelectCurrent);
        m_View.setFocus();
        m_oldCol = col;
        m_oldRow = row;
    }
    emit sigSelectionChanged();
}

QString CLunarCalendar::SelectedLunar() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QString();
    QDate date = pModel->GetDate();
    CCalendarLunar l(date);
    return l.GetLunar();
}

QString CLunarCalendar::SelectedSolarTerm() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QString();
    QDate date = pModel->GetDate();
    CCalendarLunar l(date);
    return l.GetJieQi();
}

int CLunarCalendar::SelectedLunar(int &year, int &month, int &day)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    QDate date = pModel->GetDate();
    CCalendarLunar l(date);
    year = l.GetYear();
    month = l.GetMonth();
    day = l.GetDay();
    return 0;
}

int CLunarCalendar::GetLunar(const QDate date, int &year, int &month, int &day)
{
    CCalendarLunar l(date);
    year = l.GetYear();
    month = l.GetMonth();
    day = l.GetDay();
    return 0;
}

int CLunarCalendar::GetShowYear() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->GetShowYear();
}

int CLunarCalendar::GetShowMonth() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->GetShowMonth();
}

QDate CLunarCalendar::MaximumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QDate();
    return pModel->GetMaximumDate();
}

void CLunarCalendar::SetMaximumDate(const QDate &date)
{
    if(!date.isValid())
        return;
    
    if(QDate::currentDate() > date)
        m_pbToday.setVisible(false);
    else
        ShowToday(m_bShowToday);
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    QDate oldDate = pModel->GetDate();
    pModel->SetMaximumDate(date);
    int year = date.year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
    {
        date.weekNumber(&year);
    }
    
    SetYearRange(date.year(), pModel->GetMaximumDate().year());
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetShowYear()));
    
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        UpdateViewModel();
        emit sigSelectionChanged();
    }
}

QDate CLunarCalendar::MinimumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QDate();
    return pModel->GetMinimumDate();
}

void CLunarCalendar::SetMinimumDate(const QDate &date)
{
    if(!date.isValid())
        return;
    
    if(QDate::currentDate() < date)
        m_pbToday.setVisible(false);
    else
        ShowToday(m_bShowToday);
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    QDate oldDate = pModel->GetDate();
    pModel->SetMinimumDate(date);
    int year = date.year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
    {
        date.weekNumber(&year);
    }
    
    SetYearRange(date.year(), pModel->GetMaximumDate().year());
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetShowYear()));
    
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        UpdateViewModel();
        emit sigSelectionChanged();
    }
}

void CLunarCalendar::SetDateRange(const QDate &min, const QDate &max)
{
    if (!min.isValid() || !max.isValid())
        return;
    
    if(min > max)
    {
        qCritical() << "SetDateRange parameter error: min =" << min << " max =" << max;
        return;
    }
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    if(pModel->GetMaximumDate() == max && pModel->GetMinimumDate() == min)
        return;
    
    if(QDate::currentDate() < min || QDate::currentDate() > max)
        m_pbToday.setVisible(false);
    else
        ShowToday(m_bShowToday);
    
    QDate oldDate = pModel->GetDate();
    pModel->setRange(min, max);

    int yearMin = min.year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
    {
        min.weekNumber(&yearMin);
    }
    int yearMax = max.year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
        max.weekNumber(&yearMax);
    
    SetYearRange(min.year(), max.year());
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetShowYear()));
    
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        UpdateViewModel();
        emit sigSelectionChanged();
    }
}

int CLunarCalendar::SetYearRange(int min, int max)
{
    if(min > max)
        return -1;
    
    bool check = m_cmbYear.disconnect();

    m_cmbYear.clear();
    for(int i = min; i < max; i++)
    {
        if(0 == i)
            continue;
        m_cmbYear.addItem(QString::number(i), i);
    }
    check = connect(&m_cmbYear, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_cbYear_currentIndexChanged(int)));
    Q_ASSERT(check);
    return 0;
}

int CLunarCalendar::EnableMonthMenu()
{
    bool prevEnabled = true;
    bool nextEnabled = true;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;

    int minYear = pModel->GetMinimumDate().year();
    int maxYear = pModel->GetMaximumDate().year();
    if (GetViewType() == _VIEW_TYPE::ViewTypeWeek) {
        pModel->GetMinimumDate().weekNumber(&minYear);
        pModel->GetMaximumDate().weekNumber(&maxYear);
    }

    if (pModel->GetShowYear() == minYear) {
        switch (GetViewType())
        {
        case _VIEW_TYPE::ViewTypeMonth:
            if (pModel->GetShowMonth() == pModel->GetMinimumDate().month())
                prevEnabled = false;
            break;
        case _VIEW_TYPE::ViewTypeWeek:
            if (pModel->GetShowWeek() == pModel->GetMinimumDate().weekNumber())
                prevEnabled = false;
            break;
        }
    }

    if (pModel->GetShowYear() == maxYear) {
        switch (GetViewType()) {
        case _VIEW_TYPE::ViewTypeMonth:
            if (pModel->GetShowMonth() == pModel->GetMaximumDate().month())
                nextEnabled = false;
            break;
        case _VIEW_TYPE::ViewTypeWeek:
            if(pModel->GetShowWeek() == pModel->GetMaximumDate().weekNumber())
                nextEnabled = false;
            break;
        }
    }

    m_tbPreMonth.setEnabled(prevEnabled);
    m_tbNextMonth.setEnabled(nextEnabled);
    return 0;
}

int CLunarCalendar::UpdateMonthMenu()
{
    int beg = 1, end = 12;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    
    EnableMonthMenu();

    int minYear = pModel->GetMinimumDate().year();
    int maxYear = pModel->GetMaximumDate().year();
    
    if (GetViewType() == _VIEW_TYPE::ViewTypeWeek) {
        end = pModel->GetWeeksOfYear(m_cmbYear.currentData().toInt());
        pModel->GetMinimumDate().weekNumber(&minYear);
        pModel->GetMaximumDate().weekNumber(&maxYear);
    }
    
    if (pModel->GetShowYear() == minYear) {
        switch (GetViewType())
        {
        case _VIEW_TYPE::ViewTypeMonth:
            beg = pModel->GetMinimumDate().month();
            break;
        case _VIEW_TYPE::ViewTypeWeek:
            beg = pModel->GetMinimumDate().weekNumber();
            break;
        }
    }
    
    if (pModel->GetShowYear() == maxYear) {
        switch (GetViewType()) {
        case _VIEW_TYPE::ViewTypeMonth:
            end = pModel->GetMaximumDate().month();
            break;
        case _VIEW_TYPE::ViewTypeWeek:
            end = pModel->GetMaximumDate().weekNumber();
            break;
        }
    }
    
    int index = m_cmbMonth.currentIndex();
    bool check = m_cmbMonth.disconnect();
    Q_ASSERT(check);
    m_cmbMonth.clear();
    for (int i = beg; i <= end; i++) {
        switch (GetViewType()) {
        case _VIEW_TYPE::ViewTypeMonth:
            m_cmbMonth.addItem(locale().monthName(i), i);
            break;
        case _VIEW_TYPE::ViewTypeWeek:
            m_cmbMonth.addItem(QString::number(i), i);
            break;
        }
    }
    check = connect(&m_cmbMonth, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_cbMonth_currentIndexChanged(int)));
    Q_ASSERT(check);
    if(index >= m_cmbMonth.count())
        index = m_cmbMonth.count() - 1;
    if(index < 0)
        index = 0;
    m_cmbMonth.setCurrentIndex(index);

    return 0;
}

Qt::DayOfWeek CLunarCalendar::FirstDayOfWeek() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return locale().firstDayOfWeek();
    return pModel->firstDayOfWeek();
}

//void CLunarCalendar::SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
//{
//    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
//    if(!pModel) return;
//    pModel->setFirstDayOfWeek(dayOfWeek);
//}

void CLunarCalendar::on_tvMonth_pressed(const QModelIndex &index)
{
    qDebug(Logger) << "CLunarCalendar::on_tvMonth_pressed" << index;
    if(!index.isValid())
        return;
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    QDate d = pModel->dateForCell(index.row(), index.column());
    if(d.isValid())
        SetSelectedDate(d);

    m_oldCol = index.column();
    m_oldRow = index.row();
}

bool CLunarCalendar::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug(Logger) << event->type();
    switch(event->type()){
//    case QEvent::Gesture:
//        {
//            qDebug(Logger) << "QEvent::Gesture";
//            QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);
//            if (QGesture *swipe = gestureEvent->gesture(Qt::SwipeGesture))
//                if (swipe->state() == Qt::GestureFinished) {
//                    QSwipeGesture *gesture = static_cast<QSwipeGesture *>(swipe);
//                    if (gesture->horizontalDirection() == QSwipeGesture::Left
//                        || gesture->verticalDirection() == QSwipeGesture::Up) {
//                        qDebug(Logger) << "swipeTriggered(): swipe to previous";
//                    } else {
//                        qDebug(Logger) << "swipeTriggered(): swipe to next";
//                    }
//                }
//        }
//        break;
    case QEvent::TouchBegin:
    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//        qDebug(Logger) << "touch begin points:" << touchPoints.length() << touchPoints;
        event->accept();
        return true;
    }
    case QEvent::TouchUpdate:
    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//        qDebug(Logger) << "touch update points:" << touchPoints.length() << touchPoints;
        //event->accept();
        break;
    }
    case QEvent::TouchEnd:
        {
            event->accept();
            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
#else
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
#endif
            //qDebug(Logger) << "touch end points:" << touchPoints.length() << touchPoints;
            if(touchPoints.length() == 1)
            {
                QTouchEvent::TouchPoint t = touchPoints.first();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QLineF line(QLineF(t.pressPosition(), t.lastPosition()));
#else
                QLineF line(QLineF(t.startPos(), t.lastPos()));
#endif
                if(qAbs(line.dx()) > qAbs(line.dy()))
                {
                    //qDebug(Logger) << "dx:" << line.dx() << "hor:" << m_View.horizontalHeader()->minimumSectionSize();
                    if(qAbs(line.dx()) > m_View.horizontalHeader()->minimumSectionSize())
                    {
                        if(line.dx() > 0)
                            on_tbPreviousMonth_clicked();
                        else
                            on_tbNextMonth_clicked();
                    }
                }else {
                    //qDebug(Logger) << "dy:" << line.dy();
                    if(qAbs(line.dy()) > m_View.verticalHeader()->minimumSectionSize())
                    {
                        if(line.dy() > 0)
                        {
                            if(GetViewType() != _VIEW_TYPE::ViewTypeMonth
                                    && m_TouchFunction == _TOUCH_UP_DOWN_FUNCTION::TouchChangeView)
                                SetViewType(_VIEW_TYPE::ViewTypeMonth);
                            else
                                on_tbPreviousMonth_clicked();
                        }
                        else
                        {
                            if(GetViewType() != _VIEW_TYPE::ViewTypeWeek
                                    && m_TouchFunction == _TOUCH_UP_DOWN_FUNCTION::TouchChangeView)
                                SetViewType(_VIEW_TYPE::ViewTypeWeek);
                            else
                                on_tbNextMonth_clicked();
                        }
                    } 
                }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                on_tvMonth_pressed(m_View.indexAt(t.lastPosition().toPoint()));
#else
                on_tvMonth_pressed(m_View.indexAt(t.lastPos().toPoint()));
#endif
            }

            break;
        }
#ifndef QT_NO_WHEELEVENT
    case QEvent::Wheel:
        {
            QWheelEvent *we = dynamic_cast<QWheelEvent*>(event);
            const int numDegrees = we->angleDelta().y() / 8;
            const int numSteps = numDegrees / 15;
            //qDebug(Logger) << "step:" << numSteps;
            CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
            if(pModel)
            {
                const QModelIndex index = m_View.currentIndex();
                if(index.isValid())
                {
                    QDate currentDate = pModel->dateForCell(index.row(), index.column());
                    currentDate = currentDate.addDays(-numSteps * 7);
                    this->SetSelectedDate(currentDate);
                }
            }
            break;
        }
#endif
    case QEvent::KeyRelease:
    {
        CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
        if(!pModel)
            break;
        QKeyEvent* key = dynamic_cast<QKeyEvent*>(event);
        qDebug(Logger) << "CLunarCalendar::eventFilter key:" << key
                 << "old:" << m_oldRow << m_oldCol
                 << "current:" << m_View.currentIndex().row()
                 << m_View.currentIndex().column();
        switch (key->key()) {
        case Qt::Key_Up:
            {
                QModelIndex index = m_View.currentIndex();
                if(index.isValid())
                {
                    QDate d = pModel->dateForCell(index.row(), index.column());
                    if(index.row() == 0 && m_oldRow == 0)
                    {
                        if(GetViewType() == _VIEW_TYPE::ViewTypeMonth
                                && d.month() == m_cmbMonth.currentData())
                            d = d.addDays(-7);
                        if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
                            d = d.addDays(-7);
                    }
                    SetSelectedDate(d);
                }                
            }
            break;
        case Qt::Key_Down:
            {
                QModelIndex index = m_View.currentIndex();
                if(index.isValid())
                {
                    QDate d = pModel->dateForCell(index.row(), index.column());
                    if(index.row() >= pModel->rowCount() -1
                            && m_oldRow == index.row())
                    {
                        if(GetViewType() == _VIEW_TYPE::ViewTypeMonth
                                && d.month() == m_cmbMonth.currentData())
                            d = d.addDays(7);
                        
                        if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
                            d = d.addDays(7);
                    }
                    SetSelectedDate(d);
                }                
            }
            break;
        case Qt::Key_Left:
            {
                QModelIndex index = m_View.currentIndex();
                if(index.isValid())
                {
                    QDate d = pModel->dateForCell(index.row(), index.column());
                    if(index.column() == m_oldCol)
                        d = d.addDays(-1);
                    SetSelectedDate(d);
                }
            }
            break;
        case Qt::Key_Right:
            {
                QModelIndex index = m_View.currentIndex();
                if(index.isValid())
                {
                    QDate d = pModel->dateForCell(index.row(), index.column());
                    if(index.column() == m_oldCol)
                        d = d.addDays(1);
                    SetSelectedDate(d);
                }
            }
            break;
        default:
            break;
        };
        break;
    }
    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}

bool CLunarCalendar::EnableHolidays(bool bEnable)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return true;
    return pModel->EnableHolidays(bEnable);
}

bool CLunarCalendar::EnableSolarTerm(bool bEnable)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return true;
    return pModel->EnableSolarTerm(bEnable);
}

int CLunarCalendar::SetTaskHandle(QSharedPointer<CTaskHandler> handler)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->SetTaskHandle(handler);
}

#if HAS_CPP_11
int CLunarCalendar::SetTaskHandle(
    std::function<uint (/*in*/const QDate&,
                       /*out*/QStringList &)> cbHandler)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->SetTaskHandle(cbHandler);
}
#endif

int CLunarCalendar::SetViewType(_VIEW_TYPE type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetViewType(type);
    
    int yearMin = pModel->GetMinimumDate().year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
    {
        pModel->GetMinimumDate().weekNumber(&yearMin);
        if(m_bShowBackgroupImage)
            m_View.setStyleSheet("border-image:none");
    }
    int yearMax = pModel->GetMaximumDate().year();
    if(GetViewType() == _VIEW_TYPE::ViewTypeWeek)
        pModel->GetMaximumDate().weekNumber(&yearMax);
    
    SetYearRange(yearMin, yearMax);
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetShowYear()));
    
    SetSelectedDate(QDate::currentDate(), true);

    switch (GetViewType()) {
    case _VIEW_TYPE::ViewTypeWeek:
        m_cmbMonth.setToolTip(tr("Week"));
        m_cmbMonth.setStatusTip(tr("Week"));
        break;
    case _VIEW_TYPE::ViewTypeMonth:
        m_cmbMonth.setToolTip(tr("Month"));
        m_cmbMonth.setStatusTip(tr("Month"));
        break;
    }

    updateGeometry();
    return nRet;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendar::GetViewType() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return _VIEW_TYPE::ViewTypeMonth;
    return pModel->GetViewType();
}

int CLunarCalendar::SetTouchUpDownFunction(_TOUCH_UP_DOWN_FUNCTION f)
{
    m_TouchFunction = f;
    return 0;
}

CLunarCalendar::_CalendarType CLunarCalendar::GetCalendarType() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return static_cast<_CalendarType>(
            static_cast<int>(_CalendarType::CalendarTypeLunar)
            | static_cast<int>(_CalendarType::CalendarTypeSolar));
    return pModel->GetCalendarType();
}

int CLunarCalendar::SetCalendarType(_CalendarType type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetCalendarType(type);
    UpdateViewModel();
    
    return nRet;
}

int CLunarCalendar::LoadCalendarTable(const QString &szFile)
{
    return CLunarTable::Instance()->Load(szFile);
}

int CLunarCalendar::GenerateCalendarTable(const QString &szFile,
                                          int nThreadNumber, bool bClearCache,
                                          bool bSaveAllDate)
{
    return  CLunarTable::Instance()->Generate(
                this->MinimumDate(),
                MaximumDate(),
                szFile,
                nThreadNumber,
                bClearCache,
                bSaveAllDate);
}

void CLunarCalendar::slotTimeout()
{
    m_lbTime.setText(QTime::currentTime().toString(locale().timeFormat())); // + " " + QString::number(QTime::currentTime().msec()));
}

void CLunarCalendar::ShowTime(bool bShow)
{
    m_lbTime.setVisible(bShow);
    if(bShow)
        m_Timer.start(1000);
    else
        m_Timer.stop();
    updateGeometry();
}

int CLunarCalendar::SetBackgroup(const QString &szFile)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;

    if(m_bShowBackgroupImage)
        m_View.setStyleSheet("border-image:url(" + szFile + ")");
    else
        m_View.setStyleSheet("border-image:none");
    return 0;
}

int CLunarCalendar::Update()
{
    int nRet = UpdateViewModel(true);
    return nRet;
}

//*
void CLunarCalendar::setFont(const QFont& font)
{
    QWidget::setFont(font);
    m_View.setFont(font);
    m_View.horizontalHeader()->setFont(font);
    m_View.verticalHeader()->setFont(font);
}//*/

QSize CLunarCalendar::sizeHint() const
{
    return minimumSizeHint();
}

QSize CLunarCalendar::minimumSizeHint() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return QSize();
    
    ensurePolished();
    int w = 0;
    int h = 0;

    int rows = pModel->rowCount();
    int cols = pModel->columnCount();

    int marginW = m_View.horizontalHeader()->style()->pixelMetric(
                                QStyle::PM_FocusFrameHMargin) << 1;
    int marginH = m_View.verticalHeader()->style()->pixelMetric(
                                QStyle::PM_FocusFrameVMargin) << 1;
    QMargins cm = m_View.horizontalHeader()->contentsMargins();
    w = (m_View.horizontalHeader()->minimumSectionSize() + marginW) * cols
            + marginW + cm.left() + cm.right();
    if(m_View.verticalHeader()->isVisible())
    {
//        qDebug(Logger) << "m_View.verticalHeader()->isVisible()";
        w += m_View.verticalHeader()->sizeHint().width();
    }
    cm = m_View.verticalHeader()->contentsMargins();
    if(_VIEW_TYPE::ViewTypeMonth == GetViewType())
        h = (m_View.verticalHeader()->minimumSectionSize() + marginH) * rows;
    else
        h = (m_View.verticalHeader()->minimumSectionSize() + marginH);
    h = h + marginH + cm.top() + cm.bottom();
    if(m_View.horizontalHeader()->isVisible())
    {
//        qDebug(Logger) << "m_View.horizontalHeader()->isVisible()";
        h += m_View.horizontalHeader()->sizeHint().height();
    }
    
//    qDebug(Logger) << "w:" << w << "h:" << h << "marginW:" << marginW
//             << "m_View.horizontalHeader()->minimumSectionSize():"
//             << m_View.horizontalHeader()->minimumSectionSize() 
//             << "m_View.verticalHeader()->minimumSectionSize():"
//             << m_View.verticalHeader()->minimumSectionSize() 
//             << "marginH:" << marginH << cm
//             << "ver height:" << m_View.verticalHeader()->sizeHint()
//             << "hor width:" << m_View.horizontalHeader()->sizeHint();

    //add the size of the header.
    int headerH = 0;
    int headerW = 0;
    if(m_tbPreYear.isVisible())
    {
        QSize s = m_tbPreYear.sizeHint();
        headerH = s.height();
        headerW = s.width();
    }
    if(m_cmbYear.isVisible())
    {
        QSize s = m_cmbYear.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_tbNextYear.isVisible())
    {
        QSize s = m_tbNextYear.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_tbPreMonth.isVisible())
    {
        QSize s = m_tbPreMonth.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_cmbMonth.isVisible())
    {
        QSize s = m_cmbMonth.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_tbNextMonth.isVisible())
    {
        QSize s = m_tbNextMonth.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_pbToday.isVisible())
    {
        QSize s = m_pbToday.sizeHint();
        headerH = qMax(headerH, s.height());
        headerW += s.width();
    }
    if(m_lbDate.isVisible())
    {
        QSize s = m_lbDate.sizeHint();
        headerH += s.height();
        headerW = qMax(headerW, s.width());
    }
    if(m_lbTime.isVisible())
    {
        QSize s = m_lbTime.sizeHint();
        headerH += s.height();
        headerW = qMax(headerW, s.width());
    }

    switch (m_Headposition) {
    case _HEAD_position::Top:
    case _HEAD_position::Down:
        w = qMax(w, headerW);
        h += headerH;
        break;
    case _HEAD_position::Left:
    case _HEAD_position::Right:
        w += headerW;
        h = qMax(h, headerH);
        break;
    default:
        break;
    }
    
    cm = contentsMargins();
    w += cm.left() + cm.right();
    h += cm.top() + cm.bottom();
    
//    qDebug(Logger) << "w:" << w << "h:" << h;

    return QSize(w, h);
}
