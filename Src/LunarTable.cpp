#include "LunarTable.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include "lunar.h"

static const QString g_Signature = "Lunar table. Author: Kang Lin(kl222@126.com)";
CThreadGenerate::CThreadGenerate(CLunarTable *pTable,
                                 const QDate& min,
                                 const QDate& max,
                                 bool bCached,
                                 QObject *parent)
    : QThread(parent),
      m_pTable(pTable),
      m_minDate(min),
      m_maxDate(max),
      m_bCached(bCached)
{
}

void CThreadGenerate::run()
{
    if(!m_pTable) return;
    m_pTable->Generate(m_minDate, m_maxDate, m_bCached);
}

CLunarTable::CLunarTable(QObject *parent) : QObject(parent)
{
    m_bSaveAllDate = true;
    m_nThreadNumber = 0;
    Load(":/data/LunarCalendarTable");
}

CLunarTable* CLunarTable::Instance()
{
    static CLunarTable* p = nullptr;
    if(nullptr == p)
        p = new CLunarTable();
    return p;
}

#ifndef QT_NO_DATASTREAM
QDataStream& operator<<(QDataStream& output, const CLunarTable::_LUNAR_DAY& data)
{
    output << data.nTg
           << data.nDz
           << data.bLeap
           << data.nMonth
           << data.nDay
           << data.nJq;
    return output;
}

QDataStream& operator>>(QDataStream& input, CLunarTable::_LUNAR_DAY& data)
{
    input >> data.nTg
          >> data.nDz
          >> data.bLeap
          >> data.nMonth
          >> data.nDay
          >> data.nJq;
    return input;
}

QDataStream& operator<<(QDataStream& output, const GZ& data)
{
    output << data.dz
           << data.tg;
    return output;
}

QDataStream& operator>>(QDataStream& input, GZ& data)
{
    input >> data.dz
          >> data.tg;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Day& data)
{
    output << data. d0   //儒略日
           << data.di //公历月内日序数
           << data.y   //公历年
           << data.m //公历月
           << data.d 
           << data.dn   //所在公历月的天数
           << data.week0 //月首的星期
           << data.week  //当前日的星期数
           << data.weeki //本日所在的周序号
           << data.weekN //本月的总周数
           << data.Ldi  //距农历月首的编移量,0对应初一
           << data.cur_dz  //距冬至的天数
           << data.cur_xz //距夏至的天数
           << data.cur_lq//距立秋的天数
           << data.cur_mz   //距芒种的天数
           << data.cur_xs  //距小暑的天数
              
              //std::vector<double> cur_jq  //节气时间索引 （这里可以使用JD::JD2DD转换成准确时间）
              //std::vector<int> cur_cn  //节气名称索引 
              
              
           << data.Lmc  //阴历月的月份
           << data.Ldn  //阴历月的天数
           << data.Lleap //是不是阴历的润月
           << data.Lmc2 //下个月的月份，不存在则为-1
              
           << data.qk  //节气，不存在则为-1
              
           << data.Lyear  //农历纪年(10进制,1984年起算)
           << data.Lyear0  //农历纪年(10进制,1984年起算)
           << data.Lyear2 //干支纪年(立春)
           << data.Lyear3 //干支纪年(正月)
           << data.Lyear4
              
           << data.Lmonth
           << data.Lmonth2   //月天干地支
              
           << data.Lday2
              
           << data.XiZ //星座
           << (int)data.jqmc //节气类型
           << (double)data.jqjd
           << QString(data.jqsj.c_str())  //节气时间
              
           << data.yxmc //月像类型
           << data.yxjd
           << QString(data.yxsj.c_str()) //月像时间
              ;
    return output;
}

