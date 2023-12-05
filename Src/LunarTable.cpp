/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#include "LunarTable.h"
#include <QDataStream>
#include <QFile>
#include <QLoggingCategory>
#include <QSharedPointer>
#include "sxtwl.h"

static Q_LOGGING_CATEGORY(Logger, "Rabbit.LunarCalendar.table")

static const QString g_Signature = "Lunar cache table. Author: Kang Lin(kl222@126.com)";
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
{}

void CThreadGenerate::run()
{
    qDebug(Logger) << "CThreadGenerate::run()";
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
QDataStream& operator << (QDataStream& output, const CLunarTable::_LUNAR_DAY& data)
{
    output << data.nTg
           << data.nDz
           << data.bLeap
           << data.nYear
           << data.nMonth
           << data.nDay
           << data.nJq;
    return output;
}

QDataStream& operator >> (QDataStream& input, CLunarTable::_LUNAR_DAY& data)
{
    input >> data.nTg
          >> data.nDz
          >> data.bLeap
          >> data.nYear
          >> data.nMonth
          >> data.nDay
          >> data.nJq;
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
        qCritical(Logger) << "Open file fail: " << file;
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
    qDebug(Logger) << "Thread finished: " << m_nThreadNumber;
    if(--m_nThreadNumber)
        return;
    if(m_szFile.isEmpty())
    {
        qCritical(Logger()) << "Cache file is empty";
        return;
    }
    int nRet = Save(m_szFile, m_bSaveAllDate);
    if(!nRet)
        qInfo(Logger) << "Generate finished: save to:" << m_szFile;
    else
        qCritical(Logger) << "Generate finished: save fail" << nRet << ";" << m_szFile ;
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
            auto it = m_Lunar.find(date.toJulianDay());
            if(it != m_Lunar.end())
            {
                date = date.addDays(1);
                continue;
            }
        }

        _LUNAR_DAY lunar;
        nRet = GetLunar(date, lunar, false);
        if(nRet) break;

        m_GenerateMutex.lock();
        m_Lunar[date.toJulianDay()] = lunar;
        m_GenerateMutex.unlock();

        date = date.addDays(1);
    }

    return nRet;
}

int CLunarTable::GetLunar(const QDate &date, _LUNAR_DAY &lunar, bool cache)
{
    int nRet = -1;
    if(!date.isValid())
        return nRet;

    if(cache) {
        auto it = m_Lunar.find(date.toJulianDay());
        if(m_Lunar.end() != it)
        {
            lunar = *it;
            return 0;
        }
    }

    QSharedPointer<Day> pDay(sxtwl::fromSolar(date.year(), date.month(), date.day()));
    Day& day = *pDay;

    //TODO:同步修改 CLunarTable::Generate 中
    //以立春为农历新年第一天
//        lunar.nTg = day.Lyear2.tg;
//        lunar.nDz = day.Lyear2.dz;
    //以春节为农历新年第一天
    lunar.nTg = day.getYearGZ().tg;
    lunar.nDz = day.getYearGZ().dz;

    lunar.bLeap = day.isLunarLeap();
    lunar.nYear = day.getLunarYear();
    lunar.nMonth = day.getLunarMonth();
    if(lunar.nMonth > 12 || lunar.nMonth < 1)
        qCritical(Logger) << "Lunar month error, data:" << date;
    lunar.nDay = day.getLunarDay();
    if(lunar.nDay > 31 || lunar.nDay < 1)
        qCritical(Logger) << "Lunar day error, date:" << date;

    if(day.hasJieQi())
        lunar.nJq = day.getJieQi();
    else
        lunar.nJq = -1;

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
