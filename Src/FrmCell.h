#ifndef FRMCELL_H
#define FRMCELL_H

#include <QWidget>
#include <QPalette>
#include <QFont>

namespace Ui {
class CFrmCell;
}

class CFrmCell : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmCell(QWidget *parent = nullptr);
    ~CFrmCell();
    
    int SetValue(const QString &szSolar,
                 const QPalette &palSolar,
                 const QFont &fontSolar,
                 const QString &szLunar,
                 const QPalette &palLunar,
                 const QFont &fontLunar);
    
private:
    Ui::CFrmCell *ui;
};

#endif // FRMCELL_H
