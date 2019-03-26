#include "LunarCalendar.h"
#include "ui_LunarCalendar.h"
#include <QDate>
#include <QLocale>
#include <QDebug>
#include "LunarCalendarModel.h"

CLunarCalendar::CLunarCalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLunarCalendar)
{
    ui->setupUi(this);
    
    ui->tvMonth->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tvMonth->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvMonth->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMonth->horizontalHeader()->setSectionsClickable(false);
    ui->tvMonth->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvMonth->verticalHeader()->setSectionsClickable(false);
    ui->tvMonth->setModel(new CLunarCalendarModel(this));
    ui->tvMonth->setFrameStyle(QFrame::NoFrame);
    
    for(int i = 0; i < 12; i++)
    {
        ui->cbMonth->addItem(QLocale::system().monthName(i + 1));
    }
    
    ui->spYear->setValue(QDate::currentDate().year());
    ui->cbMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    ChangeTitle(QDate::currentDate());
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
}

void CLunarCalendar::on_tbNext_clicked()
{
    int i = ui->cbMonth->currentIndex() + 1;
    if(i > 11)
        i = 0;
    ui->cbMonth->setCurrentIndex(i);
    ChangeMonth();
}

void CLunarCalendar::on_tbPrevious_clicked()
{
    int i = ui->cbMonth->currentIndex() - 1;
    if(i < 0)
        i = 11;
    ui->cbMonth->setCurrentIndex(i);
    ChangeMonth();
}

void CLunarCalendar::on_cbMonth_currentIndexChanged(int index)
{
    ChangeMonth();
}

void CLunarCalendar::on_pbToday_clicked()
{
    ChangeTitle(QDate::currentDate());
    ChangeMonth();
}

int CLunarCalendar::ChangeMonth()
{
    CLunarCalendarModel* pModel = static_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    pModel->showMonth(ui->spYear->value(), ui->cbMonth->currentIndex() + 1);
    ChangeTitle(selectedDate());
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
    CLunarCalendarModel* pModel = static_cast<CLunarCalendarModel*>(ui->tvMonth->model());
    return pModel->GetDate();
}
