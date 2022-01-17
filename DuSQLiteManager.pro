QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    duSettings.cpp \
    dunuevatabladialog.cpp \
    dupreferenciasdialog.cpp \
    dutreewidget.cpp \
    finddialog.cpp \
    findform.cpp \
    findreplacedialog.cpp \
    findreplaceform.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    duSettings.h \
    dudefines.h \
    dunuevatabladialog.h \
    dupreferenciasdialog.h \
    dutreewidget.h \
    finddialog.h \
    findform.h \
    findreplacedialog.h \
    findreplaceform.h \
    mainwindow.h

FORMS += \
    dunuevatabladialog.ui \
    dupreferenciasdialog.ui \
    findreplacedialog.ui \
    findreplaceform.ui \
    mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++11

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
