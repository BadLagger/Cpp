#include "binrunittests.h"
/*!
 * \brief  Проверка распаковки BINR пакетов без CRC и без данных
 * \remark Проверяется весь диапазон id (0 - 255). На 0х03, 0х10, 0хFF ожидается возникновение ошикби BINR_ERR_ID
 ***********************************************************************************/
void BinrUnitTests::decodeWithoutCrcWithoutData()
{
    Binr test;

    for(int i = 0; i <= 255; i++){
        qDebug() << "BINR PKG ID " << i << "...";
        QCOMPARE(test.decode(0x10),     false);
        QCOMPARE(test.getErrorStatus(), false);
        QCOMPARE(test.decode(static_cast<unsigned char>(i)), false);
        if((i != 0x03) && (i != 0x10) && (i != 0xFF))
        {
          QCOMPARE(test.getErrorStatus(),     false);
          QCOMPARE(test.decode(0x10),         false);
          QCOMPARE(test.getErrorStatus(),     false);
          QCOMPARE(test.decode(0x03),         true);
          QCOMPARE(test.getErrorStatus(),     false);
          QCOMPARE(test.getLastDecodePkgId(), i);
          qDebug() << " good as expected\r\n";
        }
        else{
          QCOMPARE(test.getErrorStatus(), true);
          QCOMPARE(test.getLastError(), BINR_ERR_ID);
          qDebug() << " bad as expected\r\n";
        }
    }
}
