/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "LunarCalendarDelegate.h"
#include "LunarCalendarModel.h"
#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QHeaderView>
#include <QPalette>
#include <QLoggingCategory>
#include <QColor>
#include <QFont>
#include <QFontInfo>
#include <QFontMetrics>

static Q_LOGGING_CATEGORY(Logger, "Rabbit.LunarCalendar.Delegate")
    
QColor GetColorRole(const QPalette &palette, int role)
{
    switch (role) {
    case CLunarCalendarModel::_COLOR_ROLE::ColorNormal:
        return palette.color(QPalette::Active, QPalette::Text);
    case CLunarCalendarModel::_COLOR_ROLE::ColorDisable:
        return palette.color(QPalette::Disabled, QPalette::Text);
    case CLunarCalendarModel::_COLOR_ROLE::ColorHighlight:
        return palette.color(QPalette::Active, QPalette::Highlight);
    case CLunarCalendarModel::_COLOR_ROLE::ColorRed:
        return QColor(255, 0, 0);
    }
    return QColor();
}

QFont GetFontRole(QFont &font, int role)
{
    switch (role) {
    case CLunarCalendarModel::_FONT_ROLE::FontBold:
        font.setBold(true);
        break;
    case CLunarCalendarModel::_FONT_ROLE::FontNormal:
        font.setBold(false);
        break;
    }
    return font;
}

CLunarCalendarDelegate::CLunarCalendarDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{}

