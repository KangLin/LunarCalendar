#include "LunarCalendar.h"
#include "ui_LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
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
    ui(new Ui::CLunarCalendar),
    m_oldRow(0),
    m_oldCol(0),
    m_bShowToday(true),
    m_bShowBackgroupImage(true)
{
    //setLocale(QLocale("zh_CN"));
    ui->setupUi(this);

    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);    

    bool check = connect(&m_Timer, SIGNAL(timeout()),
                         this, SLOT(slotTimeout()));
    Q_ASSERT(check);
    SetShowHead(true);
    
    SetShowGrid(false);
    //ui->tvMonth->setFocusPolicy(Qt::WheelFocus);
    ui->tvMonth->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tvMonth->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvMonth->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvMonth->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    /*
    ui->tvMonth->horizontalHeader()->setMinimumSize(cell.size());
    ui->tvMonth->horizontalHeader()->setMinimumSectionSize(cell.width());
    ui->tvMonth->verticalHeader()->setMinimumSize(cell.size());
    ui->tvMonth->verticalHeader()->setMinimumSectionSize(cell.height());
    */
    ui->tvMonth->horizontalHeader()->setSectionsClickable(false);
    ui->tvMonth->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMonth->verticalHeader()->setSectionsClickable(false);
    ui->tvMonth->setModel(pModel);
    ui->tvMonth->setItemDelegate(new CLunarCalendarDelegate(ui->tvMonth));
    ui->tvMonth->horizontalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(ui->tvMonth));
    ui->tvMonth->verticalHeader()->setItemDelegate(new CLunarCalendarHeaderDelegate(ui->tvMonth));
    ui->tvMonth->setFrameStyle(QFrame::NoFrame);
    ui->tvMonth->installEventFilter(this);
    //ui->tvMonth->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //ui->tvMonth->setAlternatingRowColors(true); //设置奇偶行颜色  
//    QFont font = ui->tvMonth->font();
//    font.setPointSize(6);
//    ui->tvMonth->setFont(font);

    //ui->tvMonth->setStyleSheet("background-color:rgba(0,0,0,0)"); //设置背景透明
    //setStyleSheet("border-image:url(d:/temp/4.jpg)"); //设置背景图片
    
    for(int i = 0; i < 12; i++)
    {
        ui->cbMonth->addItem(locale().monthName(i + 1), i + 1);
    }
    ui->cbMonth->setToolTip(tr("Month"));
    
    ui->spYear->setRange(pModel->GetMinimumDate().year(), 
                         pModel->GetMaximumDate().year());
    ui->spYear->setValue(pModel->GetDate().year());
    
    ui->cbMonth->setCurrentIndex(ui->cbMonth->findData(pModel->GetDate().month()));
    SetSelectedDate(pModel->GetDate());
}

CLunarCalendar::~CLunarCalendar()
{
    delete ui;
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
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -1;
    d = pModel->GetDate();
    if(d.isNull()) return -2;
    QString szDate;
    if(CalendarTypeSolar & GetCalendarType())
        szDate = d.toString(locale().dateFormat(QLocale::LongFormat));
    if(CalendarTypeLunar & GetCalendarType())
        szDate += " " + SelectedLunar();
    ui->lbDateText->setText(szDate);
    return 0;
}

void CLunarCalendar::on_spYear_valueChanged(int value)
{
    Q_UNUSED(value);
    UpdateViewModel();
    UpdateMonthMenu();
}

void CLunarCalendar::on_tbNext_clicked()
{
    if(!ui->tbNext->isEnabled())
        return;
    int i = ui->cbMonth->currentIndex() + 1;
    if(i > ui->cbMonth->count() - 1)
    {
        i = 0;
        ui->spYear->setValue(ui->spYear->value() + 1);
    }
    ui->cbMonth->setCurrentIndex(i);
}

void CLunarCalendar::on_tbPrevious_clicked()
{
    if(!ui->tbPrevious->isEnabled())
        return;
    int i = ui->cbMonth->currentIndex() - 1;
    if(i < 0)
    {
        ui->spYear->setValue(ui->spYear->value() - 1);
        i = ui->cbMonth->count() - 1;
    }
    ui->cbMonth->setCurrentIndex(i);
}

void CLunarCalendar::on_cbMonth_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    UpdateViewModel();
    EnableMonthMenu();
}

