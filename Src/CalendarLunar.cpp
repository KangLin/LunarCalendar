#include "CalendarLunar.h"
#include "lunar.h"
#include <QDate>
#include <QDebug>
#include <QTime>

static const QString g_Gan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
static const QString g_Zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
static const QString g_ShX[] = {"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"};
static const QString g_numCn[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //中文数字
static const QString g_jqmc[] = {"冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"};
static const QString g_ymc[] = {"十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //月名称,建寅
static const QString g_rmc[] = {"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"};

static QMap<int, QMap<int, QString> > g_Holiday;
static QMap<int, QMap<int, QString> > g_Anniversary;

CCalendarLunar::CCalendarLunar(QObject *parent) : QObject(parent)
{
    InitHoliday();
}

CCalendarLunar::CCalendarLunar(QDate date, QObject *parent) : QObject(parent)
{
    InitHoliday();
    
    if(date.isValid())
    {
        GetLunar(date);        
    }
}

int CCalendarLunar::GetLunar(const QDate &date)
{
    if(!date.isValid())
        return -1;
    
    Lunar l;
    
    //QTime tStart = QTime::currentTime();
    Day day = l.getDayBySolar(date.year(), date.month(), date.day());
    //qDebug() << "getDayBySolar time:" << tStart.msecsTo(QTime::currentTime());
    
    m_szLunar = g_Gan[day.Lyear2.tg] + g_Zhi[day.Lyear2.dz] + "年";
    if (day.Lleap)
    {
        m_szLunar += "润" + g_ymc[day.Lmc] + "月" + g_rmc[day.Ldi] + "日";
    }
    else
    {
        m_szLunar += g_ymc[day.Lmc] + "月" + g_rmc[day.Ldi] + "日";
    }
    
    QMap<int, QString> holiday = g_Holiday[day.Lmc];
    if(!holiday.isEmpty())
        m_szHoliday = holiday[day.Ldi];
    
    holiday = g_Anniversary[day.Lmc];
    if(!holiday.isEmpty())
        m_szAnniversary = holiday[day.Ldi];
    
    if(-1 != day.qk)
        m_szJieQi = g_jqmc[day.qk];
    
    if(0 == day.Ldi)
    {
        if (day.Lleap)
            m_szLunarDay += "润" + g_ymc[day.Lmc] + "月";
        else
            m_szLunarDay = g_ymc[day.Lmc] + "月";
    } else
        m_szLunarDay = g_rmc[day.Ldi];
    
    return 0;
}

QString CCalendarLunar::GetLunar()
{
    return m_szLunar;
}

QString CCalendarLunar::GetLunarDay()
{
    return m_szLunarDay;
}

QString CCalendarLunar::GetHoliday()
{
    return m_szHoliday;
}

QString CCalendarLunar::GetJieQi()
{
    return m_szJieQi;
}

/*
节日名称 节日时间
春节 正月初一
元宵节（上元节）正月十五
龙抬头 二月初二
社日节（土地诞） 二月初二
上巳节 三月初三
寒食节 清明节前一天
清明节 阳历4月5日前后
端午节 五月初五
七夕节 七月初七
七月半（中元节） 七月十四/十五
中秋节 八月十五
重阳节 九月初九
寒衣节 十月初一
下元节 十月十五
冬至节 阳历12月22日前后
腊八节 腊月初八
祭灶节（小年） 腊月廿三或廿四
岁除（除夕）腊月廿九或三十
*/

int CCalendarLunar::InitHoliday()
{
    if(!g_Holiday.isEmpty())
        return 0;
    
    AddHoliday(1, 1, "春节");
    AddHoliday(1, 15, "元宵");
    AddHoliday(2, 2, "社日");
    AddHoliday(3, 3, "上巳");
    AddHoliday(5, 5, "端午");
    AddHoliday(7, 7, "七夕");
    AddHoliday(8, 15, "中秋");
    AddHoliday(9, 9, "重阳");
    AddHoliday(10, 1, "寒衣");
    AddHoliday(10, 15, "下元");
    AddHoliday(12, 8, "腊八");
    AddHoliday(12, 24, "小年");
    return 0;
}

int CCalendarLunar::AddHoliday(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    int m = month + 1;
    if(m > 11)
        m = m % 12;

    g_Holiday[m].insertMulti(day - 1, szName);
    return 0;
}

int CCalendarLunar::AddAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    int m = month + 1;
    if(m > 11)
        m = m % 12;

    g_Anniversary[m].insertMulti(day - 1, szName);
    return 0;
}

QString CCalendarLunar::GetAnniversary()
{
    return m_szAnniversary;
}

int CCalendarLunar::Save(const QString &file)
{
    int nRet = 0;
    
    return nRet;
}

int CCalendarLunar::Load(const QString &file)
{
    int nRet = 0;
    
    return nRet;
}

int CCalendarLunar::Generate(const QDate &min, const QDate &max)
{
    int nRet = 0;
    
    return nRet;
}
