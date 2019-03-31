#ifndef FRMCELL_H
#define FRMCELL_H

#include <QWidget>

namespace Ui {
class CFrmCell;
}

class CFrmCell : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmCell(QWidget *parent = nullptr);
    ~CFrmCell();
    
    int SetValue(const QString &szSolar, const QString &szLunar);
    
private:
    Ui::CFrmCell *ui;
};

#endif // FRMCELL_H