void CLunarCalendar::on_pbToday_clicked()
{
    ui->spYear->setValue(QDate::currentDate().year());
    int nIndex = ui->cbMonth->findData(QDate::currentDate().month());
    if(nIndex > -1)
        ui->cbMonth->setCurrentIndex(nIndex);
    SetSelectedDate(QDate::currentDate());
}

int CLunarCalendar::UpdateViewModel()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -2;
    //ui->tvMonth->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
    ui->tvMonth->selectionModel()->clear();
    switch (GetViewType())
    {
    case ViewTypeMonth:
    {
        pModel->showMonth(ui->spYear->value(), ui->cbMonth->currentData().toInt());
        if(pModel && m_bShowBackgroupImage)
        {
            QString szBackgrpup = ":/image/" + QString::number(pModel->GetShowMonth());
            SetBackgroup(szBackgrpup);
        }
        break;
    }
    case ViewTypeWeek:
        pModel->showWeek(ui->spYear->value(), ui->cbMonth->currentData().toInt());
        break;
    }
    
    ShowSelectTitle();
    return 0;
}

void CLunarCalendar::SetShowGrid(bool show)
{
    ui->tvMonth->setShowGrid(show);
}

bool CLunarCalendar::ShowGrid()
{
    return ui->tvMonth->showGrid();
}

void CLunarCalendar::SetShowBackgroupImage(bool show)
{
    m_bShowBackgroupImage = show;
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
    ui->pbToday->setVisible(m_bShowToday);
}

void CLunarCalendar::SetShowWeeks(bool bShow)
{
    ui->tvMonth->verticalHeader()->setVisible(bShow);
}

void CLunarCalendar::SetShowWeekHead(bool bShow)
{
    ui->tvMonth->horizontalHeader()->setVisible(bShow);
}

void CLunarCalendar::SetShowHead(bool bShow)
{
    SetShowTools(bShow);
    ui->lbDateText->setVisible(bShow);
    SetShowTime(bShow);
}

void CLunarCalendar::SetShowTools(bool bShow)
{
    ui->spYear->setVisible(bShow);
    ui->tbNext->setVisible(bShow);
    ui->tbPrevious->setVisible(bShow);
    ui->cbMonth->setVisible(bShow);
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
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return QDate();
    return pModel->GetDate();
}

void CLunarCalendar::SetSelectedDate(const QDate &date)
{
    if (!date.isValid())
        return;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    if(pModel->GetDate() != date)
    {
        pModel->setDate(date);
        QDate newDate = pModel->GetDate();
        
        ui->spYear->setValue(newDate.year());
        switch (GetViewType()) {
        case ViewTypeMonth:
            ui->cbMonth->setCurrentIndex(ui->cbMonth->findData(newDate.month()));
            break;
        case ViewTypeWeek:
            {
                int week = 1;
                int year = 0;
                week = newDate.weekNumber(&year);
                if(year != newDate.year())
                {
                    ui->spYear->setValue(year);
                }
                ui->cbMonth->setCurrentIndex(ui->cbMonth->findData(week));
            }
            break;
        }
        
        UpdateViewModel();
    }   

    int row, col;
    pModel->cellForDate(pModel->GetDate(), &row, &col);
    if(row >= 0 && col >= 0)
    {
        CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
        if(pModel && m_bShowBackgroupImage)
        {
            QString szJiQi = pModel->data(pModel->index(row, col),
                                CLunarCalendarModel::BackgroupImage).toString();
            if(!szJiQi.isEmpty())
                SetBackgroup(szJiQi);
        }
        
        ui->tvMonth->selectionModel()->clear();
        ui->tvMonth->selectionModel()->setCurrentIndex(pModel->index(row, col),
                                           QItemSelectionModel::SelectCurrent);
        //ui->tvMonth->setCurrentIndex(pModel->index(row, col));
        //ui->tvMonth->selectionModel()->select(pModel->index(row, col),
        //                                    QItemSelectionModel::SelectCurrent);
        ui->tvMonth->setFocus();
        m_oldCol = col;
        m_oldRow = row;
    }
    emit sigSelectionChanged();
}

QString CLunarCalendar::SelectedLunar()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return QString();
    QDate date = pModel->GetDate();
    CCalendarLunar l(date);
    return l.GetLunar();
}

