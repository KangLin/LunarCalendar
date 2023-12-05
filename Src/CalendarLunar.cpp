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

static QMap<int, QMap<int, QStringList> > g_Holiday;     //假日
static QMap<int, QMap<int, QStringList> > g_Anniversary; //周年

CCalendarLunar::CCalendarLunar(QObject *parent) : QObject(parent)
{
    InitHoliday();
}

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
    
    // 除夕
    if(12 == m_Month && 29 <= m_Day) {
        CLunarTable::_LUNAR_DAY chuxi;
        nRet = CLunarTable::Instance()->GetLunar(date.addDays(1), chuxi);
        if(nRet)
            return -2;
        if(1 == chuxi.nMonth)
            m_Holiday << "除夕";
    }

    //TODO: 闰月节日、周年纪念日依旧提示
    auto holiday = g_Holiday[day.nMonth];
    if(!holiday.isEmpty())
        m_Holiday << holiday.value(day.nDay);

    holiday = g_Anniversary[day.nMonth];
    if(!holiday.isEmpty())
        m_Anniversary << holiday.value(day.nDay);

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

QStringList CCalendarLunar::GetHoliday()
{
    return m_Holiday;
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

/*
https://baike.baidu.com/item/%E4%B8%AD%E5%9B%BD%E4%BC%A0%E7%BB%9F%E8%8A%82%E6%97%A5/396100?fromModule=disambiguation
https://baike.baidu.com/item/中国传统节日

节日名称 节日时间
春节 正月初一
元宵节（上元节）正月十五
龙抬头 二月初二
社日节（土地诞） 二月初二
花朝 南方一般2月12日，北方2月15日。但是新化叫“花烛节”为2月15日。难道祖先是从北方迁移过来的。
上巳节 三月初三
寒食节 清明节前一天
清明节 阳历4月5日前后
端午节 五月初五
天贶节 六月初六，晒衣节， 晒伏节“六月六，晒红绿。” “姑姑节”“六月六，请姑姑”。在古代还是另外一个节日，名叫天贶（赐赠的意思）节，六月六也是佛寺的一个节日，叫做翻经节，祭祀山神。
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
    AddHoliday(1, 15, "上元");
    AddHoliday(1, 15, "元宵");
    AddHoliday(2, 2, "社日");
    AddHoliday(2, 2, "龙抬头");
    AddHoliday(2, 12, "花朝");
    AddHoliday(3, 3, "上巳");
    AddHoliday(5, 5, "端午");
    AddHoliday(6, 6, "天贶");
    AddHoliday(7, 7, "七夕");
    AddHoliday(7, 15, "中元");
    AddHoliday(8, 15, "中秋");
    AddHoliday(9, 9, "重阳");
    AddHoliday(10, 1, "寒衣");
    AddHoliday(10, 15, "下元");
    AddHoliday(12, 8, "腊八");
    AddHoliday(12, 24, "祭灶");
    AddHoliday(12, 24, "小年");

    return 0;
}

int CCalendarLunar::AddHoliday(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    g_Holiday[month][day] << szName;
    return 0;
}

int CCalendarLunar::AddAnniversary(int month, int day, const QString &szName)
{
    if(szName.isEmpty())
        return -1;
    g_Anniversary[month][day] << szName;
    return 0;
}

QStringList CCalendarLunar::GetAnniversary()
{
    return m_Anniversary;
}
