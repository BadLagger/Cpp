#ifndef ADDITIONALSETSWINDOW_H
#define ADDITIONALSETSWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdditionSetsWindow;
}

class AdditionalSetsWindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum{
        Set_Block1 = 0,
        Set_Block2,
        Set_Max
    }BlockSets;
    /*!
     * \brief     Конструктор класса AdditionalSetsWindow
     * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
     * \param[in] указатель на родительский виджет
     ***********************************************************************************/
    explicit AdditionalSetsWindow(QWidget *parent = nullptr);
    /*!
     * \brief     Деструктор класса InfoWindow
     ***********************************************************************************/
    ~AdditionalSetsWindow();
    /*!
     * \brief Отобразить окно
     ***********************************************************************************/
    void showWindow();
signals:
    void setBlockSignal(BlockSets set);
private:
    Ui::AdditionSetsWindow *ui;                //! указатель на форму окна
    BlockSets              set;                //! выбор блока
};

#endif // ADDITIONALSETSWINDOW_H
