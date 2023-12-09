/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "CalendarLunar.h"
#include <QDate>
#include <QDebug>
#include <QTime>
#include "LunarTable.h"
#include "sxtwl.h"

// 天干地支: https://baike.baidu.com/item/%E5%A4%A9%E5%B9%B2%E5%9C%B0%E6%94%AF/278140
// 天干地支纪年法: https://zhuanlan.zhihu.com/p/616094910

static const QString g_Gan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
static const QString g_Zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
static const QString g_ShX[] = {"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"};
static const QString g_numCn[] = {"零", "一", "二", "三", "四", "五",
                                  "六", "七", "八", "九", "十"}; //中文数字
static const QString g_jqmc[] = {"冬至", "小寒", "大寒", "立春",
                                 "雨水", "惊蛰", "春分", "清明",
                                 "谷雨", "立夏", "小满", "芒种",
                                 "夏至", "小暑", "大暑", "立秋",
                                 "处暑", "白露", "秋分", "寒露",
                                 "霜降", "立冬", "小雪", "大雪"};
static const QString g_jqImage[] = {"DongZhi", "XiaoHan", "DaHan", "LiChun",
                                    "YuShui", "JingZhe", "ChunFen", "QingMing",
                                    "GuYu", "LiXia", "XiaoMan", "MangZhong",
                                    "XiaZhi", "XiaShu", "DaShu", "LiQiu",
                                    "ChuShu", "BaiLu", "Qiufeng", "HanLu",
                                    "ShungJiang", "LiDong", "XiaoXue", "DaXue"};
static const QString g_ymc[] = { "正", "二", "三", "四", "五", "六", "七", "八", "九", "十","十一", "十二"}; //月名称,建寅
static const QString g_rmc[] = {"初一", "初二", "初三", "初四", "初五", "初六",
                                "初七", "初八", "初九", "初十", "十一", "十二",
                                "十三", "十四", "十五", "十六", "十七", "十八",
                                "十九", "廿", "廿一", "廿二", "廿三", "廿四",
                                "廿五", "廿六", "廿七", "廿八", "廿九", "卅", "卅一"};
static const QString g_WeekCn[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};

CCalendarLunar::CCalendarLunar(QObject *parent) : QObject(parent)
{}

CCalendarLunar::CCalendarLunar(QDate date, QObject *parent) : CCalendarLunar(parent)
{    
    if(date.isValid())
    {
        GetLunar(date);
    }
}

int CCalendarLunar::GetYear()
{
    return m_Year;
}

int CCalendarLunar::GetMonth()
{
    return m_Month;
}

int CCalendarLunar::GetDay()
{
    return m_Day;
}

int CCalendarLunar::GetLunar(const QDate &date)
{
    if(!date.isValid())
        return -1;
    
    m_Solar = date;
    CLunarTable::_LUNAR_DAY day;
    int nRet = CLunarTable::Instance()->GetLunar(date, day);
    if(nRet)
        return -2;

    m_Year = day.nYear;
    m_Month = day.nMonth;
    m_Day = day.nDay;

    m_szLunar = g_Gan[day.nTg] + g_Zhi[day.nDz] + "年";
    if (day.bLeap)
        m_szLunar += "闰" + g_ymc[day.nMonth - 1] + "月" + g_rmc[day.nDay - 1] + "日";
    else
        m_szLunar += g_ymc[day.nMonth - 1] + "月" + g_rmc[day.nDay -1] + "日";

    if(1 == day.nDay)
    {
        if (day.bLeap)
            m_szLunarDay += "闰" + g_ymc[day.nMonth - 1] + "月";
        else
            m_szLunarDay = g_ymc[day.nMonth - 1] + "月";
    } else
        m_szLunarDay = g_rmc[day.nDay - 1];

    if(-1 != day.nJq)
    {
        m_szJieQi = g_jqmc[day.nJq];
        m_szImageJieQi = ":/image/" + g_jqImage[day.nJq];
    }

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

QString CCalendarLunar::GetJieQi()
{
    return m_szJieQi;
}

QString CCalendarLunar::GetJieQiImage()
{
    return m_szImageJieQi;
}

QDate CCalendarLunar::GetSolar(int lunarYear, int lunarMonth, int lunarDay, bool isLeap)
{
    QSharedPointer<Day> day(sxtwl::fromLunar(lunarYear, lunarMonth, lunarDay, isLeap));
    return QDate(day->getSolarYear(), day->getSolarMonth(), day->getSolarDay());
}
