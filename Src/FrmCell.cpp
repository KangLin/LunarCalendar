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

int CFrmCell::SetValue(const QString &szSolar, const QString &szLunar)
{
    ui->lbSolar->setText(szSolar);
    ui->lbLunar->setText(szLunar);
    return 0;
}
