#include "dunuevatabladialog.h"
#include "ui_dunuevatabladialog.h"

#include <QAction>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>

DuNuevaTablaDialog::DuNuevaTablaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuNuevaTablaDialog)
{
    ui->setupUi(this);
    auto agregar = new QAction("Agregar",this);
    auto eliminar = new QAction("eliminar",this);

    ui->tableWidget->addActions({agregar,eliminar});

    auto nuevoComboBox = [&](QWidget *parent){
        auto combo = new QComboBox(parent);
        combo->addItems({"INTEGER","DOUBLE","TEXT","DATE","TIME","BOOL"});
        return combo;

    };
    connect(agregar,&QAction::triggered,[&](){
       int nFilas = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(nFilas);
        ui->tableWidget->setCellWidget(nFilas, 1, nuevoComboBox(this));
        ui->tableWidget->setCellWidget(nFilas,2, new QRadioButton(this));
        ui->tableWidget->setCellWidget(nFilas,3,new QCheckBox(this));
        ui->tableWidget->setCellWidget(nFilas,4,new QCheckBox(this));
    });
    connect(eliminar,&QAction::triggered,[&](){
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());

    });
}

DuNuevaTablaDialog::~DuNuevaTablaDialog()
{
    delete ui;
}

QString DuNuevaTablaDialog::resultado() const
{
    QString res = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(ui->nombreLineEdit->text());

    int nFilas = ui->tableWidget->rowCount();
    for(int i=0;i<nFilas;i++){
        auto nombre = ui->tableWidget->item(i,0)->text();
        auto tipo = static_cast<QComboBox *>(ui->tableWidget->cellWidget(i,1))->currentText();
        bool pk = static_cast<QRadioButton *>(ui->tableWidget->cellWidget(i,2))->isChecked();
        bool nn = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,3))->isChecked();
        bool ai = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,4))->isChecked();
        res += nombre +" "+tipo;
        if(pk){
            res += " PRIMARY KEY";
        }
        if(ai){
            res += " AUTOINCREMENT";
        }
        if(nn){
            res += " NOT NULL";
        }
        res += ",";
    }
    res[res.size()-1]=')';
    return res;
}

void DuNuevaTablaDialog::on_cancelarPushButton_clicked()
{
    reject();
}

void DuNuevaTablaDialog::on_aceptarPushButton_clicked()
{
    accept();
}
