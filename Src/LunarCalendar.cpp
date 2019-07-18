#include "LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
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

class CLunarCalendarPrivate
{
public:
    CLunarCalendarPrivate()
    {
        m_Translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
                        + "/LunarCalendar_" + QLocale::system().name() + ".qm");
        qApp->installTranslator(&m_Translator);
    }
    ~CLunarCalendarPrivate()
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
    m_HeadPostion(Top),
    m_oldRow(0),
    m_oldCol(0),
    m_bShowToday(true),
    m_bShowBackgroupImage(false),
    m_TouchFunction(TouchChangeMounth)
{
    //setLocale(QLocale("zh_CN"));
    
    m_tbPreYear.setArrowType(Qt::UpArrow);
    m_tbNextYear.setArrowType(Qt::DownArrow);
    m_tbPreMonth.setArrowType(Qt::UpArrow);
    m_tbNextMonth.setArrowType(Qt::DownArrow);
    
//    QLineEdit *yearEdit = new QLineEdit;
//    yearEdit->setAlignment(Qt::AlignCenter);
//    m_cmbYear.setLineEdit(yearEdit);
//    QLineEdit *monthEdit = new QLineEdit;
//    monthEdit->setReadOnly(true);
//    monthEdit->setAlignment(Qt::AlignCenter);
//    m_cmbMonth.setLineEdit(monthEdit);
    
    m_lbDate.setAlignment(Qt::AlignCenter);
    m_lbTime.setAlignment(Qt::AlignCenter);
    m_pbToday.setText(tr("Today"));
    
//    m_tbPreYear.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    m_cmbYear.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    m_tbNextYear.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    m_tbPreMonth.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    m_cmbMonth.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    m_tbNextMonth.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    m_pbToday.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_lbDate.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_lbTime.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    
    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);
    m_View.setModel(pModel);
    m_View.setItemDelegate(new CLunarCalendarDelegate(&m_View));
    m_View.horizontalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    m_View.verticalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    m_View.setAttribute(Qt::WA_AcceptTouchEvents);
    SetShowGrid(false);

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
    
    /*
    int nFontSize = 5;
    QFont font = m_View.font();
    font.setPointSize(nFontSize);
    m_View.setFont(font);
    
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
    
    m_cmbMonth.setCurrentIndex(m_cmbMonth.findData(pModel->GetDate().month()));
    SetSelectedDate(pModel->GetDate());

    SetYearRange(pModel->GetMinimumDate().year(),
                 pModel->GetMaximumDate().year());
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetDate().year()));
    m_cmbYear.setEditable(true);
    
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
    m_pToolLayout->setMargin(0);
    m_pToolLayout->setSpacing(0);

    SetHeadPostion();

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
                    this, SLOT(on_pbToday_clicked()));
    Q_ASSERT(check);
//    check = connect(&m_cmbYear, SIGNAL(currentIndexChanged(int)),
//                    this, SLOT(on_cbYear_currentIndex(int)));
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
{
}

void CLunarCalendar::InitResource()
{
    if(nullptr == g_pLunarCalendarPrivate)
        g_pLunarCalendarPrivate = new CLunarCalendarPrivate();

    Q_INIT_RESOURCE(ResourceLunarCalendar);
#if _DEBUG
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
    Q_CLEANUP_RESOURCE(translations_LunarCalendar);
#endif
}

int CLunarCalendar::SetHeadPostion(_HEAD_POSTION pos)
{
    m_HeadPostion = pos;
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
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    setLayout(m_pMainLayout);
    
    switch (pos) {
    case Not:
        ShowHead(false);
        m_pMainLayout->addWidget(&m_View);
        break;
    case Top:
        ShowHead(true);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->setMargin(0);
        m_pHeadLayout->setSpacing(0);

        m_pMainLayout->addLayout(m_pHeadLayout, 0, 0);
        m_pMainLayout->addWidget(&m_View);
        break;
    case Down:
        ShowHead(true);
        m_pMainLayout->addWidget(&m_View);
        
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->setMargin(0);
        m_pHeadLayout->setSpacing(0);
        
        m_pMainLayout->addLayout(m_pHeadLayout, 1, 0);
        break;
    case Left:
        ShowHead(true);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addStretch();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->addStretch();
        m_pHeadLayout->setMargin(0);
        m_pHeadLayout->setSpacing(0);
        
        m_pMainLayout->addLayout(m_pHeadLayout, 0, 0);
        m_pMainLayout->addWidget(&m_View, 0, 1);
        break;
    case Right:
        ShowHead(true);
        m_pMainLayout->addWidget(&m_View);
        m_pHeadLayout = new QVBoxLayout();
        m_pHeadLayout->addStretch();
        m_pHeadLayout->addLayout(m_pToolLayout);
        m_pHeadLayout->addWidget(&m_lbDate);
        m_pHeadLayout->addWidget(&m_lbTime);
        m_pHeadLayout->addStretch();
        m_pHeadLayout->setMargin(0);
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
    if(CalendarTypeSolar & GetCalendarType())
        szDate = d.toString(locale().dateFormat(QLocale::LongFormat));
    if(CalendarTypeLunar & GetCalendarType())
        szDate += " " + SelectedLunar();
    m_lbDate.setText(szDate);
    return 0;
}

void CLunarCalendar::on_cbYear_currentIndex(int index)
{
    qDebug() << "CLunarCalendar::on_cbYear_currentIndex";
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
    qDebug() << "CLunarCalendar::on_cbMonth_currentIndexChanged:" << index;
    Q_UNUSED(index);
    UpdateViewModel();
    EnableMonthMenu();
}

void CLunarCalendar::on_pbToday_clicked()
{
//    m_cmbYear.setCurrentIndex(m_cmbYear.findData(QDate::currentDate().year()));
//    int nIndex = m_cmbMonth.findData(QDate::currentDate().month());
//    if(nIndex > -1)
//        m_cmbMonth.setCurrentIndex(nIndex);
    SetSelectedDate(QDate::currentDate());
}

int CLunarCalendar::UpdateViewModel(bool bForce)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -2;
    //m_View.selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
    m_View.selectionModel()->clear();
    switch (GetViewType())
    {
    case ViewTypeMonth:
    {
        pModel->showMonth(m_cmbYear.currentData().toInt(),
                          m_cmbMonth.currentData().toInt(),
                          bForce);
        if(pModel && m_bShowBackgroupImage)
        {
            QString szBackgrpup = ":/image/" + QString::number(pModel->GetShowMonth());
            if(CalendarTypeLunar & GetCalendarType())
                szBackgrpup += "_zh_CN";
            SetBackgroup(szBackgrpup);    
        }
        
        break;
    }
    case ViewTypeWeek:
        pModel->showWeek(m_cmbYear.currentData().toInt(),
                         m_cmbMonth.currentData().toInt(),
                         bForce);
        break;
    }
    
    ShowSelectTitle();
    
    updateGeometry();
    return 0;
}

void CLunarCalendar::SetShowGrid(bool show)
{
    m_View.setShowGrid(show);
}

bool CLunarCalendar::ShowGrid()
{
    return m_View.showGrid();
}

void CLunarCalendar::SetShowBackgroupImage(bool show)
{
    m_bShowBackgroupImage = show;
    if(m_bShowBackgroupImage)
        UpdateViewModel();
    else
        m_View.setStyleSheet("border-image:none");
        
    return;
}

bool CLunarCalendar::ShowBackgroupImage()
{
    return m_bShowBackgroupImage;
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
}

void CLunarCalendar::ShowWeekHead(bool bShow)
{
    m_View.horizontalHeader()->setVisible(bShow);
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
}

void CLunarCalendar::ShowDate(bool bShow)
{
    m_lbDate.setVisible(bShow);
}

/*!
    \property QCalendarWidget::selectedDate
    \brief the currently selected date.

    The selected date must be within the date range specified by the
    minimumDate and maximumDate properties. By default, the selected
    date is the current date.

    \sa setDateRange()
*/
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
    case ViewTypeMonth:
        m_cmbMonth.setCurrentIndex(m_cmbMonth.findData(newDate.month()));
        break;
    case ViewTypeWeek:
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
        if(pModel && m_bShowBackgroupImage && GetViewType() == ViewTypeMonth)
        {
            QString szJiQi = pModel->data(pModel->index(row, col),
                                CLunarCalendarModel::BackgroupImage).toString();
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

QString CLunarCalendar::SelectedLunar()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return QString();
    QDate date = pModel->GetDate();
    CCalendarLunar l(date);
    return l.GetLunar();
}

