#include "LunarCalendarDelegate.h"
#include "FrmCell.h"
#include "LunarCalendarModel.h"
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QPalette>
#include <QDebug>
#include <QColor>

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}

void CLunarCalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //*
    CFrmCell cell;
    QPalette palette, paletteLunar;
    palette = option.palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(index.data(Qt::BackgroundRole).value<QColor>())));
    palette.setBrush(QPalette::Foreground, QBrush(QColor(index.data(Qt::ForegroundRole).value<QColor>())));
    paletteLunar = option.palette;
    paletteLunar.setBrush(QPalette::Background, QBrush(QColor(index.data(Qt::BackgroundRole).value<QColor>())));
    paletteLunar.setBrush(QPalette::Foreground, QBrush(QColor(index.data(CLunarCalendarModel::LunarHolidayColorRole).value<QColor>())));
    
    QString szLunar = index.data(CLunarCalendarModel::SolarHoliday).toString();
    if(szLunar.isEmpty())
        szLunar = index.data(CLunarCalendarModel::LunarHolidayRole).toString();
    if(szLunar.isEmpty())
        szLunar = index.data(CLunarCalendarModel::LunarRole).toString();
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  palette,
                  szLunar,
                  paletteLunar);
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    if(pView->horizontalHeader()->minimumSectionSize() < cell.width())
        pView->horizontalHeader()->setMinimumSectionSize(cell.width());
    if(pView->verticalHeader()->minimumSectionSize() < cell.height())
        pView->verticalHeader()->setMinimumSectionSize(cell.height());

    painter->save();
    painter->translate(option.rect.topLeft());
    painter->setBrush(option.backgroundBrush);

    cell.resize(option.rect.size());
    cell.render(painter);
    painter->restore();

    //*/
    
    //获取项风格设置    
    /*
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    QApplication::style()->drawItemText ( painter, myOption.rect ,
                                          myOption.displayAlignment,
                                          myOption.palette, false,
                                          index.data(CLunarCalendarModel::SolarRole).toString() );
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;
    QApplication::style()->drawItemText ( painter, myOption.rect,
                                          myOption.displayAlignment,
                                          myOption.palette, true,
                                          index.data(CLunarCalendarModel::LunarRole).toString() );
    //QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, &myOption, painter);
    //*/
}

QSize CLunarCalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CFrmCell cell;
    return cell.size();
}
