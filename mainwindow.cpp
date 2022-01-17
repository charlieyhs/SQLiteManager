#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "duSettings.h"
#include "findreplacedialog.h"
#include "dutreewidget.h"
#include "dupreferenciasdialog.h"
#include "dunuevatabladialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QListWidgetItem>
//-----------Arrastrar y soltar----------
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QUrl>

using TreeItem = QTreeWidgetItem;// Nuevo nombre para QTreeWidgetItem
using ListItem = QListWidgetItem;// Nuevo nombre para QListWidgetItem
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDb = QSqlDatabase::addDatabase("QSQLITE");// Acceder a una base de datos como Sqlite

    mQueryModel = new QSqlQueryModel(this);//Creamos un modelo query para la tabla

    ui->tableView->setModel(mQueryModel);//modelo de la tabla

    centralWidget()->layout()->setMargin(0);//centra los widgets que tenemos en nuestro frame o panel, con un margen de 0

    auto vFont = duLoad(GFONT,KFONTTYPE,ui->textEdit->font());//cargar los ajustes que se hicieron la ultima vez que se entró a la app

    ui->textEdit->setFont(vFont.value<QFont>());//se pone a nuestro textEdit la fuente que ha sido cargada en la variable vFont

    setAcceptDrops(true);//significa que nuestra aplicacion acepta elementos arrastrados desde fuera, arrastar y soltar(drag and drop)

    connect(ui->treeWidget, &DuTreeWidget::nuevaTabla,[&](){//conectamos nuestro treeWdiget o arbol, a la SIGNAL de nueva tabla con el SLOT
    //que es una funcion anonima o lambda "[&](){}"la señal "nuevaTabla del archivo DuTreeWidget hará:

        auto itm = ui->treeWidget->currentItem();//itm es el elemento actual que tenemos seleccionado en nuestro treeWidget

        if(itm == nullptr){//itm es un puntero, entonces, si itm no apunta a nada, es null, entonces no es un BBDD y no
        //hacemos la nada
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una BBDD");
            return;
        }
        auto padre = itm->parent();//ahora, padre apunta al padre de nuestra variable itm, si tiene padre, significa que
        //no es una BBDD porque la base de datos en nuestro padre

        if(padre != nullptr){//entonces si padre != nullptr, es decir, si padre si apunta a algo, entonces si tiene padre, por ende, no es una BBDD
            //se trata de una tabla porque el nodo del arbol tiene padre
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una BBDD");
            return;
        }
        // si pasa el primer if() significa que itm no es nullptr, por ende, si está apuntando a algo, pero debemos de saber si es a un padre o no
        // en le segundo if() sabemos si padre apunta a un padre o no, si padre es diferente a nullptr entonces si apunta a algo, por ende, no es nuestro padre principal
        // el nodo raiz, o nuestro padre principal no apunta a nada.

        //se trata de una base de datos, ya pasamos todas las restricciones que habian
        DuNuevaTablaDialog D(this);//Creamos un objeto de nuestro dialogo encargado de crear una nueva tabla
        if(D.exec()==QDialog::Rejected){//si al ejecutar nuestro objeto de dialogo, el usuario da en cancelar, que nos envia un Rejected
            return;//entonces simplemente no hacemos nada
        }
        // agregar el codigo SQL para generar esa tabla, ya que el usuario nos ha dado en aceptar
        ui->textEdit->setPlainText(D.resultado());//ahora ponemos en el texto que nos retorna la funcion resultado que es un QString con la consulta necesaria
        //para crear una nueva tabla
    });

    connect(ui->treeWidget, &DuTreeWidget::eliminarTabla,[&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){//si el elemento seleccionado no tiene nada entonces no sirve
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una Tabla");
            return;
        }
        auto padre = itm->parent();
        if(padre == nullptr){//si padre es igual a nullptr tampoco sirve, porque significa que es una base de datos y esto es para tablas
            //se trata de una tabla porque el nodo del arbol tiene padre
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una Tabla");
            return;
        }
        ui->textEdit->setPlainText(QString("DROP TABLE %1").arg(itm->text(0)));//agregamos al textEdit la consulta Sql necesaria para borrar una tabla

    });
    connect(ui->treeWidget, &DuTreeWidget::removerBase,[&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una BBDD");
            return;
        }
        auto padre = itm->parent();
        if(padre != nullptr){
            //se trata de una tabla porque el nodo del arbol tiene padre
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una BBDD");
            return;
        }
        delete itm;
    });
    connect(ui->treeWidget, &DuTreeWidget::selectFrom,[&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una Tabla");
            return;
        }
        auto padre = itm->parent();
        if(padre == nullptr){
            //se trata de una tabla porque el nodo del arbol tiene padre
            QMessageBox::warning(this,"Advertencia","Esta funcion solo esta permitida cuando se selecciona una Tabla");
            return;
        }
        ui->textEdit->setPlainText(QString("SELECT * FROM %1").arg(itm->text(0)));//el arg() es el texto de lo que haya seleccionado en el treeWidget
        //entonces será el nombre, y se le agrega al textEdit  la consulta sql
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)//funciones necesarias para las acciones o eventos de arrastrar y soltar
{
    e->accept();// acceptamos el evento de arrastre e ingreso
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *e)//funciones necesarias para las acciones o eventos de arrastrar y soltar
{
    e->accept();// acceptamos el evento de arrastre de salir, arrastrar cosas para sacarlas de la aplicacion
}