QDataStream& operator>>(QDataStream& input, Day& data)
{
    input >>  data. d0   //儒略日
            >> data.di //公历月内日序数
            >> data.y   //公历年
            >> data.m //公历月
            >> data.d 
            >> data.dn   //所在公历月的天数
            >> data.week0 //月首的星期
            >> data.week  //当前日的星期数
            >> data.weeki //本日所在的周序号
            >> data.weekN //本月的总周数
            >> data.Ldi  //距农历月首的编移量,0对应初一
            >> data.cur_dz  //距冬至的天数
            >> data.cur_xz //距夏至的天数
            >> data.cur_lq//距立秋的天数
            >> data.cur_mz   //距芒种的天数
            >> data.cur_xs  //距小暑的天数
            
            //std::vector<double> cur_jq  //节气时间索引 （这里可以使用JD::JD2DD转换成准确时间）
            //std::vector<int> cur_cn  //节气名称索引 
            
            
            >> data.Lmc  //阴历月的月份
            >> data.Ldn  //阴历月的天数
            >> data.Lleap //是不是阴历的润月
            >> data.Lmc2 //下个月的月份，不存在则为-1
            
            >> data.qk  //节气，不存在则为-1
            
            >> data.Lyear  //农历纪年(10进制,1984年起算)
            >> data.Lyear0  //农历纪年(10进制,1984年起算)
            >> data.Lyear2 //干支纪年(立春)
            >> data.Lyear3 //干支纪年(正月)
            >> data.Lyear4
            
            >> data.Lmonth
            >> data.Lmonth2   //月天干地支
            
            >> data.Lday2
            
            >> data.XiZ //星座
            ;
    
    int jqmc;
    input >>  jqmc;
    data.jqmc = jqmc; //节气类型
    
    double jqjd;
    input >> jqjd;
    data.jqjd = jqjd;
    
    QString jqsj;
    input >> jqsj;
    data.jqsj = jqsj.toStdString().c_str();  //节气时间
    
    input >> data.yxmc //月像类型
            >> data.yxjd;
    
    QString yxsj;
    input >> yxsj;
    data.yxsj = yxsj.toStdString().c_str();  //月像时间
    
    return input;
}
#endif

int CLunarTable::Save(const QString &file, bool bAll)
{
    int nRet = 0;
    //TODO: modify the nVersion when modify data format
    uint8_t nVersion = 0;

    QFile f(file);
    if(!f.open(QIODevice::WriteOnly))
    {
        qCritical() << "Open file fail: " << file;
        return -1;
    }
    
    if(!bAll)
        CleanOutsideRange(m_minDate, m_maxDate);
    
    QDataStream out(&f);
    out << g_Signature;
    out << nVersion;
    out << static_cast<qint64>(m_Lunar.size());
    QMap<qint64, _LUNAR_DAY>::iterator it;
    for(it = m_Lunar.begin(); it != m_Lunar.end(); it++)
    {
        out << it.key();
        out << it.value();
    }

    f.close();
    return nRet;
}

int CLunarTable::Load(const QString &file)
{
    int nRet = 0;
    uint8_t nVersion = 0;
    QString signature;
    QFile f(file);
    if(!f.open(QIODevice::ReadOnly))
    {
        qCritical() << "Open file fail: " << file;
        return -1;
    }
    QDataStream in(&f);
    in >> signature;
    if(g_Signature != signature)
    {
        qCritical() << "Luanar table Signature error: " << signature;
        f.close();
        return -2;
    }
    in >> nVersion;
    //TODO: check version
    switch (nVersion) {
    case 0:
        nRet = LoadVersion0(in);
        break;
    default:
        qCritical() << "Luanar table version[" << nVersion << "] don't support";
        break;
    }

    f.close();
    return nRet;
}

int CLunarTable::LoadVersion0(QDataStream &in)
{
    qint64 nNum = 0;
    in >> nNum;
    m_Lunar.clear();
    while(nNum--)
    {
        qint64 jd;
        in >> jd;
        _LUNAR_DAY lunar;
        in >> lunar;
        m_Lunar[jd] = lunar;
    }
    return 0;
}

