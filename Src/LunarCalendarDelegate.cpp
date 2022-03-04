// 作者：康林 <kl222@126.com>

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
                                   const QStyleOptionViewItem &o,
                                   const QModelIndex &index) const
{
    QStyleOptionViewItem option = o;
    initStyleOption(&option, index);
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    QPalette palette = option.palette; // QApplication::style()->standardPalette();
    QPalette paletteLunar = palette;
    QColor solarColor, lunarColor, anniversaryColor, workColor;
    CLunarCalendarModel* pModel
            = dynamic_cast<CLunarCalendarModel*>(pView->model());
    
    bool bSolar = pModel->GetCalendarType() & CLunarCalendar::CalendarTypeSolar;
    bool bLunar = pModel->GetCalendarType() & CLunarCalendar::CalendarTypeLunar;
   
    QFont fontSolar = option.font;
    QFont fontLunar = fontSolar;
    QFont fontTasks = fontSolar;
    QFont fontWork = fontSolar;
    
    if(-1 == fontWork.pointSize())
        fontWork.setPixelSize(fontWork.pixelSize() / 2);
    else
        fontWork.setPointSizeF(fontWork.pointSizeF() / 2);
    if(-1 == fontTasks.pointSize())
        fontTasks.setPixelSize(fontTasks.pixelSize() / 3);
    else
        fontTasks.setPointSizeF(fontTasks.pointSizeF() / 3);

    QString szSolar, szLunar, szAnniversary, szWork;
    szAnniversary = index.data(CLunarCalendarModel::Anniversary).toString();
    int nTasks = index.data(CLunarCalendarModel::Tasks).toInt();
    szWork = index.data(CLunarCalendarModel::WorkDayRole).toString();

    int solarHeight = 0;
    int solarWidth = 0;
    int tasksHeight = 0;
    int tasksWidth = 0;
    int lunarHeight = 0;
    int lunarWidth = 0;
    int width = 0;
    int height = 0;
    int nRow = 0;
    int nHadRow = 0;
    
    painter->save();
        
    if(!szAnniversary.isEmpty() || nTasks)
    {
        painter->setFont(fontTasks);
        QFontMetrics m = painter->fontMetrics();
        tasksHeight = m.height();
        tasksWidth = m.width(QString::number(nTasks));
        width = tasksWidth;
        height = tasksHeight;
        nRow++;
    }
    
    if(bSolar)
    {
        fontSolar = GetFontRole(fontSolar,
                        index.data(CLunarCalendarModel::SolarFontRole).toInt());
        szSolar = index.data(CLunarCalendarModel::SolarRole).toString();
        
        painter->setFont(fontSolar);
        QFontMetrics m = painter->fontMetrics();
        solarHeight = m.height();
        solarWidth = m.width(szSolar);
        width = qMax(width, solarWidth);
        height = qMax(height, solarHeight);
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
        
        width = qMax(width, lunarWidth);
        height = qMax(height, lunarHeight);
        nRow++;
    }
        
    if(pView->horizontalHeader()->minimumSectionSize() < width)
    {
//        qDebug() << "pView->horizontalHeader()->minimumSectionSize():" << width;
        pView->horizontalHeader()->setMinimumSectionSize(width);
        pView->updateGeometry();
    }
    if(pView->verticalHeader()->minimumSectionSize() < nRow * height)
    {
//        qDebug() << "pView->verticalHeader()->minimumSectionSize() :" << nRow * height;
        pView->verticalHeader()->setMinimumSectionSize(nRow * height);
        pView->updateGeometry();
    }

    if(option.rect.width() > width)
        width = option.rect.width();
    if(option.rect.height() > height * nRow && nRow)
        height = option.rect.height() / nRow;

    if(index.data(CLunarCalendarModel::TodayRole).toBool())
    {
        painter->setPen(palette.brush(QPalette::Active, QPalette::Highlight).color());
        //painter->drawEllipse(option.rect);
        painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
    }
    
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
//        qDebug() << "Color:" << solarColor << index.data(CLunarCalendarModel::SolarColorRole).toInt()
//                 << lunarColor << index.data(CLunarCalendarModel::LunarColorRole).toInt();
    }
    anniversaryColor = solarColor;

    if(!szWork.isEmpty())
    {
        workColor =  GetColorRole(palette,
                     index.data(CLunarCalendarModel::WorkDayColorRole).toInt());   
        
        painter->setFont(fontWork);
        painter->setPen(workColor);
        painter->drawText(option.rect.left(),
                          option.rect.top(),
                          width,
                          height,
                          Qt::AlignLeft| Qt::AlignTop,
                          szWork);
    }
    
    if(!szAnniversary.isEmpty() || nTasks)
    {
        painter->setFont(fontTasks);
        int h = qMax(tasksWidth, tasksHeight);
        painter->setBrush(QBrush(anniversaryColor, Qt::SolidPattern));
        painter->setPen(anniversaryColor);
        
        QRect rect(option.rect.left() + (width - h) / 2,
                   option.rect.top() + (height - h) / 2,
                   h, h);
        /*painter->drawArc(rect, 0, 5760);//*/
        
        painter->drawEllipse(rect);//*/

        if(nTasks > 0 && nTasks < 10)
        {
            unsigned char r = ~(unsigned char)anniversaryColor.red();
            unsigned char g = ~(unsigned char)anniversaryColor.green();
            unsigned char b = ~(unsigned char)anniversaryColor.blue();
            QColor clr(r, g, b, anniversaryColor.alpha());            
            painter->setBrush(clr);
            painter->setPen(clr);
            painter->drawText(option.rect.left(),
                          option.rect.top(),
                          width,
                          height,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          QString::number(nTasks));//*/
        }
        
        nHadRow++;
    }

    if(bSolar)
    {
        painter->setFont(fontSolar);
        painter->setPen(solarColor);
        painter->drawText(option.rect.left(),
                          option.rect.top() + height * nHadRow,
                          width,
                          height,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          szSolar);
        nHadRow++;
    }
    
    if(bLunar)
    {
        painter->setFont(fontLunar);
        painter->setPen(lunarColor);
        painter->drawText(option.rect.left(),
                          option.rect.top() + height * nHadRow,
                          width,
                          height,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          szLunar);
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
