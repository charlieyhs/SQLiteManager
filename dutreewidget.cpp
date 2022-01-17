#include "dutreewidget.h"
#include <QAction>
#include <QMouseEvent>

DuTreeWidget::DuTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{

    mActionNuevaTabla = new QAction(QIcon(":tabla"),"Nueva Tabla",this);
    mActionEliminarTabla = new QAction(QIcon(":eliminar"),"Eliminar Tabla",this);
    mActionRemoverBase = new QAction(QIcon(":remover"),"Remover Base",this);
    mSELECT_FROM = new QAction(QIcon(":mostrar"),"SELECT * FROM",this);

    addActions({mActionNuevaTabla,mActionEliminarTabla,
                mActionRemoverBase,mSELECT_FROM});


    connect(mActionNuevaTabla, &QAction::triggered,[&](){
           emit nuevaTabla();
    });
    connect(mActionEliminarTabla, &QAction::triggered,[&](){
        emit eliminarTabla();
    });
    connect(mActionRemoverBase, &QAction::triggered,[&](){
        emit removerBase();
    });
    connect(mSELECT_FROM, &QAction::triggered,[&](){
        emit selectFrom();
    });

}