int CLunarCalendar::GetLunar(const QDate date, int &year, int &month, int &day)
{
    CCalendarLunar l(date);
    year = l.GetYear();
    month = l.GetMonth();
    day = l.GetDay();
    return 0;
}

int CLunarCalendar::YearShown() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->GetShowYear();
}

int CLunarCalendar::MonthShown() const
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
    if(GetViewType() == ViewTypeWeek)
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
    if(GetViewType() == ViewTypeWeek)
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
        qCritical() << "SetDateRange parmter error: min =" << min << " max =" << max;
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
    if(GetViewType() == ViewTypeWeek)
    {
        min.weekNumber(&yearMin);
    }
    int yearMax = max.year();
    if(GetViewType() == ViewTypeWeek)
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
                    this, SLOT(on_cbYear_currentIndex(int)));
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
    if (GetViewType() == ViewTypeWeek) {
        pModel->GetMinimumDate().weekNumber(&minYear);
        pModel->GetMaximumDate().weekNumber(&maxYear);
    }

    if (pModel->GetShowYear() == minYear) {
        switch (GetViewType())
        {
        case ViewTypeMonth:
            if (pModel->GetShowMonth() == pModel->GetMinimumDate().month())
                prevEnabled = false;
            break;
        case ViewTypeWeek:
            if (pModel->GetShowWeek() == pModel->GetMinimumDate().weekNumber())
                prevEnabled = false;
            break;
        }
    }

    if (pModel->GetShowYear() == maxYear) {
        switch (GetViewType()) {
        case ViewTypeMonth:
            if (pModel->GetShowMonth() == pModel->GetMaximumDate().month())
                nextEnabled = false;
            break;
        case ViewTypeWeek:
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
    
    if (GetViewType() == ViewTypeWeek) {
        end = pModel->GetWeeksOfYear(m_cmbYear.currentData().toInt());
        pModel->GetMinimumDate().weekNumber(&minYear);
        pModel->GetMaximumDate().weekNumber(&maxYear);
    }
    
    if (pModel->GetShowYear() == minYear) {
        switch (GetViewType())
        {
        case ViewTypeMonth:
            beg = pModel->GetMinimumDate().month();
            break;
        case ViewTypeWeek:
            beg = pModel->GetMinimumDate().weekNumber();
            break;
        }
    }
    
    if (pModel->GetShowYear() == maxYear) {
        switch (GetViewType()) {
        case ViewTypeMonth:
            end = pModel->GetMaximumDate().month();
            break;
        case ViewTypeWeek:
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
        case ViewTypeMonth:
            m_cmbMonth.addItem(locale().monthName(i), i);
            break;
        case ViewTypeWeek:
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
    //qDebug() << "CLunarCalendar::on_tvMonth_pressed" << index;
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
    //qDebug() << event->type();
    switch(event->type()){
    case QEvent::Gesture:
        {
            qDebug() << "QEvent::Gesture";
            QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);
            if (QGesture *swipe = gestureEvent->gesture(Qt::SwipeGesture))
                if (swipe->state() == Qt::GestureFinished) {
                    QSwipeGesture *gesture = static_cast<QSwipeGesture *>(swipe);
                    if (gesture->horizontalDirection() == QSwipeGesture::Left
                        || gesture->verticalDirection() == QSwipeGesture::Up) {
                        qDebug() << "swipeTriggered(): swipe to previous";
                    } else {
                        qDebug() << "swipeTriggered(): swipe to next";
                    }
                }
        }
        break;
    case QEvent::TouchBegin:
    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//        qDebug() << "touch begin points:" << touchPoints.length() << touchPoints;
        event->accept();
        return true;
    }
    case QEvent::TouchUpdate:
    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//        qDebug() << "touch update points:" << touchPoints.length() << touchPoints;
        //event->accept();
        break;
    }
    case QEvent::TouchEnd:
        {
            event->accept();
            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
            //qDebug() << "touch end points:" << touchPoints.length() << touchPoints;
            if(touchPoints.length() == 1)
            {
                QTouchEvent::TouchPoint t = touchPoints.first();
                QLineF line(QLineF(t.startPos(), t.lastPos()));
                if(qAbs(line.dx()) > qAbs(line.dy()))
                {
                    //qDebug() << "dx:" << line.dx() << "hor:" << m_View.horizontalHeader()->minimumSectionSize();
                    if(qAbs(line.dx()) > m_View.horizontalHeader()->minimumSectionSize())
                    {
                        if(line.dx() > 0)
                            on_tbPreviousMonth_clicked();
                        else
                            on_tbNextMonth_clicked();
                    }
                }else {
                    //qDebug() << "dy:" << line.dy();
                    if(qAbs(line.dy()) > m_View.verticalHeader()->minimumSectionSize())
                    {
                        if(line.dy() > 0)
                        {
                            if(GetViewType() != ViewTypeMonth
                                    && m_TouchFunction == TouchChangeView)
                                SetViewType(ViewTypeMonth);
                            else
                                on_tbPreviousMonth_clicked();
                        }
                        else
                        {
                            if(GetViewType() != ViewTypeWeek
                                    && m_TouchFunction == TouchChangeView)
                                SetViewType(ViewTypeWeek);
                            else
                                on_tbNextMonth_clicked();
                        }
                    } 
                }

                on_tvMonth_pressed(m_View.indexAt(t.lastPos().toPoint()));
            }

            break;
        }
#ifndef QT_NO_WHEELEVENT
    case QEvent::Wheel:
        {
            QWheelEvent *we = dynamic_cast<QWheelEvent*>(event);
            const int numDegrees = we->delta() / 8;
            const int numSteps = numDegrees / 15;
            //qDebug() << "step:" << numSteps;
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
            QKeyEvent* ke = dynamic_cast<QKeyEvent*>(event);
            switch (ke->key()) {
            case Qt::Key_Up:
                if(m_View.currentIndex().row() == m_oldRow)
                {
                    on_tbPreviousMonth_clicked();
                }
                m_oldRow = m_View.currentIndex().row();
                UpdateSelect();
                break;
            case Qt::Key_Down:
                if(m_View.currentIndex().row() >= m_oldRow)
                {
                    on_tbNextMonth_clicked();
                }
                m_oldRow = m_View.currentIndex().row();
                UpdateSelect();
                break;
            case Qt::Key_Left:
                if(m_View.currentIndex().column() == m_oldCol)
                {  
                    on_tbPreviousMonth_clicked();
                }
                m_oldCol = m_View.currentIndex().column();
                UpdateSelect();
                break;
            case Qt::Key_Right:
                if(m_View.currentIndex().column() == m_oldCol)
                {
                    on_tbNextMonth_clicked();                   
                }
                m_oldCol = m_View.currentIndex().column();
                UpdateSelect();
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

int CLunarCalendar::UpdateSelect()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    if(m_oldRow >= pModel->rowCount())
        m_oldRow = pModel->rowCount() - 1;
    if(m_oldRow < 0)
        m_oldRow = 0;
    if(m_oldCol >= pModel->columnCount())
        m_oldCol = pModel->columnCount() - 1;
    if(m_oldCol < 0)
        m_oldCol = 0;
    QModelIndex index = pModel->index(m_oldRow, m_oldCol);
    //m_View.setCurrentIndex(index);
    QDate d = pModel->dateForCell(index.row(), index.column());
    if(d.isValid())
    {
        SetSelectedDate(d);
    }
    return 0;
}

int CLunarCalendar::AddHoliday(int month, int day, const QString &szName)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->AddHoliday(month, day, szName);
}

int CLunarCalendar::AddAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->AddAnniversary(month, day, szName);
}

int CLunarCalendar::AddLunarAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    return pModel->AddLunarAnniversary(month, day, szName);
}