void MainWindow::dragMoveEvent(QDragMoveEvent *e)//funciones necesarias para las acciones o eventos de arrastrar y soltar
{
    e->accept();//aceptamos el evento de arrastre y mover
}

void MainWindow::dropEvent(QDropEvent *e)
{
    //los datos MIME son Extensiones de correo de Internet multipropósito (MUltipurpose Internet Mail Extensions) que sirve para el intercambio de todo
    //tipo de archivos entre usuarios

    auto datos = e->mimeData();//sacamos los datos MIME que tiene nuestro proyecto

    auto urls = datos->urls();//despues de los datos MIMES, sacamos las urls, es una QList<QUrl>

    for(auto u : urls){//foreach()---- 'u' va a ser igual a las urls

        auto nombre = u.toLocalFile();//sacamos el nombres de nuestro archivo local
        auto baseItem = new TreeItem;

        baseItem->setIcon(0,QIcon(":base"));//icono para nuestro TreeItem o arbol, que es en la columna 0
        baseItem->setText(0,nombre);//icono para nuestro TreeItem o arbol, que es en la columna 0

        mDb.setDatabaseName(nombre);//establecemos el nombre de nuestra base de datos, que es la que ya tenemos guardada

        if(!mDb.open()){//si no podemos abrir mDb, si surge algun error
            QMessageBox::critical(this,"Error",QString("Error: No se ha podido abrir la BBDD %1").arg(nombre));
            return;
        }

        QSqlQuery qry;//creamos una variable de una consulta Sql

        qry.exec("SELECT tbl_name FROM sqlite_master WHERE type LIKE'table'");//ejecutamos la consulta para mostrar todas las tablas
        while(qry.next()){//recorremos el resultado de la consulta
            auto tablaItem = new TreeItem;
            tablaItem->setIcon(0,QIcon(":tabla"));//ponemos el icono a nuestra variable de tablaItem que es objeto de TreeItem
            tablaItem->setText(0,qry.value(0).toString());//ponemos texto en la columna 0, el valor 0 de nuestra consulta en texto o string
            baseItem->addChild(tablaItem);//a nuestra base principal, o nodo principal en el momento, le agregamos un hijo, que será tablaItem
            //entonces asi se va agregando hijos a los que vamos arrastrando y soltando
        }
        ui->treeWidget->addTopLevelItem(baseItem);//agregamos un nuevo item a nuestro treeWidget a lo ultimo, y lo agregado será lo que hemos arrastrado
    }
}


