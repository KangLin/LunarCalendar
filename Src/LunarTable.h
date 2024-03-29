/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef LUNARTABLE_H
#define LUNARTABLE_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QFile>
#include <QMutex>

/*!
 * \brief 产生、保存、加载农历缓存。用于提高性能。
 * \see CLunarCalendar::LoadCalendarTable CLunarCalendar::GenerateCalendarTable
 * \ingroup INTERNAL_API
 */
class CLunarTable : public QObject
{
    Q_OBJECT
public:
    static CLunarTable* Instance();

    struct _LUNAR_DAY
    {
        quint8 nTg;
        quint8 nDz;
        bool bLeap; //是闰月
        int nYear;
        quint8 nMonth;
        quint8 nDay;
        qint8 nJq;
    };

    /*!
     * \brief 得到指定日期的农历
     * \param date 要查询的日期
     * \param lunar 相应的农历
     * \param cache 是否先从缓存查找
     *        - true: 先从缓存查找，如果没查到，则计算。
     *        - false: 不查缓存，直接计算。
     * \return 0，成功；其它，失败
     */
    int GetLunar(const QDate &date, _LUNAR_DAY &lunar, bool cache = true);

    int Load(const QString &file);
    int Save(const QString &file, bool bAll = true);

    int Generate(const QDate &min, const QDate &max, const QString& szFile, int nThread = 2, bool bClearCache = false, bool bSaveAllDate = true);
    int Generate(const QDate &min, const QDate &max, bool bUseCached);

public Q_SLOTS:
    void slotGenerateFinished();

private:
    explicit CLunarTable(QObject *parent = nullptr);
    int LoadVersion0(QDataStream &data);
    int CleanOutsideRange(const QDate&min, const QDate& max);

    QMap<qint64, _LUNAR_DAY> m_Lunar;
    QMutex m_GenerateMutex;
    QString m_szFile;
    QBasicAtomicInt m_nThreadNumber;
    QDate m_minDate, m_maxDate;
    bool m_bSaveAllDate;
};

#include <QThread>

class CThreadGenerate: public QThread
{
    Q_OBJECT
public:
    explicit CThreadGenerate(CLunarTable* pTable,
                             const QDate& min,
                             const QDate& max,
                             bool bCached,
                             QObject *parent = nullptr);

protected:
    virtual void run();

private:
    CLunarTable* m_pTable;
    QDate m_minDate, m_maxDate;
    bool m_bCached;
};

#endif // LUNARTABLE_H
