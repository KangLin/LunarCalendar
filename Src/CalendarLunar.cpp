#include "CalendarLunar.h"
#include "lunar.h"
#include <QDate>

static const QString Gan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
static const QString Zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
static const QString ShX[] = {"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"};
static const QString numCn[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //中文数字
static const QString jqmc[] = {"冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"};
static const QString ymc[] = {"十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //月名称,建寅
static const QString rmc[] = {"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"};

CCalendarLunar::CCalendarLunar(QObject *parent) : QObject(parent)
{   
}

QString CCalendarLunar::GetLunar(const QDate &date)
{
    QString szDate;
    Lunar l;
    Day day = l.getDayBySolar(date.year(), date.month(), date.day());
    szDate = Gan[day.Lyear2.tg] + Zhi[day.Lyear2.dz] + "年";
    if (day.Lleap)
	{
		szDate += "润" + ymc[day.Lmc] + "月" + rmc[day.Ldi] + "日";
	}
	else
	{
		szDate += ymc[day.Lmc] + "月" + rmc[day.Ldi] + "日";
	}

    return szDate;
}

QString CCalendarLunar::GetLunarDay(const QDate &date)
{
    QString szDay;
    QString szDate;
    Lunar l;
    Day day = l.getDayBySolar(date.year(), date.month(), date.day());
    if(0 == day.Ldi)
    {
        if (day.Lleap)
            szDay += "润" + ymc[day.Lmc] + "月";
        else 
            szDay = ymc[day.Lmc] + "月";
    } else
        szDay = rmc[day.Ldi];
    if(-1 != day.qk)
        szDay = jqmc[day.qk];

    return szDay;
}
