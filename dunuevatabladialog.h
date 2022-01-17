#ifndef DUNUEVATABLADIALOG_H
#define DUNUEVATABLADIALOG_H

#include <QDialog>

namespace Ui {
class DuNuevaTablaDialog;
}

class DuNuevaTablaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuNuevaTablaDialog(QWidget *parent = nullptr);
    ~DuNuevaTablaDialog();
    QString resultado() const;

private slots:
    void on_cancelarPushButton_clicked();
    void on_aceptarPushButton_clicked();

private:
    Ui::DuNuevaTablaDialog *ui;
};

#endif // DUNUEVATABLADIALOG_H
