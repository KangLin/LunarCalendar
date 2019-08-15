#ifndef LUNARCALENDARDELEGATE_H_KL
#define LUNARCALENDARDELEGATE_H_KL

#include <QStyledItemDelegate>

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
