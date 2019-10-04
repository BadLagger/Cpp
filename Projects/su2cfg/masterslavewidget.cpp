#include "masterslavewidget.h"


/* Открытые методы */
/*!
 * \brief     Конструктор класса для графического отображения блоков СУ 02
 ***********************************************************************************/
MasterSlaveWidget::MasterSlaveWidget()
{
    masterBtn = nullptr;
    slaveBtn  = nullptr;
}
/*!
 * \brief     Инициализация графического элемента (спец. кнопки) для СУ-М
 * \param[in] указатель на спец. кнопку
 ***********************************************************************************/
void MasterSlaveWidget::setMasterElement(BlockBtn *master)
{
    if(masterBtn == nullptr) {
        masterBtn = master;
        masterBtn->setStyleSheet(STYLE_SETTINGS);
        connect(masterBtn, &QPushButton::clicked, [=]{
            masterBtn->toggleDirection();
        });
    }
}
/*!
 * \brief     Инициализация графического элемента (спец. кнопки) для СУ-S
 * \param[in] указатель на спец. кнопку
 ***********************************************************************************/
void MasterSlaveWidget::setSlaveElement(BlockBtn *master)
{
    if(slaveBtn == nullptr) {
        slaveBtn = master;
        slaveBtn->setStyleSheet(STYLE_SETTINGS);
        connect(slaveBtn, &QPushButton::clicked, [=]{
            slaveBtn->toggleDirection();
        });
    }
}
/*!
 * \brief     Активация/дизактивация элемента
 * \param[in] true - активировать, false - дизактивировать
 ***********************************************************************************/
void MasterSlaveWidget::setEnable(bool state)
{
    masterBtn->setEnabled(state);
    slaveBtn->setEnabled(state);
}
/*!
 * \brief     Получение текущего расположения СУ 02 на виджете
 * \param[in] true - активировать, false - дизактивировать
 ***********************************************************************************/
MasterSlaveWidget::Arrangment MasterSlaveWidget::getElementsArrangment() const
{
    return arrangment;
}
/* Открытые слоты */
/*!
 * \brief     Элементы СУ-М и СУ-S меняются местами (СЛОТ)
 ***********************************************************************************/
void MasterSlaveWidget::swapElementsSlot()
{
    int x = 0, y = 0, w = 0, h = 0;

    x = masterBtn->x();
    y = masterBtn->y();
    w = masterBtn->width();
    h = masterBtn->height();

    masterBtn->setGeometry(slaveBtn->x(), slaveBtn->y(), slaveBtn->height(), slaveBtn->width());
    slaveBtn->setGeometry(x, y, h, w);
}
/*!
 * \brief     Установка расположения СУ 02 на виджете (СЛОТ)
 * \param[in] идентификатор рассположения
 ***********************************************************************************/
void MasterSlaveWidget::setElementsArrangmentSlot(Arrangment arrangment)
{
    if(this->arrangment != arrangment){
        this->arrangment = arrangment;
        switch(arrangment){
            case Collinear:
              masterBtn->setOrientation(BlockBtn::Horizontal);
              slaveBtn->setOrientation(BlockBtn::Horizontal);
            break;
            case Parallel:
              masterBtn->setOrientation(BlockBtn::Vertical);
              slaveBtn->setOrientation(BlockBtn::Vertical);
            break;
            default:;
        }
    }
}
