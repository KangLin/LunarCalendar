#ifndef LUNARCALENDARDELEGATE_H
#define LUNARCALENDARDELEGATE_H

#include <QStyledItemDelegate>

class CLunarCalendarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    explicit CLunarCalendarDelegate(QObject *parent = nullptr);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LUNARCALENDARDELEGATE_H
