#include "LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QModelIndex>
#include "CalendarLunar.h"
#include "LunarCalendarDelegate.h"
#include <QTranslator>
#include <QApplication>
#include <QDir>
#include <QStyleOption>
#include <QPainter>
#include "CalendarLunar.h"
#include "LunarTable.h"

class CLunarCalendarPrivate
{
public:
    CLunarCalendarPrivate() 
    {
        QString szPre;    
    #if defined(Q_OS_ANDROID) || _DEBUG
        szPre = ":/Translations";
    #else
        szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
    #endif
        m_Translator.load(szPre + "/LunarCalendar_" + QLocale::system().name() + ".qm");
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
    m_oldRow(0),
    m_oldCol(0),
    m_bShowToday(true),
    m_bShowBackgroupImage(false)
{
    //setLocale(QLocale("zh_CN"));
    
    SetShowHead(true);
    m_tbPreYear.setArrowType(Qt::UpArrow);
    m_tbNextYear.setArrowType(Qt::DownArrow);
    m_tbPreMonth.setArrowType(Qt::UpArrow);
    m_tbNextMonth.setArrowType(Qt::DownArrow);
    m_lbDate.setAlignment(Qt::AlignCenter);
    m_lbTime.setAlignment(Qt::AlignCenter);
    m_pbToday.setText(tr("Today"));
        
    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);
    SetShowGrid(false);
    //m_View.setFocusPolicy(Qt::WheelFocus);
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
    m_View.setModel(pModel);
    m_View.setItemDelegate(new CLunarCalendarDelegate(&m_View));
    m_View.horizontalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    m_View.verticalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(&m_View));
    m_View.setFrameStyle(QFrame::NoFrame);
    m_View.installEventFilter(this);
    //m_View.setAutoScroll(false);
    //m_View.setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //m_View.setAlternatingRowColors(true); //设置奇偶行颜色
//    QFont font = m_View.font();
//    font.setPointSize(6);
//    m_View.setFont(font);

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
    
    QHBoxLayout* pToolLayout = new QHBoxLayout();
    pToolLayout->addWidget(&m_tbPreYear);
    pToolLayout->addWidget(&m_cmbYear);
    pToolLayout->addWidget(&m_tbNextYear);
    pToolLayout->addWidget(&m_tbPreMonth);
    pToolLayout->addWidget(&m_cmbMonth);
    pToolLayout->addWidget(&m_tbNextMonth);
    pToolLayout->addWidget(&m_pbToday);
    
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addLayout(pToolLayout);
    pVLayout->addWidget(&m_lbDate);
    pVLayout->addWidget(&m_lbTime);
    pVLayout->addWidget(&m_View);
    pVLayout->setMargin(0);
    setLayout(pVLayout);
    
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
}

CLunarCalendar::~CLunarCalendar()
{
}

void CLunarCalendar::InitResource()
{
    if(nullptr == g_pLunarCalendarPrivate)
        g_pLunarCalendarPrivate = new CLunarCalendarPrivate();

    Q_INIT_RESOURCE(ResourceLunarCalendar);
#if defined(Q_OS_ANDROID) || _DEBUG
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
#if defined(Q_OS_ANDROID) || _DEBUG
    Q_CLEANUP_RESOURCE(translations_LunarCalendar);
#endif
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
    qDebug() << "CLunarCalendar::on_cbMonth_currentIndexChanged";
    Q_UNUSED(index);
    UpdateViewModel();
    EnableMonthMenu();
}

void CLunarCalendar::on_pbToday_clicked()
{
    m_cmbYear.setCurrentIndex(m_cmbYear.findData(QDate::currentDate().year()));
    int nIndex = m_cmbMonth.findData(QDate::currentDate().month());
    if(nIndex > -1)
        m_cmbMonth.setCurrentIndex(nIndex);
    SetSelectedDate(QDate::currentDate());
}

int CLunarCalendar::UpdateViewModel()
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
                          m_cmbMonth.currentData().toInt());
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
                         m_cmbMonth.currentData().toInt());
        break;
    }
    
    ShowSelectTitle();
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

void CLunarCalendar::SetShowToday(bool bShow)
{
    if(m_bShowToday == bShow)
        return;
    m_bShowToday = bShow;
    m_pbToday.setVisible(m_bShowToday);
}

void CLunarCalendar::SetShowWeeks(bool bShow)
{
    m_View.verticalHeader()->setVisible(bShow);
}

void CLunarCalendar::SetShowWeekHead(bool bShow)
{
    m_View.horizontalHeader()->setVisible(bShow);
}

void CLunarCalendar::SetShowHead(bool bShow)
{
    SetShowTools(bShow);
    m_lbDate.setVisible(bShow);
    SetShowTime(bShow);
}

void CLunarCalendar::SetShowTools(bool bShow)
{
    m_tbPreYear.setVisible(bShow);
    m_tbNextYear.setVisible(bShow);
    m_cmbYear.setVisible(bShow);
    m_tbNextMonth.setVisible(bShow);
    m_tbPreMonth.setVisible(bShow);
    m_cmbMonth.setVisible(bShow);
    SetShowToday(bShow);
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

void CLunarCalendar::SetSelectedDate(const QDate &date)
{
    if (!date.isValid())
        return;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    if(pModel->GetDate() != date)
    {
        pModel->setDate(date);
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
    }   

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
        SetShowToday(m_bShowToday);
    
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
        SetShowToday(m_bShowToday);
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
        SetShowToday(m_bShowToday);
    
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
    Q_ASSERT(check);
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

void CLunarCalendar::SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel) return;
    pModel->setFirstDayOfWeek(dayOfWeek);
}

void CLunarCalendar::on_tvMonth_pressed(const QModelIndex &index)
{
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
    switch(event->type()){
    
    case QEvent::TouchBegin:
        
        qDebug() << "QEvent::TouchBegin";
        break;
    case QEvent::TouchEnd:
        qDebug() << "QEvent::TouchEnd";
        break;
#ifndef QT_NO_WHEELEVENT
    case QEvent::Wheel:
        {
            QWheelEvent *we = dynamic_cast<QWheelEvent*>(event);
            const int numDegrees = we->delta() / 8;
            const int numSteps = numDegrees / 15;
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
                if(m_View.currentIndex().row() == m_oldRow)
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

int CLunarCalendar::SetViewType(_VIEW_TYPE type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetViewType(type);
    switch (GetViewType()) {
    case ViewTypeWeek:
        m_cmbMonth.setToolTip(tr("Week"));
        break;
    case ViewTypeMonth:
        m_cmbMonth.setToolTip(tr("Month"));
        break;
    }
    
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
    
    UpdateViewModel();
    UpdateMonthMenu();
    return nRet;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendar::GetViewType()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(m_View.model());
    if(!pModel)
        return ViewTypeMonth;
    return pModel->GetViewType();
}

CLunarCalendar::_CalendarType CLunarCalendar::GetCalendarType()
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

void CLunarCalendar::SetShowTime(bool bShow)
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
