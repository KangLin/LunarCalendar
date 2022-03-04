// Author: Kang Lin <kl222@126.com>

#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>
#include <QMap>
#include <QPixmap>

class CCalendarLunar : public QObject
{
    Q_OBJECT
public:
    explicit CCalendarLunar(QDate date, QObject *parent = nullptr);
    explicit CCalendarLunar(QObject *parent = nullptr);

    int GetLunar(const QDate &date);
    int GetYear(){return 0;} //TODO: Not implemented
    int GetMonth();
    int GetDay();
    QString GetLunar();
    QString GetLunarDay();
    QString GetHoliday();
    QString GetAnniversary();
    QString GetJieQi();
    QString GetJieQiImage();
    
    static int AddHoliday(int month, int day, const QString &szName);
    static int AddAnniversary(int month, int day, const QString &szName);

private:
    int InitHoliday();
    
    QString m_szLunar;
    QString m_szLunarDay;
    QString m_szHoliday;
    QString m_szAnniversary;
    QString m_szJieQi;
    QString m_szImageJieQi;
    int m_Year;
    int m_Month;
    int m_Day;
};

#endif // LUNAR_H