void MainWindow::on_actionAbrir_Base_triggered()
{
    auto nombre = QFileDialog::getOpenFileName(this,"Abrir Base de datos",
                                               QDir::currentPath(),"Base de datos (*.db);;"\
                                               "Cualquier tipo (*.*)");//abrimos la ayuda que es un QFileDialog, que nos abre un dialogo directamente
    //para acceder a los archivos que tenemos en nuestro pc

    if(nombre.isEmpty()){return;}//si el nombre es vacio, no hacemos nada
    auto baseItem = new TreeItem;
    baseItem->setIcon(0,QIcon(":base"));
    baseItem->setText(0,nombre);

    mDb.setDatabaseName(nombre);//establecemos el nombre de la base, con el nombre que nos ha pasado el QFileDialog
    if(!mDb.open()){//por si hay errores al abrir la BBDD
        QMessageBox::critical(this,"Error",QString("Error: No se ha podido abrir la BBDD %1").arg(nombre));
        return;
    }
    QSqlQuery qry;//creamos una consulta de sql

    qry.exec("SELECT tbl_name FROM sqlite_master WHERE type LIKE'table'");//ejecutamos la consulta de select, para mostrar todas las tablas que hayan
    while(qry.next()){//recorremos el resultado de la consulta
        auto tablaItem = new TreeItem;//creamos un nuevo TreeItem
        tablaItem->setIcon(0,QIcon(":tabla"));//le ponemos el icono de la tabla
        tablaItem->setText(0,qry.value(0).toString());//le ponemos el texto que nos ha pasado, o que hay en la posicion cero de nuestra consulta a texto
        baseItem->addChild(tablaItem);//agregamos un hijo a nuestro nodo base, y el que agregamos es el tablaItem, o la nueva tabla creada
    }
    ui->treeWidget->addTopLevelItem(baseItem);//agregamos en la ultima parte, un nuevo nodo al arbol que tenemos
}

void MainWindow::on_actionGuardar_Base_triggered()
{
    auto nombre = QFileDialog::getSaveFileName(this,"Guardar Base de datos",
                                               QDir::currentPath(),"Base de datos (*.db);;"\
                                               "Cualquier tipo (*.*)");//abrimos la ayuda que es un QFileDialog, que nos abre un dialogo directamente
    //para acceder a los archivos que tenemos en nuestro pc

    if(nombre.isEmpty()){return;}//si el nombre es vacio no se hace nada
    auto baseItem = new TreeItem;//creamos una base para nuestro TreeItem
    baseItem->setIcon(0,QIcon(":base"));//le ponemos icono
    baseItem->setText(0,nombre);//le ponemos el texto que viene de el QFileDialog
    ui->treeWidget->addTopLevelItem(baseItem);//agregamos un nuevo item en la cima de nuestro treeWidget
    mDb.setDatabaseName(nombre);//establecemos el nombre a la base de datos
    if(!mDb.open()){//por si da algun error al abrir la base de datos
        QMessageBox::critical(this,"Error",QString("Error: No se ha podido Guardar la BBDD %1").arg(nombre));
    }
    //si no da error al abrir, pues ya guardamos
}

void MainWindow::on_actionAbrir_SQL_triggered()
{
    auto nombre = QFileDialog::getOpenFileName(this,"Abrir SQL",QDir::currentPath(),
                                               "Archivo SQL (*.sql);;"\
                                               "Cualquier tipo (*.*)");//abrimos la ayuda que es un QFileDialog, que nos abre un dialogo directamente
    //para acceder a los archivos que tenemos en nuestro pc en este caso para abrir
    if(nombre.isEmpty()){return;}//si nombre es vacio no hacemos nada, retornamos
    QFile F(nombre);//creamos una variable de archivo llamada F()con el nombre que nos da el usuario a traves del QFileDialog
    if(!F.open(QIODevice::Text | QIODevice::ReadOnly)){//si no se puede abrir de manera de sola lectura, y de texto, entonces da error
        QMessageBox::critical(this,"Error",QString("Error: %1").arg(F.errorString()));
        return;
    }
    QTextStream S(&F);//el QTextStream guarda todo lo que hay en el archivo 'F', se lo pasamos por referencia, o apuntando a la direccion de la variable
    ui->textEdit->setText(S.readAll());//ahora ponemos como texto en el textEdit todo lo que tenga el textStream S
    F.close();//cerramos el archivo
}

