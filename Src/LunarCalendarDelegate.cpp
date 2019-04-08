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
#include <QFont>
#include <QFontInfo>
#include <QFontMetrics>

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}

void CLunarCalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTableView *pView = dynamic_cast<QTableView*>(this->parent());
    int firstHeight = 10;
    QPalette palette = QApplication::style()->standardPalette();
    QPalette paletteLunar = palette;
    QColor solarColor, lunarColor, anniversaryColor;
        
    if(pView->currentIndex() == index)
    palette.setBrush(QPalette::Background,
                QBrush(QColor(index.data(Qt::BackgroundRole).value<QColor>())));
    solarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
    if(pView->currentIndex() == index)
        solarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
    else
        solarColor = QColor(index.data(Qt::ForegroundRole).value<QColor>());
    palette.setBrush(QPalette::Foreground, QBrush(solarColor));
    paletteLunar.setBrush(QPalette::Background,
                QBrush(QColor(index.data(Qt::BackgroundRole).value<QColor>())));
    if(pView->currentIndex() == index)
        lunarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
    else
        lunarColor = QColor(index.data(
                          CLunarCalendarModel::LunarColorRole).value<QColor>());
    paletteLunar.setBrush(QPalette::Foreground, QBrush(lunarColor));
    
    if(pView->currentIndex() == index)
        anniversaryColor = palette.color(QPalette::Active, QPalette::HighlightedText);
    else
        anniversaryColor = QColor(index.data(Qt::ForegroundRole).value<QColor>());
    
    QFont fontSolar, fontLunar;
    fontSolar = index.data(Qt::FontRole).value<QFont>();
    fontLunar = index.data(CLunarCalendarModel::LunarFontRole).value<QFont>();
    
    /*
    CFrmCell cell;
    cell.SetValue(index.data(CLunarCalendarModel::SolarRole).toString(),
                  palette,
                  fontSolar,
                  index.data(CLunarCalendarModel::LunarRole).toString(),
                  paletteLunar,
                  fontLunar);
    
    if(pView->horizontalHeader()->minimumSectionSize() < cell.width())
        pView->horizontalHeader()->setMinimumSectionSize(cell.width());
    if(pView->verticalHeader()->minimumSectionSize() < cell.height())
        pView->verticalHeader()->setMinimumSectionSize(cell.height());       
    
    if(pView->currentIndex() == index)
    {
        QPalette palette;
        painter->setBrush(palette.brush(QPalette::Active, QPalette::Highlight));
        painter->drawRect(option.rect);
    }
    painter->save();   
    painter->translate(option.rect.topLeft());
    painter->setBrush(option.backgroundBrush);
    cell.resize(option.rect.size());
    cell.render(painter);
    painter->restore();

    //*/
    
    //*
    QString szSolar = index.data(CLunarCalendarModel::SolarRole).toString();
    QString szLunar = index.data(CLunarCalendarModel::LunarRole).toString();
    QString szAnniversary = index.data(CLunarCalendarModel::Anniversary).toString();
    
    painter->save();
    painter->setFont(fontSolar);
    QFontMetrics m = painter->fontMetrics();
    int solarHeight = m.height();
    int solarWidth = m.width(szSolar);

    painter->setFont(fontLunar);
    m = painter->fontMetrics();
    int lunarHeight = m.height();
    int lunarWidth = m.width(szLunar);
    
    int width = lunarWidth;
    int height = lunarHeight + solarHeight + firstHeight;
    if(width < solarWidth)
        width = solarWidth;
    
    if(pView->horizontalHeader()->minimumSectionSize() < width)
        pView->horizontalHeader()->setMinimumSectionSize(width);
    if(pView->verticalHeader()->minimumSectionSize() < height)
        pView->verticalHeader()->setMinimumSectionSize(height);

    if(pView->currentIndex() == index)
    {
        QPalette palette = QApplication::style()->standardPalette();
        painter->setBrush(palette.brush(QPalette::Active, QPalette::Highlight));
        painter->drawRect(option.rect);
    }
    
    if(!szAnniversary.isEmpty())
    {
        painter->setPen(anniversaryColor);
        painter->drawArc(option.rect.left() + option.rect.width() / 2 - 3,
                         option.rect.top() + option.rect.height() / 6 - 3,
                         6, 6, 0, 5760);
    }
    
    painter->setFont(fontSolar);
    painter->setBackground(palette.background());
    painter->setBrush(palette.foreground());
    painter->setPen(solarColor);
    QPoint p;
    p.setX(option.rect.left() + (option.rect.width() - solarWidth) / 2);
    p.setY(option.rect.top() + (option.rect.height() - solarHeight) / 2);
    painter->drawText(p, szSolar);
  
    painter->setFont(fontLunar);
    painter->setPen(lunarColor);
    p.setX(option.rect.left() + (option.rect.width() - lunarWidth) / 2);
    p.setY(option.rect.top() + option.rect.height() * 2 / 3
           + (option.rect.height() / 3 - lunarHeight) / 2);
    painter->drawText(p, szLunar);

    painter->restore();
    
    //获取项风格设置    
    /*
    QStyleOptionViewItem myOption = option;
    myOption.font = fontSolar;
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
    QApplication::style()->drawItemText ( painter, myOption.rect ,
                                          myOption.displayAlignment,
                                          palette, false,
                                          index.data(CLunarCalendarModel::SolarRole).toString() );
    myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;
    QApplication::style()->drawItemText ( painter, myOption.rect,
                                          myOption.displayAlignment,
                                          paletteLunar, false,
                                          index.data(CLunarCalendarModel::LunarRole).toString() );
    QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, &myOption, painter);
    //*/
}

QSize CLunarCalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CFrmCell cell;
    return cell.size();
}
