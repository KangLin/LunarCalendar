#ifndef LUNARTABLE_H
#define LUNARTABLE_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QFile>
#include <QMutex>

class CLunarTable : public QObject
{
    Q_OBJECT
public:
    static CLunarTable* Instance();

    struct _LUNAR_DAY
    {
        quint8 nTg;
        quint8 nDz;
        bool bLeap;
        quint8 nMonth;
        quint8 nDay;
        qint8 nJq;
    };
    
    int Load(const QString &file);
    int Save(const QString &file, bool bAll = true);

    int Generate(const QDate &min, const QDate &max, const QString& szFile, int nThread = 2, bool bSaveAllDate = true);
    int Generate(const QDate &min, const QDate &max);
    int GetLunar(const QDate &date, _LUNAR_DAY &lunar);

public Q_SLOTS:
    void slotGenerateFinished();
    
private:
    explicit CLunarTable(QObject *parent = nullptr);
    int LoadVersion0(QDataStream &data);
    int CleanOutsideRange(const QDate&min, const QDate& max);
    
    QMap<qint64, _LUNAR_DAY> m_Lunar;
    QMutex m_GenerateMutex;
    QString m_szFile;
    int m_nThreadNumber;
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
                             QObject *parent = nullptr);
    
protected:
    virtual void run();
    
private:
    CLunarTable* m_pTable;
    QDate m_minDate, m_maxDate;
};

#endif // LUNARTABLE_H
