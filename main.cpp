#include "mainwindow.h"
#include "dudefines.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APPNAME);
    a.setApplicationVersion(APPVER);
    a.setOrganizationDomain(ORGDOM);
    a.setOrganizationName(ORGNAME);

    a.setStyle("fusion");
    MainWindow w;
    w.show();
    return a.exec();
}
