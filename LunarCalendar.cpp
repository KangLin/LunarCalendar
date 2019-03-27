#include "LunarCalendar.h"
#include "ui_LunarCalendar.h"
#include "LunarCalendarModel.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
#include <QKeyEvent>
#include <QModelIndex>

CLunarCalendar::CLunarCalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLunarCalendar),
    m_oldRow(0),
    m_oldCol(0)
{
    ui->setupUi(this);
    
    ui->tvMonth->setFocusPolicy(Qt::WheelFocus);
    ui->tvMonth->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tvMonth->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvMonth->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvMonth->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMonth->horizontalHeader()->setSectionsClickable(false);
    ui->tvMonth->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMonth->verticalHeader()->setSectionsClickable(false);
    CLunarCalendarModel* pModel = new CLunarCalendarModel(this);
    ui->tvMonth->setModel(pModel);
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

int CLunarCalendar::ChangeTitle(const QDate &date)
{
    QLocale native = QLocale::system();
    ui->lbDateText->setText(date.toString(QLocale::system().dateFormat(QLocale::LongFormat)));
    return 0;
}

void CLunarCalendar::on_spYear_valueChanged(int)
{
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
    ChangeMonth();
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
    ChangeMonth();
}

void CLunarCalendar::on_cbMonth_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ChangeMonth();
}

void CLunarCalendar::on_pbToday_clicked()
{
    ChangeTitle(QDate::currentDate());
    ui->spYear->setValue(QDate::currentDate().year());
    int nIndex = ui->cbMonth->findData(QDate::currentDate().month());
    if(nIndex > -1)
        ui->cbMonth->setCurrentIndex(nIndex);
    ChangeMonth();
}

int CLunarCalendar::ChangeMonth()
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    pModel->showMonth(ui->spYear->value(), ui->cbMonth->currentData().toInt());
    setSelectedDate(pModel->GetDate());
    ChangeTitle(pModel->GetDate());
    return 0;
}

void CLunarCalendar::setShowGrid(bool show)
{
    ui->tvMonth->setShowGrid(show);
}

/*!
    \property QCalendarWidget::selectedDate
    \brief the currently selected date.

    The selected date must be within the date range specified by the
    minimumDate and maximumDate properties. By default, the selected
    date is the current date.

    \sa setDateRange()
*/

QDate CLunarCalendar::selectedDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetDate();
}

void CLunarCalendar::setSelectedDate(const QDate &date)
{
    if (!date.isValid())
        return;

    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(pModel->GetDate() != date)
    {
        pModel->setDate(date);
        QDate newDate = pModel->GetDate();
        pModel->showMonth(newDate.year(), newDate.month());
    }   

    int row, col;
    pModel->cellForDate(pModel->GetDate(), &row, &col);
    if(row >= 0 && col >= 0)
    {
        ui->tvMonth->selectionModel()->clear();
        //ui->tvMonth->selectionModel()->setCurrentIndex(pModel->index(row, col),
        //                                   QItemSelectionModel::SelectCurrent);
        ui->tvMonth->selectionModel()->select(pModel->index(row, col),
                                            QItemSelectionModel::SelectCurrent);
        ui->tvMonth->setFocus();
    }
    emit sigSelectionChanged();
}

int CLunarCalendar::yearShown() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetShowYear();
}

int CLunarCalendar::monthShown() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetShowMonth();
}

QDate CLunarCalendar::maximumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetMaximumDate();
}

void CLunarCalendar::setMaximumDate(const QDate &date)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    pModel->SetMaximumDate(date);
}

QDate CLunarCalendar::minimumDate() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetMinimumDate();
}

void CLunarCalendar::setMinimumDate(const QDate &date)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    pModel->SetMinimumDate(date);
}

void CLunarCalendar::setDateRange(const QDate &min, const QDate &max)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    if(pModel->GetMaximumDate() == max && pModel->GetMinimumDate() == min)
        return;
    if (!min.isValid() || !max.isValid())
        return;

    QDate oldDate = pModel->GetDate();
    pModel->setRange(min, max);
    ui->spYear->setRange(min.year(), max.year());

    UpdateMonthMenu();

    QDate newDate = pModel->GetDate();
    if (oldDate != newDate) {
        pModel->showMonth(newDate.year(), newDate.month());
        emit sigSelectionChanged();
    }
}

int CLunarCalendar::UpdateMonthMenu()
{
    int beg = 1, end = 12;
    bool prevEnabled = true;
    bool nextEnabled = true;
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
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
    ui->cbMonth->clear();

    for (int i = beg; i <= end; i++) {
        ui->cbMonth->addItem(QLocale::system().monthName(i), i);
    }
    return 0;
}

Qt::DayOfWeek CLunarCalendar::firstDayOfWeek() const
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->firstDayOfWeek();
}

void CLunarCalendar::setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
{
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    pModel->setFirstDayOfWeek(dayOfWeek);
}

void CLunarCalendar::on_tvMonth_pressed(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    m_oldCol = index.column();
    m_oldCol = index.row();
    CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    QDate d = pModel->dateForCell(index.row(), index.column());
    if(d.isValid())
        setSelectedDate(d);
    ChangeMonth();
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
                    on_tbPrevious_clicked();
                m_oldRow = ui->tvMonth->currentIndex().row();
                break;
            case Qt::Key_Down:
                if(ui->tvMonth->currentIndex().row() == m_oldRow)
                {
                    on_tbNext_clicked();
                    UpdateSelect();
                }
                m_oldRow = ui->tvMonth->currentIndex().row();
                break;
            case Qt::Key_Left:
                if(ui->tvMonth->currentIndex().column() == m_oldCol)
                {  
                    on_tbPrevious_clicked();
                    UpdateSelect();
                }
                m_oldCol = ui->tvMonth->currentIndex().column();
                break;
            case Qt::Key_Right:
                if(ui->tvMonth->currentIndex().column() == m_oldCol)
                {
                    on_tbNext_clicked();
                    UpdateSelect();
                }
                m_oldCol = ui->tvMonth->currentIndex().column();
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
    if(m_oldRow != ui->tvMonth->model()->rowCount() - 1)
        m_oldRow = ui->tvMonth->model()->rowCount() - 1;
    QModelIndex index = ui->tvMonth->model()->index(
                m_oldRow,
                m_oldCol);
    ui->tvMonth->setCurrentIndex(index);
    return 0;
}
