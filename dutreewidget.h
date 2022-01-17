#ifndef DUTREEWIDGET_H
#define DUTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>

class QAction;

class DuTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    DuTreeWidget(QWidget *parent=nullptr);
signals:
    void nuevaTabla();
    void eliminarTabla();
    void removerBase();
    void selectFrom();
private:
    QAction *mActionNuevaTabla;
    QAction *mActionEliminarTabla;
    QAction *mActionRemoverBase;
    QAction *mSELECT_FROM;

};

#endif // DUTREEWIDGET_H
