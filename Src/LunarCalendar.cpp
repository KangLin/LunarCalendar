#include "LunarCalendar.h"
#include "ui_LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
#include <QKeyEvent>
#include <QModelIndex>
#include "CalendarLunar.h"
#include "LunarCalendarDelegate.h"

CLunarCalendar::CLunarCalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLunarCalendar),
    m_oldRow(0),
    m_oldCol(0),
    m_bShowToday(true),
    m_bShowHead(true),
    m_bUpdate(true)
{
    ui->setupUi(this);

    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);    
    
    //ui->tvMonth->setFocusPolicy(Qt::WheelFocus);
    SetShowGrid(true);
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
    ui->tvMonth->setFrameStyle(QFrame::NoFrame);
    ui->tvMonth->installEventFilter(this);
    
    for(int i = 0; i < 12; i++)
    {
        ui->cbMonth->addItem(QLocale::system().monthName(i + 1), i + 1);
    }

    ui->spYear->setValue(pModel->GetDate().year());
    ui->cbMonth->setCurrentIndex(ui->cbMonth->findData(pModel->GetDate().month()));
}

CLunarCalendar::~CLunarCalendar()
{
    delete ui;
}

int CLunarCalendar::ShowSelectTitle()
{
    QDate d;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -1;
    d = pModel->GetDate();
    if(d.isNull()) return -2;
    QLocale native = QLocale::system();
    CCalendarLunar l(d);
    ui->lbDateText->setText(d.toString(QLocale::system().dateFormat(QLocale::LongFormat))
                            + "\n"
                            + l.GetLunar());
    return 0;
}

void CLunarCalendar::on_spYear_valueChanged(int value)
{
    Q_UNUSED(value);
    ChangeMonth();
    
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
        i = ui->cbMonth->count() - 1;
        ui->spYear->setValue(ui->spYear->value() - 1);
    }
    ui->cbMonth->setCurrentIndex(i);
}

void CLunarCalendar::on_cbMonth_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ChangeMonth();
}

void CLunarCalendar::on_pbToday_clicked()
{
    ui->spYear->setValue(QDate::currentDate().year());
    int nIndex = ui->cbMonth->findData(QDate::currentDate().month());
    if(nIndex > -1)
        ui->cbMonth->setCurrentIndex(nIndex);
    SetSelectedDate(QDate::currentDate());
    ChangeMonth();
}

int CLunarCalendar::ChangeMonth()
{
    if(!m_bUpdate)
        return -1;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel)
        return -2;
    pModel->showMonth(ui->spYear->value(), ui->cbMonth->currentData().toInt());
    ShowSelectTitle();
    return 0;
}

void CLunarCalendar::SetShowGrid(bool show)
{
    ui->tvMonth->setShowGrid(show);
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
    if(m_bShowHead == bShow)
        return;
    m_bShowHead = bShow;
    ui->spYear->setVisible(bShow);
    ui->tbNext->setVisible(bShow);
    ui->tbPrevious->setVisible(bShow);
    ui->cbMonth->setVisible(bShow);
    ui->lbDateText->setVisible(bShow);
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
        ui->cbMonth->setCurrentIndex(ui->cbMonth->findData(newDate.month()));
        ChangeMonth();
    }   

    int row, col;
    pModel->cellForDate(pModel->GetDate(), &row, &col);
    if(row >= 0 && col >= 0)
    {
        ui->tvMonth->selectionModel()->clear();
        ui->tvMonth->selectionModel()->setCurrentIndex(pModel->index(row, col),
                                           QItemSelectionModel::SelectCurrent);
        //ui->tvMonth->setCurrentIndex(pModel->index(row, col));
        ui->tvMonth->selectionModel()->select(pModel->index(row, col),
                                            QItemSelectionModel::SelectCurrent);
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
    m_bUpdate = false;
    QDate oldDate = pModel->GetDate();
    pModel->SetMaximumDate(date);
    ui->spYear->setMaximum(date.year());
    UpdateMonthMenu();
    QDate newDate = pModel->GetDate();
    m_bUpdate = true;
    if (oldDate != newDate) {
        ChangeMonth();
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
    m_bUpdate = false;
    QDate oldDate = pModel->GetDate();
    pModel->SetMinimumDate(date);
    ui->spYear->setMinimum(date.year());
    UpdateMonthMenu();
    m_bUpdate = true;
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        ChangeMonth();
        emit sigSelectionChanged();
    }
}

void CLunarCalendar::SetDateRange(const QDate &min, const QDate &max)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return;
    if(pModel->GetMaximumDate() == max && pModel->GetMinimumDate() == min)
        return;
    if (!min.isValid() || !max.isValid())
        return;

    if(QDate::currentDate() < min || QDate::currentDate() > max)
        ui->pbToday->setVisible(false);
    else
        SetShowToday(m_bShowToday);
    m_bUpdate = false;
    QDate oldDate = pModel->GetDate();
    pModel->setRange(min, max);
    ui->spYear->setRange(min.year(), max.year());

    UpdateMonthMenu();
    m_bUpdate = true;
    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        ChangeMonth();
        emit sigSelectionChanged();
    }
}

int CLunarCalendar::UpdateMonthMenu()
{
    int beg = 1, end = 12;
    bool prevEnabled = true;
    bool nextEnabled = true;
    m_bUpdate = false;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    if (pModel->GetShowYear() == pModel->GetMinimumDate().year()) {
        beg = pModel->GetMinimumDate().month();
        if (pModel->GetShowMonth() == pModel->GetMinimumDate().month())
            prevEnabled = false;
    }
    if (pModel->GetShowYear() == pModel->GetMaximumDate().year()) {
        end = pModel->GetMaximumDate().month();
        if (pModel->GetShowMonth() == pModel->GetMaximumDate().month())
            nextEnabled = false;
    }

    ui->tbPrevious->setEnabled(prevEnabled);
    ui->tbNext->setEnabled(nextEnabled);
    
    int index = ui->cbMonth->currentIndex();
    ui->cbMonth->clear();
    for (int i = beg; i <= end; i++) {
        ui->cbMonth->addItem(QLocale::system().monthName(i), i);
    }
    if(index >= ui->cbMonth->count())
        index = ui->cbMonth->count() - 1;
    if(index < 0)
        index = 0;
    ui->cbMonth->setCurrentIndex(index);
    m_bUpdate = true;
    return 0;
}

Qt::DayOfWeek CLunarCalendar::FirstDayOfWeek() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return QLocale::system().firstDayOfWeek();
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
    ui->tvMonth->setCurrentIndex(index);
    QDate d = pModel->dateForCell(index.row(), index.column());
    if(d.isValid())
    {
        pModel->setDate(d);
        ShowSelectTitle();
        emit sigSelectionChanged();
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
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(!pModel) return -1;
    return pModel->AddAnniversary(month, day, szName);
}
