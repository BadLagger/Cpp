#include "baseswidget.h"
#include "customdoublevalidator.h"
#include <math.h>

/*!
 * \brief  Конструктор по умолчанию
 ***********************************************************************************/
BasesWidget::BasesWidget()
{
    base1 = nullptr;
    base2 = nullptr;
    base3 = nullptr;

    // создание и инициализация таймера обновления списка портов в комбобоксе
    distanceMonitor = new QTimer(this);
    distanceMonitor->setInterval(100);
    collinearPosition = false;

    connect(distanceMonitor, &QTimer::timeout, [=]{
        QString curentDistance = mDistance->text();

        if(!curentDistance.isEmpty()){
            if(curentDistance.compare(lastDistanceStr) != 0){
                lastDistanceStr = curentDistance;
                double distanceValue = lastDistanceStr.toDouble();
                if(collinearPosition){
                    double b3Value = (0.36+DEVICE_LENGTH_DISTANCE_M)*2 + distanceValue,
                           b2Value = b3Value - 0.36;

                    base1->setText("0.36");
                    base2->setText(QString::number(b2Value, 'f', 2));
                    base3->setText(QString::number(b3Value, 'f', 2));

                } else {

                    double b2Value = distanceValue + 2*DEVICE_WIDTH_DISTANCE_M,
                           b3Value = sqrt(0.36*0.36 + b2Value*b2Value);

                    base1->setText("0.36");
                    base2->setText(QString::number(b2Value, 'f', 2));
                    base3->setText(QString::number(b3Value, 'f', 2));
                }
            }
        }
    });
}
/*!
 * \brief     Адаптер для установки графического элемента первой базы
 * \param[in] указатель на объект лайнэдита для первой базы
 ***********************************************************************************/
void BasesWidget::setBase1(QLineEdit *base)
{
    base1 = base;
    base1->setValidator(new CustomDoubleValidator(0, 99.99, 10));
}
/*!
 * \brief     Адаптер для установки графического элемента второй базы
 * \param[in] указатель на объект лайнэдита для второй базы
 ***********************************************************************************/
void BasesWidget::setBase2(QLineEdit *base)
{
    base2 = base;
    base2->setValidator(new CustomDoubleValidator(0, 99.99, 10));
}
/*!
 * \brief     Адаптер для установки графического элемента третьей базы
 * \param[in] указатель на объект лайнэдита для третьей базы
 ***********************************************************************************/
void BasesWidget::setBase3(QLineEdit *base)
{
    base3 = base;
    base3->setValidator(new CustomDoubleValidator(0, 99.99, 10));
}
/*!
 * \brief Адаптер для установки кнопки окна ввода вручную
 ***********************************************************************************/
void BasesWidget::setManualButton(QPushButton *btn)
{
    baseBtn = btn;
    baseBtn->setEnabled(false);
    connect(baseBtn, &QPushButton::clicked, [=](){
       emit manualBtnSignal();
    });
}
/*!
 * \brief Адаптер для установки чекбокса на разрешения ввода расстояния между устройствами вручную
 * \param[in] указатель на объект чекбокса
 ***********************************************************************************/
void BasesWidget::setManualCheckbox(QCheckBox *ckbox)
{
    mCheck = ckbox;
    mCheck->setEnabled(false);
    connect(mCheck, &QCheckBox::stateChanged, [=](int state){
       if(state == Qt::Checked) {
           mDistance->setEnabled(true);
           baseBtn->setEnabled(false);
           distanceMonitor->start();
       } else {
           mDistance->setEnabled(false);
           baseBtn->setEnabled(true);
           distanceMonitor->stop();
       }
    });
}
/*!
 * \brief Адаптер для установки эдита на ввода расстояния между устройствами вручную
 * \param[in] указатель на объект эдита
 ***********************************************************************************/
void BasesWidget::setManualLineedit(QLineEdit *ledit)
{
    mDistance = ledit;
    mDistance->setEnabled(false);
    mDistance->setValidator(new CustomDoubleValidator(0, 99.99, 10));
}
/*!
 * \brief     Получение значения первой базы из графического элемента
 * \return    значение в метрах
 ***********************************************************************************/
double BasesWidget::getBase1Value()
{
    return base1->text().toDouble();
}
/*!
 * \brief     Получение значения второй базы из графического элемента
 * \return    значение в метрах
 ***********************************************************************************/
