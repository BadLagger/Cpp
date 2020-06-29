#-------------------------------------------------
#
# Project created by QtCreator 2019-10-23T11:59:43
#
#-------------------------------------------------

QT       += core gui \
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = su2cfg
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        customs/baseswidget.cpp \
        customs/blockbtn.cpp \
        customs/cbytearraytextedit.cpp \
        customs/comport.cpp \
        customs/customdoublevalidator.cpp \
        customs/masterslavewidget.cpp \
        customs/modecfg.cpp \
        main.cpp \
        protocols/bcp2.cpp \
        protocols/binr.cpp \
        protocols/nmea.cpp \
        protocols/nmeabase.cpp \
        windows/additionalsetswindow.cpp \
        windows/basesmanualwindow.cpp \
        windows/infowindow.cpp \
        windows/mainwindow.cpp \
        windows/progresswindow.cpp

HEADERS += \
    customs/baseswidget.h \
    customs/blockbtn.h \
    customs/cbytearraytextedit.h \
    customs/comport.h \
    customs/customdoublevalidator.h \
    customs/masterslavewidget.h \
    customs/modecfg.h \
    protocols/bcp2.h \
    protocols/binr.h \
    protocols/nmea.h \
    protocols/nmeabase.h \
    windows/additionalsetswindow.h \
    windows/basesmanualwindow.h \
    windows/infowindow.h \
    windows/mainwindow.h \
    windows/progresswindow.h

FORMS += \
    forms/additionsetswindow.ui \
    forms/basesmanualwindow.ui \
    forms/infowindow.ui \
    forms/mainwindow.ui \
    forms/progresswindow.ui

UNITTEST{

QT      += testlib

SOURCES += tests/bcp2classtests.cpp \
           tests/binrunittests.cpp

HEADERS += tests/bcp2classtests.h \
           tests/binrunittests.h
}

STATIC{

QMAKE_LFLAGS += -static -static-libgcc

}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
