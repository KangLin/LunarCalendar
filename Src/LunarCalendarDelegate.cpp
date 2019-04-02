#include "LunarCalendarDelegate.h"
#include "FrmCell.h"
#include "LunarCalendarModel.h"
#include <QPainter>

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}


void CLunarCalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CFrmCell cell;
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  index.data(CLunarCalendarModel::LunarRole).toString());
    
    painter->save();
    painter->setBrush(option.backgroundBrush);
    painter->translate(option.rect.topLeft());
    cell.resize(option.rect.size());
    cell.render(painter);
    painter->restore();
}

QSize CLunarCalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CFrmCell cell;
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  index.data(CLunarCalendarModel::LunarRole).toString());
    return cell.size();
}
