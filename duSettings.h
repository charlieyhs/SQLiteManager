#ifndef DUSETTINGS_H
#define DUSETTINGS_H

#include <QString>
#include <QVariant>

#define GFONT "GFONT"
#define KFONTTYPE "KFONTTYPE"
#define KFONTSIZE "KFONTSIZE"


// carga ajustes
QVariant duLoad(const QString &g, const QString &k,
                const QVariant &vd = QVariant());

//Guarda ajustes
void duSave(const QString &g, const QString &k, const QVariant &v);

#endif // DUSETTINGS_H
