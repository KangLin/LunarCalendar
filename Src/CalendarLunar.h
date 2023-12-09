/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef LUNAR_H
#define LUNAR_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include <QDate>

/*!
 * \brief 农历类
 * \details 从阳历计算出农历
 * \ingroup INTERNAL_API
 */
class CCalendarLunar : public QObject
{
    Q_OBJECT

public:
    explicit CCalendarLunar(QDate date, QObject *parent = nullptr);

    /*!
     * \brief 得到指定日期的农历年份
     */
    int GetYear();
    //! 得到指定日期的农历月份
    int GetMonth();
    //! 得到指定日期的农历天数
    int GetDay();

    //! 得到指定日期的农历（汉字）
    //! \note 编码：UTF-8 ，由文件编码决定
    QString GetLunar();
    //! 得到指定日期的农历当月的第几天（汉字）
    //! \note 编码：UTF-8 ，由文件编码决定
    QString GetLunarDay();
    //! 得到节气
    QString GetJieQi();
    //! 得到节气的图片
    QString GetJieQiImage();
    
    /*!
     * \brief 得到指定农历的阳历
     * \param lunarYear 农历年份
     * \param lunarMonth 农历月份
     * \param lunarDay 农历日
     * \param bLeap
     *          - true: 闰月
     *          - false
     * \return 
     */
    static QDate GetSolar(int lunarYear, int lunarMonth, int lunarDay, bool bLeap = false);

private:
    explicit CCalendarLunar(QObject *parent = nullptr);

    /*!
     * \brief 得到指定日期的农历。
     * \param date：日期
     * \return 成功返回 0，否则返回非 0
     */
    int GetLunar(const QDate &date);
    
    QDate m_Solar;
    int m_Year;
    int m_Month;
    int m_Day;
    QString m_szLunar;
    QString m_szLunarDay;
    QString m_szJieQi;
    QString m_szImageJieQi;
};

#endif // LUNAR_H
