#ifndef BINRUNITTESTS_H
#define BINRUNITTESTS_H

#include <QtTest/QtTest>
#include "protocols/binr.h"

/* Описание класса */
/*!
 * \brief Класс Unit-тестов класса BINR
 ***********************************************************************************/
class BinrUnitTests : public QObject
{
    Q_OBJECT
private slots:
   /*!
    * \brief  Проверка распаковки BINR пакетов без CRC и без данных
    * \remark Проверяется весь диапазон id (0 - 255).
    *         На 0х03, 0х10, 0хFF ожидается возникновение ошикби BINR_ERR_ID
    ***********************************************************************************/
    void decodeWithoutCrcWithoutData();

};

#endif // BINRUNITTESTS_H
