/*!
 * \copyright 康林工作室版权所有 (c)，保留所有权力。
 * \author 康　林 <kl222@126.com>
 */

#ifndef LUNARCALENDARDELEGATE_H_KL
#define LUNARCALENDARDELEGATE_H_KL

#include <QStyledItemDelegate>

/*!
 * \brief 农历代理
 * \ingroup INTERNAL_API
 */
class CLunarCalendarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CLunarCalendarDelegate(QObject *parent = nullptr);

public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &o,
                       const QModelIndex &index) const override;
};

/*!
 * \brief 农历头代理
 * \ingroup INTERNAL_API
 */
class CLunarCalendarHeaderDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CLunarCalendarHeaderDelegate(QObject *parent = nullptr);

public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
};

#endif // LUNARCALENDARDELEGATE_H_KL
