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

int CFrmCell::SetValue(const QString &szSolar,
                       const QPalette &palSolar,
                       const QFont &fontSolar,
                       const QString &szLunar,
                       const QPalette &palLunar,
                       const QFont &fontLunar)
{
    ui->lbSolar->setText(szSolar);
    ui->lbSolar->setPalette(palSolar);
    ui->lbSolar->setFont(fontSolar);
    ui->lbLunar->setText(szLunar);
    ui->lbLunar->setPalette(palLunar);
    ui->lbLunar->setFont(fontLunar);
    return 0;
}
