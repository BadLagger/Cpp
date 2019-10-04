#include "modecfg.h"
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
                    modeListCombox->setEnabled(true);
                    currentMode.mode = true;
                }  else {
                    modeListCombox->setCurrentIndex(0);
                    modeListCombox->setEnabled(false);
                    currentMode.mode = false;
              }
            }
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
        connect(modeListCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
           currentMode.position =  Positions(index);
           emit positionChangedSignal(Positions(index));
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

    if((cfg.mode) && (cfg.position < Position_Max)) {  //! \todo пересмотреть... присутствует избыточность
        modeListCombox->setCurrentIndex(cfg.position);
    } else {
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
}
