/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>
#include <QMap>
#include <QPixmap>

/*!
 * \brief 农历类
 * \ingroup INTERNAL_API
 */
class CCalendarLunar : public QObject
{
    Q_OBJECT
public:
    explicit CCalendarLunar(QDate date, QObject *parent = nullptr);
    explicit CCalendarLunar(QObject *parent = nullptr);

    /*!
     * \brief 得到指定日期的农历
     * \param date：日期
     * \return 成功返回 0，否则返回非 0
     */
    int GetLunar(const QDate &date);
    int GetYear(){return 0;} //TODO: Not implemented
    int GetMonth();
    int GetDay();
    
    QString GetLunar();
    QString GetLunarDay();
    QStringList GetHoliday();
    QString GetAnniversary();
    QString GetJieQi();
    QString GetJieQiImage();
    
    static int AddHoliday(int month, int day, const QString &szName);
    static int AddAnniversary(int month, int day, const QString &szName);

private:
    int InitHoliday();
    
    QString m_szLunar;
    QString m_szLunarDay;
    QStringList m_Holiday;
    QString m_szAnniversary;
    QString m_szJieQi;
    QString m_szImageJieQi;
    int m_Year;
    int m_Month;
    int m_Day;
};

#endif // LUNAR_H