double BasesWidget::getBase2Value()
{
    return base2->text().toDouble();
}
/*!
 * \brief     Получение значения третьей базы из графического элемента
 * \\return   значение в метрах
 ***********************************************************************************/
double BasesWidget::getBase3Value()
{
    return base3->text().toDouble();
}
/*!
 * \brief     Установка значения графического элемента первой базы
 * \param[in] значение в метрах
 ***********************************************************************************/
void   BasesWidget::setBase1Value(double val)
{
    base1->setText(QString::number(val, 'f', 3));
}
/*!
 * \brief     Установка значения графического элемента второй базы
 * \param[in] значение в метрах
 ***********************************************************************************/
void   BasesWidget::setBase2Value(double val)
{
    base2->setText(QString::number(val, 'f', 3));
}
/*!
 * \brief     Установка значения графического элемента третьей базы
 * \param[in] значение в метрах
 ***********************************************************************************/
void   BasesWidget::setBase3Value(double val)
{
    base3->setText(QString::number(val, 'f', 3));
}
/*!
 * \brief     Очистка значения графического элемента первой базы
 ***********************************************************************************/
void   BasesWidget::cleanBase1()
{
    base1->clear();
}
/*!
 * \brief     Очистка значения графического элемента второй базы
 ***********************************************************************************/
void   BasesWidget::cleanBase2()
{
    base2->clear();
}
/*!
 * \brief     Очистка значения графического элемента третьей базы
 ***********************************************************************************/
void   BasesWidget::cleanBase3()
{
    base3->clear();
}
/*!
 * \brief     Активировать эдиты для ввода значения баз в главном окне
 ***********************************************************************************/
void BasesWidget::setEnable()
{
    base1->setEnabled(true);
    base2->setEnabled(true);
    base3->setEnabled(true);
}
/*!
 * \brief     Деактивировать эдиты для ввода значения баз в главном окне
 ***********************************************************************************/
void BasesWidget::setDisable()
{
    base1->setEnabled(false);
    base2->setEnabled(false);
    base3->setEnabled(false);
}
/*!
 * \brief     Активировать кнопку открытия окна "Ввода баз вручную"
 ***********************************************************************************/
void BasesWidget::setBtnEnable()
{
    baseBtn->setEnabled(true);
}
/*!
 * \brief     Деактивировать кнопку открытия окна "Ввода баз вручную"
 ***********************************************************************************/
void BasesWidget::setBtnDisable()
{
    baseBtn->setEnabled(false);
}
/*!
 * \brief     Активировать чекбокс для использования ввода расстояния между устройствами
 ***********************************************************************************/
void BasesWidget::setCheckEnable()
{
    mCheck->setEnabled(true);
}
/*!
 * \brief     Деактивировать чекбокс для использования ввода расстояния между устройствами
 ***********************************************************************************/
void BasesWidget::setCheckDisable()
{
    mCheck->setEnabled(false);
    mCheck->setCheckState(Qt::Unchecked);
    mDistance->setEnabled(false);
    mDistance->clear();
}

/*!
 * \brief     Посчёт и отображение растояния между блоками на основе введённых значений баз при расположении "соосно"
 ***********************************************************************************/
void BasesWidget::setCollinearDistance()
{
    double b3Value = base3->text().toDouble(),
           b1Value = base1->text().toDouble(),
           result;

    collinearPosition = true;
    result = b3Value - (b1Value + DEVICE_LENGTH_DISTANCE_M)*2;

    if(result > 0.)
      mDistance->setText(QString::number(result, 'f', 3));
}
/*!
 * \brief     Посчёт и отображение растояния между блоками на основе введённых значений баз при расположении "параллельно"
 ***********************************************************************************/
void BasesWidget::setParallelDistance()
{
    double b2Value = base2->text().toDouble(),
           result;

    collinearPosition = false;
    result = b2Value - 2*DEVICE_WIDTH_DISTANCE_M;

    if(result > 0.)
      mDistance->setText(QString::number(result, 'f', 3));
}

/*!
 * \brief     Очистка эдита расстояния между блоками
 ***********************************************************************************/
void BasesWidget::cleanDistance()
{
    mDistance->clear();
}