void MainWindow::on_actionGuardar_SQL_triggered()
{
    auto nombre = QFileDialog::getSaveFileName(this,"Guardar SQL",QDir::currentPath(),
                                               "Archivo SQL (*.sql);;"\
                                               "Cualquier tipo (*.*)");//abrimos la ayuda que es un QFileDialog, que nos abre un dialogo directamente
    //para acceder a los archivos que tenemos en nuestro pc, en este caso para guardar
    if(nombre.isEmpty()){return;}//si el nombre es vacio retornamos
    QFile F(nombre);//creamos una variable de QFile que es un archivo, y lo ponemos como nombre lo que nos pasa el QFileDialog
    if(!F.open(QIODevice::Text | QIODevice::WriteOnly)){//si surge algun error al abrir de manera de escritura y de texto
        QMessageBox::critical(this,"Error",QString("Error: %1").arg(F.errorString()));//mostramos el error ocasionado
        return;//y nos salimos
    }
    QTextStream S(&F);//Creamos una variable de textStream para guardar todo lo que tiene el archivo F, le pasamos la direccion del archivo F
    S << ui->textEdit->toPlainText();//en el QTextStream agregamos lo que hay en el textEdit, y se lo pasamos como texto plano
    F.flush();// solo se limpia cuando se escribe en un archivo
    F.close();//se cierra el archivo
}

void MainWindow::on_actionImprimir_triggered()
{
    QPrintDialog P(this);//se abre el printDialog que sirve para abrir un dialog de impresion
    if(P.exec() == QDialog::Rejected){//si en el dialogo el usuario le da cancelar, que nos arroja un Rejected
        return;//retornamos, no hacemos nada
    }
    ui->textEdit->print(P.printer());//llamamos a la funcion print(imprimir) nos funciona para imprimir, e imprimimos lo que la funcion printer nos retorne
    //en este caso seria lo que se ha impreso o se va a imprimir en el QPrintDialog
}

void MainWindow::on_actionQuitar_triggered()
{
    close();//Cerrar el programa
}

void MainWindow::on_actionCopiar_triggered()
{
    ui->textEdit->copy();//para copiar en el text edit
}

void MainWindow::on_actionCortar_triggered()
{
    ui->textEdit->cut();//para cortar en el text edit
}

void MainWindow::on_actionPegar_triggered()
{
    ui->textEdit->paste();//para pegar en el text edit
}

