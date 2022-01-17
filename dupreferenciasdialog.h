#ifndef DUPREFERENCIASDIALOG_H
#define DUPREFERENCIASDIALOG_H

#include <QDialog>
#include <QFont>

namespace Ui {
class DuPreferenciasDialog;
}

class DuPreferenciasDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DuPreferenciasDialog(QWidget *parent = nullptr);
    ~DuPreferenciasDialog();

signals:
    void aplicadoFuente(QFont);
private slots:
    void on_aplicarPushButton_clicked();
    void on_aceptarPushButton_clicked();

private:
    Ui::DuPreferenciasDialog *ui;
};

#endif // DUPREFERENCIASDIALOG_H
