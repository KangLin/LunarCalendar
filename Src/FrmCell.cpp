#include "FrmCell.h"
#include "ui_FrmCell.h"

CFrmCell::CFrmCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrmCell)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
}

CFrmCell::~CFrmCell()
{
    delete ui;
}

int CFrmCell::SetValue(const QString &szSolar, const QPalette &palSolar,
                       const QString &szLunar, const QPalette &palLunar)
{
    ui->lbSolar->setText(szSolar);
    ui->lbSolar->setPalette(palSolar);
    ui->lbLunar->setText(szLunar);
    ui->lbLunar->setPalette(palLunar);
    return 0;
}