//! \see \ref LunarUI
void CLunarCalendarDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &o,
                                   const QModelIndex &index) const
{
    QStyleOptionViewItem option = o;
    //initStyleOption(&option, index);
    //qDebug(Logger) << "Option:" << option << o;
    //qDebug(Logger) << "Option.text:" << o.text;

    QPalette palette = option.palette; // QApplication::style()->standardPalette();
    QColor solarColor, lunarColor, tasksColor, taskNumberColor, workColor;

    QTableView *pView = dynamic_cast<QTableView*>(option.styleObject);
    if(!pView)
    {
        qCritical(Logger) << "The style object is null";
        return;
    }
    // CLunarCalendarModel* pModel = dynamic_cast<CLunarCalendarModel*>(pView->model());
    // if(!pModel)
    // {
    //     qCritical(Logger) << "The model is null";
    //     return;
    // }
    
    bool bSolar = index.data(CLunarCalendarModel::ROLE::CalendarTypeRole).toInt()
           & static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeSolar);
    bool bLunar = index.data(CLunarCalendarModel::ROLE::CalendarTypeRole).toInt()
           & static_cast<int>(CLunarCalendar::_CalendarType::CalendarTypeLunar);

    QFont fontSolar = option.font;
    QFont fontLunar = option.font;
    QFont fontTasks = option.font;
    QFont fontWork = option.font;

    if(-1 != fontWork.pointSize())
        fontWork.setPointSizeF(fontWork.pointSize() >> 1);
    if(-1 != fontWork.pixelSize())
        fontWork.setPixelSize(fontWork.pixelSize() >> 1);
    if(-1 != fontTasks.pointSize())
        fontTasks.setPointSizeF(fontTasks.pointSize() >> 1);
    if(-1 != fontTasks.pixelSize())
        fontTasks.setPixelSize(fontTasks.pixelSize() >> 1);

    QString szSolar, szLunar, szWork;
    uint nTasks = index.data(CLunarCalendarModel::ROLE::Tasks).toUInt();
    szWork = index.data(CLunarCalendarModel::ROLE::WorkDayRole).toString();

    int solarHeight = 0;
    int solarWidth = 0;
    int lunarHeight = 0;
    int lunarWidth = 0;
    int taskHeight = 0;
    int taskWidth = 0;
    int width = 0;
    int height = 0;
    int nRow = 0;
    int nHadRow = 0;

    painter->save();

    if(bSolar)
    {
        fontSolar = GetFontRole(fontSolar,
                        index.data(CLunarCalendarModel::ROLE::SolarFontRole).toInt());
        szSolar = index.data(CLunarCalendarModel::ROLE::SolarRole).toString();

        painter->setFont(fontSolar);
        QFontMetrics m = painter->fontMetrics();
        solarHeight = m.height();
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        solarWidth = m.horizontalAdvance(szSolar);
#else
        solarWidth = m.width(szSolar);
#endif
        width = qMax(width, solarWidth);
        height = qMax(height, solarHeight);
        nRow++;
    }

    if(bLunar)
    {
        fontLunar = GetFontRole(fontLunar,
                        index.data(CLunarCalendarModel::ROLE::LunarFontRole).toInt());
        szLunar = index.data(CLunarCalendarModel::ROLE::LunarRole).toString();
        if(szLunar.length() > 3)
        {
            szLunar = szLunar.left(2) + "...";
        }
        painter->setFont(fontLunar);
        QFontMetrics m = painter->fontMetrics();
        lunarHeight = m.height();
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        lunarWidth = m.horizontalAdvance(szLunar); 
#else
        lunarWidth = m.width(szLunar); 
#endif

        width = qMax(width, lunarWidth);
        height = qMax(height, lunarHeight);
        nRow++;
    }
    
    // 顶行（中国节假日，任务)
    if(bSolar || bLunar)
    {
        taskHeight = height;
        taskWidth = width;
        nRow++;
    }
    
    if(pView->horizontalHeader()->minimumSectionSize() < width)
    {
        //qDebug(Logger) << "Set pView->horizontalHeader()->minimumSectionSize() =" << width;
        pView->horizontalHeader()->setMinimumSectionSize(width);
        pView->updateGeometry();
        //qDebug(Logger) << "Set pView->horizontalHeader()->minimumSectionSize() ok";
    }
    /*
    qDebug(Logger) << "pView->horizontalHeader()->minimumSectionSize():"
                   << pView->horizontalHeader()->minimumSectionSize();//*/
    if(pView->verticalHeader()->minimumSectionSize() < nRow * height)
    {
        //qDebug(Logger) << "Set pView->verticalHeader()->minimumSectionSize() =" << nRow * height;
        pView->verticalHeader()->setMinimumSectionSize(nRow * height);
        pView->updateGeometry();
        //qDebug(Logger) << "Set pView->verticalHeader()->minimumSectionSize() ok";
    }
    /*
    qDebug(Logger) << "pView->verticalHeader()->minimumSectionSize():"
                   << pView->verticalHeader()->minimumSectionSize();//*/
    
    if(option.rect.width() > width)
        width = option.rect.width();
    if(option.rect.height() > height * nRow && nRow)
        height = option.rect.height() / nRow;
    
    // 是今日
    if(index.data(CLunarCalendarModel::ROLE::TodayRole).toBool())
    {
        painter->setPen(palette.brush(QPalette::Active, QPalette::Highlight).color());
        //painter->drawEllipse(option.rect);
        painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
    }

    // 是当前选择，反显字体
    if(pView->currentIndex() == index)
    {
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(palette.brush(QPalette::Active, QPalette::Highlight));
        painter->drawRect(option.rect);

        solarColor = palette.color(QPalette::Active, QPalette::HighlightedText);
        lunarColor = solarColor;
        tasksColor = solarColor;
        taskNumberColor = palette.color(QPalette::Active, QPalette::Highlight);
        workColor = solarColor;
    }
    else
    {
        if(bSolar)
            solarColor = GetColorRole(palette,
                 index.data(CLunarCalendarModel::ROLE::SolarColorRole).toInt());    
        if(bLunar)
            lunarColor = GetColorRole(palette,
                 index.data(CLunarCalendarModel::ROLE::LunarColorRole).toInt());
//        qDebug(Logger) << "Color:" << solarColor << index.data(CLunarCalendarModel::SolarColorRole).toInt()
//                 << lunarColor << index.data(CLunarCalendarModel::LunarColorRole).toInt();

        tasksColor = GetColorRole(palette,
                 index.data(CLunarCalendarModel::ROLE::TasksColorRole).toInt());
        if(CLunarCalendarModel::_COLOR_ROLE::ColorHighlight
            == index.data(CLunarCalendarModel::ROLE::TasksColorRole))
            taskNumberColor
                = palette.color(QPalette::Active, QPalette::HighlightedText);
        else
            taskNumberColor = GetColorRole(palette, CLunarCalendarModel::_COLOR_ROLE::ColorNormal);

        workColor = GetColorRole(palette,
               index.data(CLunarCalendarModel::ROLE::WorkDayColorRole).toInt());
    }

    if(!szWork.isEmpty() && bSolar)
    {
        painter->setFont(fontWork);
        painter->setPen(workColor);
        painter->drawText(option.rect.left(),
                          option.rect.top(),
                          width,
                          height,
                          Qt::AlignLeft| Qt::AlignTop,
                          szWork);
    }

    if(nTasks && (bSolar || bLunar))
    {
        painter->setFont(fontTasks);
        int h = qMin(taskHeight, taskWidth) >> 1;
        painter->setBrush(QBrush(tasksColor, Qt::SolidPattern));
        painter->setPen(tasksColor);

        QRect rect(option.rect.left() + (width - h) / 2,
                   option.rect.top() + (height - h) / 2,
                   h, h);
        //painter->drawArc(rect, 0, 5760);//*/

        painter->drawEllipse(rect);//*/
        
        /*
        if(nTasks > 0 && nTasks < 10)
        {
            painter->setPen(taskNumberColor);
            painter->drawText(option.rect.left(),
                          option.rect.top(),
                          width,
                          height,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          QString::number(nTasks));
        }//*/    
    }
    
    if(bSolar || bLunar)
        nHadRow++; // 第一行（中国节假日，任务)
    
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
{}

void CLunarCalendarHeaderDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QStyleOptionViewItem o = option;
    //initStyleOption(&o, index);
    qDebug(Logger) << "Option:" << option << o;
    qDebug(Logger) << "Option.text:" << o.text;
    QPalette palette = o.palette;
    QColor color = GetColorRole(palette,
                      index.data(CLunarCalendarModel::ROLE::SolarColorRole).toInt());
    QString szText = index.data(Qt::DisplayRole).toString();

    painter->save();
    painter->setFont(o.font);
    painter->setPen(color);

    QFontMetrics m = painter->fontMetrics();
    QPoint pos;
    pos.setX(option.rect.left()
             + ((option.rect.width() -
         #if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                 m.horizontalAdvance(szText)) >> 1
         #else
                 m.width(szText)) >> 1
         #endif
             ));
    pos.setY(option.rect.top() + ((option.rect.height() - m.height()) >> 1));
    painter->drawText(pos, szText);
    painter->restore();
}
