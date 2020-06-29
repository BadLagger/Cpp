#include "windows/mainwindow.h"
#ifdef QT_DEBUG
#include "tests/binrunittests.h"
#include "tests/bcp2classtests.h"
#endif
#include <QApplication>

/*!
 * \brief     Точка входа в программу
 * \param[in] количество аргументов командной строки
 * \param[in] указатель на массив строк с аргументами командной строки
 * \return    результат выполнения программы
 ***********************************************************************************/
int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    // набор юнит-тестов (пердставлены тесты только для тех модулей в которых были ранее обнаружены ошибки)
    BinrUnitTests  binrTest;
    bcp2ClassTests bcp2Test;

    QTest::qExec(&binrTest);
    QTest::qExec(&bcp2Test);

#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
