#ifndef BCP2CLASSTESTS_H
#define BCP2CLASSTESTS_H

#include <QtTest/QtTest>
#include <QObject>
#include "protocols/bcp2.h"

class bcp2ClassTests : public Bcp2
{
    Q_OBJECT
private slots:
    void shortFromData();
    void ulongFromData();
    void floatFromData();
    void doubleFromData();
    void angleOrientationFromData();
};

#endif // BCP2CLASSTESTS_H