int CLunarCalendar::YearShown() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->GetShowYear();
}

int CLunarCalendar::MonthShown() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->GetShowMonth();
}

QDate CLunarCalendar::MaximumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return QDate();
    return pModel->GetMaximumDate();
}

void CLunarCalendar::SetMaximumDate(const QDate &date)
{
    if(!date.isValid())
        return;
    
    if(QDate::currentDate() > date)
        ui->pbToday->setVisible(false);
    else
        SetShowToday(m_bShowToday);
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    QDate oldDate = pModel->GetDate();
    pModel->SetMaximumDate(date);
    int year = date.year();
    if(GetViewType() == ViewTypeWeek)
    {
        date.weekNumber(&year);
    }
    ui->spYear->setMaximum(year);
    
    if(ui->spYear->value() > year)
        ui->spYear->setValue(year);
    else 
        UpdateMonthMenu();
    
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        UpdateViewModel();
        emit sigSelectionChanged();
    }
}

QDate CLunarCalendar::MinimumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return QDate();
    return pModel->GetMinimumDate();
}

void CLunarCalendar::SetMinimumDate(const QDate &date)
{
    if(!date.isValid())
        return;
    
    if(QDate::currentDate() < date)
        ui->pbToday->setVisible(false);
    else
        SetShowToday(m_bShowToday);
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    QDate oldDate = pModel->GetDate();
    pModel->SetMinimumDate(date);
    int year = date.year();
    if(GetViewType() == ViewTypeWeek)
    {
        date.weekNumber(&year);
    }
    ui->spYear->setMinimum(year);
    if(pModel->GetShowYear() < year)
        ui->spYear->setValue(year);
    else 
        UpdateMonthMenu();
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
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    if(pModel->GetMaximumDate() == max && pModel->GetMinimumDate() == min)
        return;
    
    if(QDate::currentDate() < min || QDate::currentDate() > max)
        ui->pbToday->setVisible(false);
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
    
    ui->spYear->setRange(yearMin, yearMax);
    if(ui->spYear->value() < yearMin)
        ui->spYear->setValue(yearMin);
    else if (ui->spYear->value() > yearMax) 
        ui->spYear->setValue(yearMax);
    else
        UpdateMonthMenu();
    
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        UpdateViewModel();
        emit sigSelectionChanged();
    }
}

int CLunarCalendar::EnableMonthMenu()
{
    bool prevEnabled = true;
    bool nextEnabled = true;
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
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
    
    ui->tbPrevious->setEnabled(prevEnabled);
    ui->tbNext->setEnabled(nextEnabled);
    return 0;
}

int CLunarCalendar::UpdateMonthMenu()
{
    int beg = 1, end = 12;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    
    EnableMonthMenu();

    int minYear = pModel->GetMinimumDate().year();
    int maxYear = pModel->GetMaximumDate().year();
    
    if (GetViewType() == ViewTypeWeek) {
        end = pModel->GetWeeksOfYear(ui->spYear->value());
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
    
    int index = ui->cbMonth->currentIndex();
    ui->cbMonth->clear();
    for (int i = beg; i <= end; i++) {
        switch (GetViewType()) {
        case ViewTypeMonth:
            ui->cbMonth->addItem(locale().monthName(i), i);
            break;
        case ViewTypeWeek:
            ui->cbMonth->addItem(QString::number(i), i);
            break;
        }
    }
    if(index >= ui->cbMonth->count())
        index = ui->cbMonth->count() - 1;
    if(index < 0)
        index = 0;
    ui->cbMonth->setCurrentIndex(index);

    return 0;
}

Qt::DayOfWeek CLunarCalendar::FirstDayOfWeek() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return locale().firstDayOfWeek();
    return pModel->firstDayOfWeek();
}

void CLunarCalendar::SetFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    pModel->setFirstDayOfWeek(dayOfWeek);
}

