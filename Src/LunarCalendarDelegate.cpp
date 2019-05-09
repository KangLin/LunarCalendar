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
#include <algorithm> 

QColor GetColorRole(const QPalette &palette, int role)
{
    switch (role) {
    case CLunarCalendarModel::ColorNormal:
        return palette.color(QPalette::Active, QPalette::Text);
    case CLunarCalendarModel::ColorDisable:
        return palette.color(QPalette::Disabled, QPalette::Text);
    case CLunarCalendarModel::ColorHighlight:
        return palette.color(QPalette::Active, QPalette::Highlight);
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
    CLunarCalendarModel* pModel
            = dynamic_cast<CLunarCalendarModel*>(pView->model());
    
    bool bSolar = pModel->GetCalendarType() & CLunarCalendar::CalendarTypeSolar;
    bool bLunar = pModel->GetCalendarType() & CLunarCalendar::CalendarTypeLunar;
   
    QFont fontSolar = option.font;
    QFont fontLunar = fontSolar;
    
    QString szSolar, szLunar, szAnniversary;
    
    int solarHeight = 0;
    int solarWidth = 0;
    int firstHeight = 6;
    int lunarHeight = 0;
    int lunarWidth = 0;
    int width = 0;
    int height = 0;
    int nRow = 1;
    painter->save();
    
    if(bSolar)
    {
        fontSolar = GetFontRole(fontSolar,
                        index.data(CLunarCalendarModel::SolarFontRole).toInt());
        szSolar = index.data(CLunarCalendarModel::SolarRole).toString();
        
        painter->setFont(fontSolar);
        QFontMetrics m = painter->fontMetrics();
        solarHeight = m.height();
        solarWidth = m.width(szSolar);
        width = std::max(firstHeight, solarWidth);
        height = std::max(firstHeight, solarHeight);
        nRow++;
    }
    
    if(bLunar)
    {
        fontLunar = GetFontRole(fontLunar,
                        index.data(CLunarCalendarModel::LunarFontRole).toInt());
        szLunar = index.data(CLunarCalendarModel::LunarRole).toString();  
        
        painter->setFont(fontLunar);
        QFontMetrics m = painter->fontMetrics();
        lunarHeight = m.height();
        lunarWidth = m.width(szLunar); 
        
        width = std::max(width, lunarWidth);
        height = std::max(height, lunarHeight);
        nRow++;
    }
    
    szAnniversary = index.data(CLunarCalendarModel::Anniversary).toString();
    int nTasks = index.data(CLunarCalendarModel::Tasks).toInt();
    
    if(pView->horizontalHeader()->minimumSectionSize() < width)
    {
//        qDebug() << "pView->horizontalHeader()->minimumSectionSize():" << width;
        pView->horizontalHeader()->setMinimumSectionSize(width);
    }
    if(pView->verticalHeader()->minimumSectionSize() < nRow * height)
    {
        qDebug() << "pView->verticalHeader()->minimumSectionSize() :" << nRow * height;
        pView->verticalHeader()->setMinimumSectionSize(nRow * height);
    }

    if(option.rect.width() > width)
        width = option.rect.width();
    if(option.rect.height() > nRow * height)
        height = option.rect.height() / nRow;

    if(pView->currentIndex() == index)
    {
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(palette.brush(QPalette::Active, QPalette::Highlight));
        painter->drawRect(option.rect);

        solarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
        lunarColor = solarColor;
    }
    else
    {
        if(bSolar)
            solarColor = GetColorRole(palette,
                       index.data(CLunarCalendarModel::SolarColorRole).toInt());    
        if(bLunar)
            lunarColor = GetColorRole(palette,
                       index.data(CLunarCalendarModel::LunarColorRole).toInt());
    }
    anniversaryColor = solarColor;
  
    if(!szAnniversary.isEmpty() || nTasks)
    {
        painter->setBrush(anniversaryColor);
        painter->setPen(anniversaryColor);
        painter->drawEllipse(option.rect.left()
                             + (option.rect.width() >> 1) - 2,
                             option.rect.top() + (height >> 1) - 6,
                             4, 4);
    }
    
    if(bSolar)
    {
        painter->setFont(fontSolar);
        painter->setPen(solarColor);
        QPoint pos;
        pos.setX(option.rect.left() + ((option.rect.width() - solarWidth) >> 1));
        pos.setY(option.rect.top() + (height << 1) - ((height + solarHeight) >> 1));
        painter->drawText(pos, szSolar);    
    }
    
    if(bLunar)
    {
        painter->setFont(fontLunar);
        painter->setPen(lunarColor);
        QPoint pos;
        pos.setX(option.rect.left() + ((option.rect.width() - lunarWidth) >> 1));
        
        pos.setY(option.rect.top() + height * nRow - ((height + solarHeight) >> 1));
        painter->drawText(pos, szLunar);
    }

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
    painter->setPen(color);
    
    QFontMetrics m = painter->fontMetrics();
    QPoint pos;
    pos.setX(option.rect.left()
             + ((option.rect.width() - m.width(szText)) >> 1));
    pos.setY(option.rect.top() + ((option.rect.height() - m.height()) >> 1));
    painter->drawText(pos, szText);
    painter->restore();
}