int CLunarTable::Generate(const QDate &min, const QDate &max, const QString &szFile, int nThread, bool bClearCache, bool bSaveAllDate)
{
    int nRet = 0;
    if(bClearCache)
        m_Lunar.clear();
    m_nThreadNumber = nThread;
    m_szFile = szFile;
    m_minDate = min;
    m_maxDate = max;
    m_bSaveAllDate = bSaveAllDate;
    CThreadGenerate* pGenerate = nullptr;
    qint64 days = min.daysTo(max);
    qint64 num = days / nThread;
    QDate d = min;
    for(int i = 1; i <= nThread; i++)
    {
        QDate maxDate;
        if(i == nThread)
            maxDate = max;
        else
            maxDate = d.addDays(num);
        pGenerate = new CThreadGenerate(this, d, maxDate, !bClearCache, this);
        d = maxDate.addDays(1);
        if(pGenerate)
        {
            bool check = false;
            check = connect(pGenerate, SIGNAL(finished()),
                                 this, SLOT(slotGenerateFinished()));
            Q_ASSERT(check);
            check = connect(pGenerate, SIGNAL(finished()),
                            pGenerate, SLOT(deleteLater()));
            Q_ASSERT(check);
            pGenerate->start();
        }
        
    }

    return nRet;
}

void CLunarTable::slotGenerateFinished()
{
    if(--m_nThreadNumber)
        return;
    if(m_szFile.isEmpty())
        return;
    Save(m_szFile, m_bSaveAllDate);
}

int CLunarTable::Generate(const QDate &min, const QDate &max, bool bUseCached)
{
    int nRet = 0;
    if(!min.isValid() || !max.isValid())
        return -1;
    
    QDate date = min;
    while(date.isValid() && date <= max)
    {
        if(bUseCached)
        {
            QMap<qint64, _LUNAR_DAY>::const_iterator it;
            it = m_Lunar.find(date.toJulianDay());
            if(it != m_Lunar.end())
            {
                date = date.addDays(1);
                continue;
            }
        }
        
        Lunar l;
        Day day = l.getDayBySolar(date.year(), date.month(), date.day());
        
        _LUNAR_DAY lunar;
        
        //TODO:同步修改CLunarTable::GetLunar中的
        //以立春为农历新年第一天
//        lunar.nTg = day.Lyear2.tg;
//        lunar.nDz = day.Lyear2.dz;
        //以春节为农历新年第一天
        lunar.nTg = day.Lyear3.tg;
        lunar.nDz = day.Lyear3.dz;
        
        lunar.bLeap = day.Lleap;
        lunar.nMonth = day.Lmc;
        lunar.nDay = day.Ldi;
        lunar.nJq = day.qk;
        
        m_GenerateMutex.lock();
        m_Lunar[date.toJulianDay()] = lunar;
        m_GenerateMutex.unlock();
        
        date = date.addDays(1);
    }
    
    return nRet;
}

int CLunarTable::GetLunar(const QDate &date, _LUNAR_DAY &lunar)
{
    int nRet = -1;
    if(!date.isValid())
        return nRet;

    QMap<qint64, _LUNAR_DAY>::const_iterator it;
    it = m_Lunar.find(date.toJulianDay());
    if(m_Lunar.end() != it)
    {
        lunar = *it;
        return 0;
    }
    
    Lunar l;
    Day day = l.getDayBySolar(date.year(), date.month(), date.day());

    //TODO:同步修改 CLunarTable::Generate 中
    //以立春为农历新年第一天
//        lunar.nTg = day.Lyear2.tg;
//        lunar.nDz = day.Lyear2.dz;
    //以春节为农历新年第一天
    lunar.nTg = day.Lyear3.tg;
    lunar.nDz = day.Lyear3.dz;
    
    lunar.bLeap = day.Lleap;
    lunar.nMonth = day.Lmc;
    lunar.nDay = day.Ldi;
    lunar.nJq = day.qk;
    m_Lunar[date.toJulianDay()] = lunar;
    
    return 0;
}

int CLunarTable::CleanOutsideRange(const QDate &min, const QDate &max)
{
    if(!min.isValid() || !max.isValid())
        return -1;
    QMap<qint64, _LUNAR_DAY>::iterator it;
    for(it = m_Lunar.begin(); it != m_Lunar.end();)
    {
        qint64 key = it.key();
        it++;
        if(key < min.toJulianDay() || key > max.toJulianDay())
        {
            m_Lunar.remove(key);
        }
    }
    return 0;
}
