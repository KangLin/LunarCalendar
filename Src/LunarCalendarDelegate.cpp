#include "LunarCalendarDelegate.h"
#include "LunarCalendarModel.h"
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QPalette>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <QFontInfo>
#include <QFontMetrics>

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}

void CLunarCalendarDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    QPalette palette = QApplication::style()->standardPalette();
    QPalette paletteLunar = palette;
    QColor solarColor, lunarColor, anniversaryColor;
    QFont fontSolar, fontLunar;
    fontSolar = index.data(Qt::FontRole).value<QFont>();
    fontLunar = index.data(CLunarCalendarModel::LunarFontRole).value<QFont>();

    QString szSolar = index.data(CLunarCalendarModel::SolarRole).toString();
    QString szLunar = index.data(CLunarCalendarModel::LunarRole).toString();
    QString szAnniversary = index.data(CLunarCalendarModel::Anniversary).toString();
    
    painter->save();
    painter->setFont(fontSolar);
    QFontMetrics m = painter->fontMetrics();
    int solarHeight = m.height();
    int solarWidth = m.width(szSolar);
    int firstHeight = 6;
    painter->setFont(fontLunar);
    m = painter->fontMetrics();
    int lunarHeight = m.height();
    int lunarWidth = m.width(szLunar);
    
    int width = lunarWidth;
    int height = lunarHeight;
    if(width < solarWidth)
        width = solarWidth;
    if(height < solarHeight)
        height = solarHeight;
    if(height < firstHeight)
        height = firstHeight;
    if(width < firstHeight)
        width = firstHeight;

    if(pView->horizontalHeader()->minimumSectionSize() < width)
        pView->horizontalHeader()->setMinimumSectionSize(width);
    if(pView->verticalHeader()->minimumSectionSize() < 3 * height)
        pView->verticalHeader()->setMinimumSectionSize(3 * height);

    if(option.rect.width() > width)
        width = option.rect.width();
    if(option.rect.height() > 3 * height)
        height = option.rect.height() / 3;

    if(pView->currentIndex() == index)
    {
        painter->setBrush(palette.brush(QPalette::Active, QPalette::Highlight));
        painter->drawRect(option.rect);

        solarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
        lunarColor = solarColor;  
    }
    else
    {
        solarColor = QColor(index.data(Qt::ForegroundRole).value<QColor>());
        lunarColor = QColor(index.data(
                                CLunarCalendarModel::LunarColorRole).value<QColor>());
    }
    anniversaryColor = solarColor;
    
    if(!szAnniversary.isEmpty())
    {
        painter->setBrush(anniversaryColor);
        painter->setPen(anniversaryColor);
        painter->drawEllipse(option.rect.left() + (option.rect.width() >> 1) - 2,
                             option.rect.top() + (height >> 1) - 6,
                             4, 4);
    }
    
    painter->setFont(fontSolar);
    painter->setPen(solarColor);
    QPoint pos;
    pos.setX(option.rect.left() + ((option.rect.width() - solarWidth) >> 1));
    pos.setY(option.rect.top() + height + ((height - solarHeight) >> 1));
    painter->drawText(pos, szSolar);
  
    painter->setFont(fontLunar);
    painter->setPen(lunarColor);
    pos.setX(option.rect.left() + ((option.rect.width() - lunarWidth) >> 1));
    pos.setY(option.rect.top() + (height << 1) + ((height - solarHeight) >> 1));
    painter->drawText(pos, szLunar);

    painter->restore();
}
