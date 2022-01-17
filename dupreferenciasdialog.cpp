#include "dupreferenciasdialog.h"
#include "ui_dupreferenciasdialog.h"
#include "duSettings.h"

DuPreferenciasDialog::DuPreferenciasDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuPreferenciasDialog)
{
    ui->setupUi(this);
    auto vFont= duLoad(GFONT,KFONTTYPE);
    ui->fontComboBox->setCurrentFont(vFont.value<QFont>());
    ui->doubleSpinBox->setValue(vFont.value<QFont>().pointSizeF());
}

DuPreferenciasDialog::~DuPreferenciasDialog()
{
    delete ui;
}

void DuPreferenciasDialog::on_aplicarPushButton_clicked()
{
    auto f = ui->fontComboBox->currentFont();
    f.setPointSizeF(ui->doubleSpinBox->value());
    emit aplicadoFuente(f);

}

void DuPreferenciasDialog::on_aceptarPushButton_clicked()
{
    on_aplicarPushButton_clicked();
    accept();
}
