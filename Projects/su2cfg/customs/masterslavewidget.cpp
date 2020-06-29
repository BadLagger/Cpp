#include "masterslavewidget.h"


/* Открытые методы */
/*!
 * \brief     Конструктор класса для графического отображения блоков СУ 02
 ***********************************************************************************/
MasterSlaveWidget::MasterSlaveWidget()
{
    masterBtn = nullptr;
    slaveBtn  = nullptr;
    masterActive = false;
    arrangment = MaxArrangment;
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
            if(arrangment == Parallel)
              setBlock1Active();
            else {
              setBlock2Active();
            }
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
            if(arrangment == Parallel)
              setBlock2Active();
            else {
              setBlock1Active();
            }
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
        /*if(((this->arrangment == Collinear) || (this->arrangment == CollinearTurn)) && (arrangment == Parallel) && (masterActive == true)){
            masterActive = false;
        } else {
            if(((this->arrangment == Collinear) || (this->arrangment == CollinearTurn)) && ((arrangment == Collinear) || (arrangment == CollinearTurn)) && (!masterActive)){
                masterActive = true;
            }
        }*/
        masterActive = false;
        this->arrangment = arrangment;
        switch(arrangment){
            case Collinear:
              masterBtn->setOrientation(BlockBtn::Horizontal);
              masterBtn->setDirection(BlockBtn::Left);
              slaveBtn->setOrientation(BlockBtn::Horizontal);
              slaveBtn->setDirection(BlockBtn::Left);
            break;
            case CollinearTurn:
              masterBtn->setOrientation(BlockBtn::Horizontal);
              masterBtn->setDirection(BlockBtn::Right);
              slaveBtn->setOrientation(BlockBtn::Horizontal);
              slaveBtn->setDirection(BlockBtn::Left);
            break;
            case Parallel:
             // setBlocksInnactive();
              masterBtn->setOrientation(BlockBtn::Vertical);
              masterBtn->setDirection(BlockBtn::Up);
              slaveBtn->setOrientation(BlockBtn::Vertical);
              slaveBtn->setDirection(BlockBtn::Up);
            break;
            default: setBlocksInnactive();
        }
    }
}
/*!
 * \brief     Выделить изображение первого блока
 ***********************************************************************************/
void MasterSlaveWidget::setBlock1Active()
{
    if((arrangment == CollinearTurn) || (arrangment == Collinear))
    {
        if(!masterActive){
            masterActive = true;
            masterBtn->setStyleSheet(STYLE_SETTINGS);
            slaveBtn->setStyleSheet(STYLE_SETTINGS_BOLD_BORDER);
            emit block1ActivateSignal();

        }
    } else {
        if(arrangment == Parallel){
            if(!masterActive){
                masterActive = true;
                masterBtn->setStyleSheet(STYLE_SETTINGS_BOLD_BORDER);
                slaveBtn->setStyleSheet(STYLE_SETTINGS);
                emit block1ActivateSignal();
            }
        }
    }
}
/*!
 * \brief     Выделить изображение второго блока
 ***********************************************************************************/
void MasterSlaveWidget::setBlock2Active()
{
    if((arrangment == CollinearTurn) || (arrangment == Collinear))
    {
        if(masterActive){

            masterActive = false;
            masterBtn->setStyleSheet(STYLE_SETTINGS_BOLD_BORDER);
            slaveBtn->setStyleSheet(STYLE_SETTINGS);
            emit block2ActivateSignal();
        }
    } else {
        if(arrangment == Parallel){
            if(masterActive){
                masterActive = false;
                masterBtn->setStyleSheet(STYLE_SETTINGS);
                slaveBtn->setStyleSheet(STYLE_SETTINGS_BOLD_BORDER);
                emit block2ActivateSignal();
            }
        }
    }
}
/*!
 * \brief     Снять выделение изображений обоих блоков
 ***********************************************************************************/
void MasterSlaveWidget::setBlocksInnactive()
{
    masterBtn->setStyleSheet(STYLE_SETTINGS);
    slaveBtn->setStyleSheet(STYLE_SETTINGS);
    masterActive = false;
}
