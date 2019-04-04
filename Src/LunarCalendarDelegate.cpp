#include "LunarCalendarDelegate.h"
#include "FrmCell.h"
#include "LunarCalendarModel.h"
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}


void CLunarCalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //*
    CFrmCell cell;
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  index.data(CLunarCalendarModel::LunarRole).toString());
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    if(pView->horizontalHeader()->minimumSectionSize() < cell.width())
        pView->horizontalHeader()->setMinimumSectionSize(cell.width());
    if(pView->verticalHeader()->minimumSectionSize() < cell.height())
        pView->verticalHeader()->setMinimumSectionSize(cell.height());
    
    painter->save();
    painter->setBrush(option.backgroundBrush);
    painter->translate(option.rect.topLeft());
    cell.resize(option.rect.size());
    cell.render(painter);
    painter->restore();//*/
    
    //获取项风格设置    
    /*
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    QApplication::style()->drawItemText ( painter, myOption.rect ,
                                          myOption.displayAlignment,
                                          QApplication::palette(), true,
                                          index.data(CLunarCalendarModel::SolarRole).toString() );
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;
    QApplication::style()->drawItemText ( painter, myOption.rect,
                                          myOption.displayAlignment,
                                          QApplication::palette(), true,
                                          index.data(CLunarCalendarModel::LunarRole).toString() );//*/
}

QSize CLunarCalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CFrmCell cell;
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  index.data(CLunarCalendarModel::LunarRole).toString());
    return cell.size();
}