void MainWindow::on_actionEjecutar_triggered()
{

    auto sql = ui->textEdit->toPlainText();//guardamos lo que hay en el textEdit
    if(sql.startsWith("select",Qt::CaseInsensitive)){//si la consulta inicia con  "select"
        mQueryModel->setQuery(sql);//establecemos a mQueryModel la consulta que nos han pasado

    }else if(sql.startsWith("create table",Qt::CaseInsensitive)){//si la consulta empieza con create table
        QSqlQuery qry;//creamos una variable de consultaSQL
        if(!qry.exec(sql)){//si la consulta no se ejecuta correctamente
            ui->listWidget->addItem(new ListItem(QIcon(":fallo"),sql));//ponemos en el listWidget un nuevo item diciendo el fallo
            return;//salimos
        }
        //si la consulta se ejecuta correctamente
        ui->listWidget->addItem(new ListItem(QIcon(":correcto"),sql));//en el listWidget agregamos un nuevo item diciendo que se ha ejecutado correctamente
        auto base = mDb.databaseName();//guardamos el nombre de la base en una variable
        int i,tam = ui->treeWidget->topLevelItemCount();//utilizamos i para los for(), y tam va a ser igual al ultimo elemento del treeWidget

        for(i=0;i<tam;i++){
            if(ui->treeWidget->topLevelItem(i)->text(0).toLower() == base.toLower()){//si el el texto de la posicion 0 del ultimo elemento del treeWidget
                //convertido en minusculas para que no se confunda es igual al nombre de la base
                break;//entonces nos salimos porque hemos encontrado la base que necesitamos
            }
        }
        auto baseItem = ui->treeWidget->topLevelItem(i);//utilizamos el valor en el que haya quedado la variable 'i' y creamos un nuevo item que va a ser
        // en la posicion 'i' del treeWidget
        auto createTable = QString("create table ");//creamos una variable de QString para el create table
        if(sql.contains("if not exists",Qt::CaseInsensitive)){//si la consulta sql tiene "if not exists"
            createTable = QString("create table if not exists ");//agregamos ese texo a la variable createTable
        }

        //Buscando el nombre de la tabla
        int pos1 = createTable.size();//pos1 va a ser igual a la longitud del QString de createTable
        int pos2 = sql.indexOf("(",pos1);//pos2 va a ser igual al indice en donde encuentre "(", desde el valor de pos1
        auto tabla = sql.mid(pos1,pos2-pos1);//tabla va a ser igual a lo que la funcion mid recorte desde pos1 hasta (pos2-pos1) en el QString sql
        tabla = tabla.mid(0,tabla.indexOf(" "));//tabla va a ser igual a lo que la funcion mid retorne desde la posicion 0, hasta lo que retorne(tabla.indexOf(" "));
        //o sea, otra posicion que es donde encuentra el espacio, y donde encuentra el espacio, se acaba el nombre de la tabla

        tam = baseItem->childCount();//tamaño pasa a ser igual a la cantidad de hijos que tenga nuestro TreeItem "baseItem"
        for(i=0;i<tam;i++){//desde 0 hasta la cantidad de hijos que hayan
            if(baseItem->child(i)->text(0).toLower()== tabla.toLower()){//si lo que hay en el texto(0) del hijo(i)
                //de baseItem en minusculas es igual al nombre de la tabla
                return;//entonces nos salimos porque la tabla ya existe, y no pueden haber dos tablas con el mismo nombre
            }
        }

        auto tablaItem = new TreeItem;//un nuevo item para el arbol
        tablaItem->setIcon(0,QIcon(":tabla"));//agregamos el icono
        tablaItem->setText(0,tabla);//agregamos el texto, que en este caso en el nombre de la tabla
        baseItem->addChild(tablaItem);//y agregamos como hijo a nuestro baseItem el tablaItem que contiene el nombre de la tabla creada

    }else if(sql.startsWith("drop table",Qt::CaseInsensitive)){//si la consulta empieza con "drop tabl"
        QSqlQuery qry;//creamos una consulta de sql
        if(!qry.exec(sql)){//si al ejecutar la consulta sql falla
            ui->listWidget->addItem(new ListItem(QIcon(":fallo"),sql));
            return;
        }
        ui->listWidget->addItem(new ListItem(QIcon(":correcto"),sql));//si no falla agregamos a listWidget correcto
        auto base = mDb.databaseName();//creamos una variable con el nombre de la base de datos
        int i,tam = ui->treeWidget->topLevelItemCount();//tam va a ser igual al ultimo elementos que hay en la treeWidget

        for(i=0;i<tam;i++){//desde 0 hasta el valor de tam
            if(ui->treeWidget->topLevelItem(i)->text(0).toLower() == base.toLower()){//si el texto(0) en minusculas que hay en
                //la cima del treeWidget es igual al nombre de la base en minusculas
                break;//rompemos el bucle for porque ya hemos encontrado la base de datos
            }
        }

        auto baseItem = ui->treeWidget->topLevelItem(i);//baseItem va a ser igual al elemento de nivel superior del elemento i
        tam = baseItem->childCount();// tam pasa a ser igual a los hijos que tiene baseItem
        auto dropTable = QString("drop table");// creamos un QString para eliminar la tabla
        auto tabla = sql.mid(dropTable.size());// tabla va a ser igual al QString que la funcion mid retorne, desde el tamaño de dropTable
        //o sea, retorna un string de lo que haya despues de dropTable
        tabla = tabla.mid(1);//tabla pasa a ser igual a lo que hay en el QString desde la posicion 1
        for(i=0;i<tam;i++){
            if(baseItem->child(i)->text(0).toLower() == tabla.toLower()){//si el texto(0) en minusculas que hay en el hijo(i) de baseItem
                //es igual al nombre de la tabla en minuscula
                break;//rompemos el bucle for porque ya hemos encontrado el nombre de la tabla que vamos a borrar
            }
        }
        baseItem->removeChild(baseItem->child(i));//removemos el hijo que hay en la posicion 'i'

    }else{//si no es ni select, ni create table, ni drop table, entonces
        QSqlQuery qry;//creamos una variable de consulta SQL
        if(!qry.exec(sql)){//si al ejecutar la consulta sucede algun error
            ui->listWidget->addItem(new ListItem(QIcon(":fallo"),sql));//agregamos al listWidget que ha fallado
            return;//y salimos
        }
        ui->listWidget->addItem(new ListItem(QIcon(":correcto"),sql));//si se ejecuta correctamente, agregamos un item
        //o elemento al listWidget que se ha ejecutado correctamente
    }
}

