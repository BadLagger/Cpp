#include "modecfg.h"
#include <QMessageBox>
/*!
 * \brief  Конструктор по умолчанию
 ***********************************************************************************/
ModeCfg::ModeCfg()
{
    usageCheckBox  = nullptr;
    modeListCombox = nullptr;
}
/*!
 * \brief     Адаптер для задания внешнего чекбокса, который отвечает за вкл/выкл режима СУ 02
 * \param[in] указатель на объект чекбокса
 ***********************************************************************************/
void ModeCfg::setUsageCheckbox(QCheckBox    *chbox)
{
    if(chbox != nullptr) {
        usageCheckBox = chbox;
        connect(usageCheckBox, &QCheckBox::stateChanged, [=](int state){
            if(modeListCombox != nullptr){
                if(state == Qt::Checked) {
                    modeListCombox->clear();
                    modeListCombox->addItem("параллельно");
                    modeListCombox->addItem("соосно");
                    modeListCombox->addItem("соосно развёрнуты");
                    modeListCombox->setEnabled(true);
                    currentMode.mode = true;
                }  else {
                    modeListCombox->clear();
                    modeListCombox->setCurrentIndex(0);
                    modeListCombox->setEnabled(false);
                    currentMode.mode = false;
              }
            }
            emit checkBoxSignal(state);
        });
    }
}
/*!
 * \brief     Адаптер для задания внешнего комбобокса в котором содержится список вариантов размещения устройств
 * \param[in] указатель на объект комбобокса
 ***********************************************************************************/
void ModeCfg::setPositionCombox(QComboBox *cbox)
{
    if(cbox != nullptr)
    {
        modeListCombox = cbox;
        modeListCombox->clear();
        connect(modeListCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
           currentMode.position =  Positions(index);
           if((currentMode.position == Position_Collinear) || (currentMode.position == Position_Collinear_Turn) ||  (currentMode.position == Position_Parallel)){
               setEnableActiveBlockCbox();
           } else {
               setDisableActiveBlockCbox();
           }
           emit positionChangedSignal(Positions(index));
        });
    }
}
/*!
 * \brief     Адаптер для задания внешнего комбобокса в котором содержится список устройств для соосного расположения
 * \param[in] указатель на объект комбобокса
 ***********************************************************************************/
void  ModeCfg::setActiveBlockCombox(QComboBox *cbox)
{
    if(cbox != nullptr){
        activeBlockCombobox= cbox;
        currentMode.slaveForwardSign = true;
        connect(activeBlockCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
            if(activeBlockCombobox->isEnabled()){
                emit activeBlockChangedSignal(index);
            }
        });
    }
}
/*!
 * \brief     Установка режима СУ 02 для графического интерфейса
 * \param[in] ссылка на структуру с конфигурацией
 ***********************************************************************************/
void    ModeCfg::setMode(MODECFG &cfg)
{
    currentMode = cfg;

    if(cfg.mode) {
        usageCheckBox->setCheckState(Qt::Checked);
    }
    else {
        usageCheckBox->setCheckState(Qt::Unchecked);
    }

    if(cfg.mode) {
        if(cfg.position < Position_Max){
            modeListCombox->setCurrentIndex(cfg.position);
            if(cfg.position == Position_Collinear || cfg.position == Position_Collinear_Turn || cfg.position == Position_Parallel){
                setEnableActiveBlockCbox();
                if(cfg.slaveForwardSign)
                    setActiveBlock1();
                else
                    setActiveBlock2();
            }
        } else {
            QMessageBox msg(QMessageBox::Warning, "Предупреждение", "Режим работы не поддерживается", QMessageBox::Ok, nullptr, Qt::Dialog);
            msg.exec();
        }
    } else {
        /*if(cfg.position ==  Position_Max){
          QMessageBox msg(QMessageBox::Warning, "Предупреждение", "Режим работы не поддерживается", QMessageBox::Ok, nullptr, Qt::Dialog);
          msg.exec();
        }*/

        modeListCombox->setCurrentIndex(0);
        modeListCombox->setEnabled(false);
    }
}
/*!
 * \brief     Получение текущей конфигурации режима СУ 02 графического интерфейса
 * \return    структура с текущей конфигурацией
 ***********************************************************************************/
ModeCfg::MODECFG ModeCfg::getMode()
{
    return currentMode;
}
/*!
 * \brief     Деактивировать графические элементы режима СУ 02
 ***********************************************************************************/
void    ModeCfg::setDisable()
{
    if(usageCheckBox != nullptr)
        usageCheckBox->setEnabled(false);
    if(modeListCombox != nullptr)
        modeListCombox->setEnabled(false);
    if(activeBlockCombobox)
        setDisableActiveBlockCbox();
}
/*!
 * \brief     Активировать графические элементы режима СУ 02
 ***********************************************************************************/
void    ModeCfg::setEnable()
{
    if(usageCheckBox != nullptr)
        usageCheckBox->setEnabled(true);
    if(modeListCombox != nullptr)
        modeListCombox->setEnabled(true);
   /* if(activeBlockCombobox)
        setEnableActiveBlockCbox();*/
}
/*!
 * \brief     Деактивировать комбобокс блока соосного размещения
 ***********************************************************************************/
void             ModeCfg::setDisableActiveBlockCbox()
{
    activeBlockCombobox->setEnabled(false);
    activeBlockCombobox->clear();
}
/*!
 * \brief     Активировать комбобокс блока соосного размещения
 ***********************************************************************************/
void             ModeCfg::setEnableActiveBlockCbox()
{
    if(!activeBlockCombobox->isEnabled()){
        activeBlockCombobox->setEnabled(true);
        activeBlockCombobox->addItem("Блок 1");
        activeBlockCombobox->addItem("Блок 2");
        activeBlockCombobox->setCurrentIndex(0);
        currentMode.slaveForwardSign = false;
    }
}
/*!
 * \brief     Установить в комбобоксе значение первого блока
 ***********************************************************************************/
void             ModeCfg::setActiveBlock1()
{
    currentMode.slaveForwardSign = false;
    activeBlockCombobox->setCurrentIndex(0);
}
/*!
 * \brief     Установить в комбобоксе значение второго блока
 ***********************************************************************************/
void             ModeCfg::setActiveBlock2()
{
    currentMode.slaveForwardSign = true;
    activeBlockCombobox->setCurrentIndex(1);
}
