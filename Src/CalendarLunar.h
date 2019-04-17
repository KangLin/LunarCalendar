#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>
#include <QMap>

class CCalendarLunar : public QObject
{
    Q_OBJECT
public:
    explicit CCalendarLunar(QDate date, QObject *parent = nullptr);
    explicit CCalendarLunar(QObject *parent = nullptr);
    
    int GetLunar(const QDate &date);
    QString GetLunar();
    QString GetLunarDay();
    QString GetHoliday();
    QString GetAnniversary();
    QString GetJieQi();
    
    static int AddHoliday(int month, int day, const QString &szName);
    static int AddAnniversary(int month, int day, const QString &szName);

private:
    int InitHoliday();
    
    QString m_szLunar;
    QString m_szLunarDay;
    QString m_szHoliday;
    QString m_szAnniversary;
    QString m_szJieQi;
};

#endif // LUNAR_H
