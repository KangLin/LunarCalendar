#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>
#include <QMap>

class CCalendarLunar : public QObject
{
    Q_OBJECT
public:
    explicit CCalendarLunar(QObject *parent = nullptr);

    QString GetLunar(const QDate &date);
    QString GetLunarDay(const QDate &date);

    int AddHoliday(int month, int day, const QString &szName);
    int AddAnniversary(int month, int day, const QString &szName);

private:
    int InitHoliday();
    
    QMap<int, QMap<int, QString> > m_Holiday;
};

#endif // LUNAR_H
