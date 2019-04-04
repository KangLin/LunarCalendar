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
    
    QString GetLunar(const QDate &date);
    QString GetLunar();
    QString GetLunarDay(const QDate &date);
    QString GetLunarDay();
    QString GetHoliday();
    QString GetAnniversary();
    int AddHoliday(int month, int day, const QString &szName);
    int AddAnniversary(int month, int day, const QString &szName);

private:
    int InitHoliday();
    QString m_szLunar;
    QString m_szLunarDay;
    QString m_szHoliday;
    QString m_szAnniversary;
    QMap<int, QMap<int, QString> > m_Holiday;
    QMap<int, QMap<int, QString> > m_Anniversary;
};

#endif // LUNAR_H