int CLunarCalendar::SetTaskHandle(QSharedPointer<CGetTaskHandler> handler)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return -1;
    pModel->SetTaskHandle(handler);
    return 0;
}

int CLunarCalendar::SetViewType(_VIEW_TYPE type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetViewType(type);
    
    int yearMin = pModel->GetMinimumDate().year();
    if(GetViewType() == ViewTypeWeek)
    {
        pModel->GetMinimumDate().weekNumber(&yearMin);
        if(m_bShowBackgroupImage)
            m_View.setStyleSheet("border-image:none");
    }
    int yearMax = pModel->GetMaximumDate().year();
    if(GetViewType() == ViewTypeWeek)
        pModel->GetMaximumDate().weekNumber(&yearMax);
    
    SetYearRange(yearMin, yearMax);
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(pModel->GetShowYear()));
    
    SetSelectedDate(QDate::currentDate(), true);

    switch (GetViewType()) {
    case ViewTypeWeek:
        m_cmbMonth.setToolTip(tr("Week"));
        break;
    case ViewTypeMonth:
        m_cmbMonth.setToolTip(tr("Month"));
        break;
    }

    updateGeometry();
    return nRet;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendar::GetViewType() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return ViewTypeMonth;
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
        return static_cast<_CalendarType>(CalendarTypeLunar | CalendarTypeSolar);
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
                                          int nThreadNumber,
                                          bool bSaveAllDate)
{
    return  CLunarTable::Instance()->Generate(
                this->MinimumDate(),
                MaximumDate(),
                szFile,
                nThreadNumber, bSaveAllDate);
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
    return UpdateViewModel(true);
}

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
//        qDebug() << "m_View.verticalHeader()->isVisible()";
        w += m_View.verticalHeader()->sizeHint().width();
    }
    cm = m_View.verticalHeader()->contentsMargins();
    if(ViewTypeMonth == GetViewType())
        h = (m_View.verticalHeader()->minimumSectionSize() + marginH) * rows;
    else
        h = (m_View.verticalHeader()->minimumSectionSize() + marginH);
    h = h + marginH + cm.top() + cm.bottom();
    if(m_View.horizontalHeader()->isVisible())
    {
//        qDebug() << "m_View.horizontalHeader()->isVisible()";
        h += m_View.horizontalHeader()->sizeHint().height();
    }
    
//    qDebug() << "w:" << w << "h:" << h << "marginW:" << marginW
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

    switch (m_HeadPostion) {
    case Top:
    case Down:
        w = qMax(w, headerW);
        h += headerH;
        break;
    case Left:
    case Right:
        w += headerW;
        h = qMax(h, headerH);
        break;
    default:
        break;
    }
    
    cm = contentsMargins();
    w += cm.left() + cm.right();
    h += cm.top() + cm.bottom();
    
//    qDebug() << "w:" << w << "h:" << h;

    return QSize(w, h);
}
