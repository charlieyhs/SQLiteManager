#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dutreewidget.h"
#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QSqlQueryModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
private slots:

    void on_actionAbrir_Base_triggered();
    void on_actionGuardar_Base_triggered();
    void on_actionAbrir_SQL_triggered();
    void on_actionGuardar_SQL_triggered();
    void on_actionImprimir_triggered();
    void on_actionQuitar_triggered();
    void on_actionCopiar_triggered();
    void on_actionCortar_triggered();
    void on_actionPegar_triggered();
    void on_actionEjecutar_triggered();
    void on_actionPreferencias_triggered();
    void on_actionManual_triggered();
    void on_actionAcerca_de_SQLiteManager_triggered();
    void on_actionBuscar_y_Reemplazar_triggered();

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase mDb;
    QSqlQueryModel *mQueryModel;
};
#endif // MAINWINDOW_H
