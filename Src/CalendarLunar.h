#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>

class CCalendarLunar : public QObject
{
    Q_OBJECT
public:
    explicit CCalendarLunar(QObject *parent = nullptr);
    
    QString GetLunar(const QDate &date);
    QString GetLunarDay(const QDate &date);

signals:
    
public slots:
};

#endif // LUNAR_H