void MainWindow::on_actionPreferencias_triggered()
{
    DuPreferenciasDialog p(this);//Creamos un objeto del archivo o clase DuPreferenciasDialog
    connect(&p, &DuPreferenciasDialog::aplicadoFuente,[&](const QFont &fuente){//conectamos al objeto b, con la señal 'aplicadoFuente', con
        //el SLOT que es una funcion anonima o lambda que recibe un QFont, ya en la funcion
        ui->textEdit->setFont(fuente);//le establecemos una fuente a nuestro textEdit, y la fuente que le establecemos es que han puesto en el FontComboBox
        //o lo que haya guardado en los ajustes
        duSave(GFONT,KFONTTYPE,fuente);//guardamos los ajustes que se han seleccionado, para esto le paso la macro definida en "duSettings", de igual
        //manera el KFONTTYPE que esta definida en el "duSettings", y le pasamos la fuente que queremos guardar

        // duSave(GFONT, KFONTSIZE,fuente.pointSizeF());
    });
    if(p.exec()==QDialog::Rejected){//si el usuario da cancelar en el cuadro de dialogo para las preferencias
        return;//simplemente retornamos, nos salimos
    }

}

void MainWindow::on_actionManual_triggered()
{
    QMessageBox::information(this,"Manual","No hay manual para este programa");//Mostramos un mensaje del manual
}

void MainWindow::on_actionAcerca_de_SQLiteManager_triggered()
{
    QMessageBox::about(this,"SQLite Manager","Autor: Carlos Hernandez\n"
                       "Compañia: Carlos Yecid\n"
                       "Fecha: 2 de enero de 2015\n"
                       "Software: SQLite Manager");//un cuadro de mensaje para mostrar acerca del autor y cosas asi
}

void MainWindow::on_actionBuscar_y_Reemplazar_triggered()
{
    FindReplaceDialog F(this);//creamos un objeto de la clase y dialogo para buscar y reemplazar
    F.setTextEdit(ui->textEdit);//implementamos la funcion setTextEdit() que es donde vamos a buscar y reemplazar, en este caso
    //es en nuestro textEdit
    F.exec();//ejecutamos el dialogo
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{
    if(index.parent().isValid()){
        return;
    }
    auto nombre = index.data().toString();
    mDb.setDatabaseName(nombre);
    if(!mDb.open()){
        QMessageBox::critical(this,"Error",QString("Error: No se ha podido Guardar la BBDD %1").arg(nombre));
    }
}
