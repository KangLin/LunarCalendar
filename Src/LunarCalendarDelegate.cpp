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

QColor GetColorRole(const QPalette &palette, int role)
{
    switch (role) {
    case CLunarCalendarModel::ColorNormal:
        return palette.color(QPalette::Active, QPalette::Text);
    case CLunarCalendarModel::ColorDisable:
        return palette.color(QPalette::Disabled, QPalette::Text);
    case CLunarCalendarModel::ColorHighlight:
        return palette.color(QPalette::Active, QPalette::BrightText);
    }
    return QColor();
}

QFont GetFontRole(QFont &font, int role)
{
    switch (role) {
    case CLunarCalendarModel::FontBold:
        font.setBold(true);
        break;
    case CLunarCalendarModel::FontNormal:
        font.setBold(false);
        break;
    }
    return font;
}

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{
}

void CLunarCalendarDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    QPalette palette = option.palette; // QApplication::style()->standardPalette();
    QPalette paletteLunar = palette;
    QColor solarColor, lunarColor, anniversaryColor;
    
    QFont fontSolar = option.font, fontLunar = fontSolar;
    fontSolar = GetFontRole(fontSolar,
                      index.data(CLunarCalendarModel::SolarFontRole).toInt());
    fontLunar = GetFontRole(fontLunar,
                      index.data(CLunarCalendarModel::LunarFontRole).toInt());

    QString szSolar = index.data(CLunarCalendarModel::SolarRole).toString();
    QString szLunar = index.data(CLunarCalendarModel::LunarRole).toString();
    QString szAnniversary
            = index.data(CLunarCalendarModel::Anniversary).toString();
    
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
        solarColor = GetColorRole(palette,
                       index.data(CLunarCalendarModel::SolarColorRole).toInt());
        lunarColor = GetColorRole(palette,
                       index.data(CLunarCalendarModel::LunarColorRole).toInt());
    }
    anniversaryColor = solarColor;
  
    if(!szAnniversary.isEmpty())
    {
        painter->setBrush(anniversaryColor);
        painter->setPen(anniversaryColor);
        painter->drawEllipse(option.rect.left()
                             + (option.rect.width() >> 1) - 2,
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

CLunarCalendarHeaderDelegate::CLunarCalendarHeaderDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{
}

void CLunarCalendarHeaderDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QPalette palette = option.palette;
    QPalette paletteLunar = palette;
    QColor color = GetColorRole(palette,
                      index.data(CLunarCalendarModel::SolarColorRole).toInt());
    QString szText = index.data(Qt::DisplayRole).toString();
    
    painter->save();
    painter->setFont(option.font);
    QFontMetrics m = painter->fontMetrics();
    QPoint pos;
    pos.setX(option.rect.left()
             + ((option.rect.width() - m.width(szText)) >> 1));
    pos.setY(option.rect.top() + ((option.rect.height() - m.height()) >> 1));
    painter->drawText(pos, szText);
    painter->restore();
}