void CLunarCalendar::on_tvMonth_pressed(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
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
            QWheelEvent *we = (QWheelEvent*)event;
            const int numDegrees = we->delta() / 8;
            const int numSteps = numDegrees / 15;
            CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
            if(pModel)
            {
                const QModelIndex index = ui->tvMonth->currentIndex();
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
            QKeyEvent* ke = (QKeyEvent*)event;
            switch (ke->key()) {
            case Qt::Key_Up:
                if(ui->tvMonth->currentIndex().row() == m_oldRow)
                {
                    on_tbPrevious_clicked();
                }
                m_oldRow = ui->tvMonth->currentIndex().row();
                UpdateSelect();
                break;
            case Qt::Key_Down:
                if(ui->tvMonth->currentIndex().row() == m_oldRow)
                {
                    on_tbNext_clicked();
                }
                m_oldRow = ui->tvMonth->currentIndex().row();
                UpdateSelect();
                break;
            case Qt::Key_Left:
                if(ui->tvMonth->currentIndex().column() == m_oldCol)
                {  
                    on_tbPrevious_clicked();
                }
                m_oldCol = ui->tvMonth->currentIndex().column();
                UpdateSelect();
                break;
            case Qt::Key_Right:
                if(ui->tvMonth->currentIndex().column() == m_oldCol)
                {
                    on_tbNext_clicked();                   
                }
                m_oldCol = ui->tvMonth->currentIndex().column();
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
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
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
    //ui->tvMonth->setCurrentIndex(index);
    QDate d = pModel->dateForCell(index.row(), index.column());
    if(d.isValid())
    {
        SetSelectedDate(d);
    }
    return 0;
}

int CLunarCalendar::AddHoliday(int month, int day, const QString &szName)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->AddHoliday(month, day, szName);
}

int CLunarCalendar::AddAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->AddAnniversary(month, day, szName);
}

int CLunarCalendar::AddLunarAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->AddLunarAnniversary(month, day, szName);
}

int CLunarCalendar::SetViewType(_VIEW_TYPE type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetViewType(type);
    switch (GetViewType()) {
    case ViewTypeWeek:
        ui->cbMonth->setToolTip(tr("Week"));
        break;
    case ViewTypeMonth:
        ui->cbMonth->setToolTip(tr("Month"));
        break;
    }
    
    int yearMin = pModel->GetMinimumDate().year();
    if(GetViewType() == ViewTypeWeek)
    {
        pModel->GetMinimumDate().weekNumber(&yearMin);
    }
    int yearMax = pModel->GetMaximumDate().year();
    if(GetViewType() == ViewTypeWeek)
        pModel->GetMaximumDate().weekNumber(&yearMax);
    
    ui->spYear->setRange(yearMin, yearMax);
    if(ui->spYear->value() < yearMin)
        ui->spYear->setValue(yearMin);
    else if (ui->spYear->value() > yearMax) 
        ui->spYear->setValue(yearMax);
    else
        UpdateMonthMenu();
    
    UpdateViewModel();
    UpdateMonthMenu();
    return nRet;
}

CLunarCalendar::_VIEW_TYPE CLunarCalendar::GetViewType()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return ViewTypeMonth;
    return pModel->GetViewType();
}

CLunarCalendar::_CalendarType CLunarCalendar::GetCalendarType()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return static_cast<_CalendarType>(CalendarTypeLunar | CalendarTypeSolar);
    return pModel->GetCalendarType();
}

int CLunarCalendar::SetCalendarType(_CalendarType type)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -1;
    int nRet = pModel->SetCalendarType(type);
    ShowSelectTitle();
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
    ui->lbTime->setText(QTime::currentTime().toString(locale().timeFormat())); // + " " + QString::number(QTime::currentTime().msec()));
}

void CLunarCalendar::SetShowTime(bool bShow)
{
    ui->lbTime->setVisible(bShow);
    if(bShow)
        m_Timer.start(1000);
    else
        m_Timer.stop();
}

int CLunarCalendar::SetBackgroup(const QString &szFile)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -1;

    QPalette palette = ui->tvMonth->palette();
    ui->tvMonth->setAutoFillBackground(m_bShowBackgroupImage);
    if(!m_bShowBackgroupImage)
    {
        palette.setBrush(QPalette::Window, QBrush());
        setPalette(palette);
        return 0;
    }
    qDebug() << "File: " << szFile << ui->tvMonth->size();
    palette.setBrush(QPalette::Background,
                     QBrush(QPixmap(szFile).scaled(
                                ui->tvMonth->size(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation)));// 使用平滑的缩放方式  
    ui->tvMonth->setPalette(palette);// 给widget加上背景图  
    return 0;
}
