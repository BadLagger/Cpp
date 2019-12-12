#ifndef BASESWIDGET_H
#define BASESWIDGET_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>

//! \brief Расстояние от центра крайней базы до края корпуса по длине устройства в метрах
#define DEVICE_LENGTH_DISTANCE_M (0.04)
//! \brief Расстояние от центра крайней базы до края корпуса по ширине устройства в метрах
#define DEVICE_WIDTH_DISTANCE_M  (0.031)

/* Описание класса */
/*!
 * \brief  Класс для управления группой графических элементов для конфигурирования антенных баз
 ***********************************************************************************/
class BasesWidget : public QObject
{
    Q_OBJECT
public:
    /* Открытые методы */
    /*!
     * \brief  Конструктор по умолчанию
     ***********************************************************************************/
    BasesWidget();
    /*!
     * \brief     Адаптер для установки графического элемента первой базы
     * \param[in] указатель на объект лайнэдита для первой базы
     ***********************************************************************************/
    void setBase1(QLineEdit *base);
    /*!
     * \brief     Адаптер для установки графического элемента второй базы
     * \param[in] указатель на объект лайнэдита для второй базы
     ***********************************************************************************/
    void setBase2(QLineEdit *base);
    /*!
     * \brief     Адаптер для установки графического элемента третьей базы
     * \param[in] указатель на объект лайнэдита для третьей базы
     ***********************************************************************************/
    void setBase3(QLineEdit *base);
    /*!
     * \brief Адаптер для установки кнопки окна ввода вручную
     ***********************************************************************************/
    void setManualButton(QPushButton *btn);
    /*!
     * \brief Адаптер для установки чекбокса на разрешения ввода расстояния между устройствами вручную
     * \param[in] указатель на объект чекбокса
     ***********************************************************************************/
    void setManualCheckbox(QCheckBox *ckbox);
    /*!
     * \brief Адаптер для установки эдита на ввода расстояния между устройствами вручную
     * \param[in] указатель на объект эдита
     ***********************************************************************************/
    void setManualLineedit(QLineEdit *ledit);
    /*!
     * \brief     Получение значения первой базы из графического элемента
     * \return    значение в метрах
     ***********************************************************************************/
    double getBase1Value();
    /*!
     * \brief     Получение значения второй базы из графического элемента
     * \return    значение в метрах
     ***********************************************************************************/
    double getBase2Value();
    /*!
     * \brief     Получение значения третьей базы из графического элемента
     * \\return   значение в метрах
     ***********************************************************************************/
    double getBase3Value();
    /*!
     * \brief     Установка значения графического элемента первой базы
     * \param[in] значение в метрах
     ***********************************************************************************/
    void   setBase1Value(double val);
    /*!
     * \brief     Установка значения графического элемента второй базы
     * \param[in] значение в метрах
     ***********************************************************************************/
    void   setBase2Value(double val);
    /*!
     * \brief     Установка значения графического элемента третьей базы
     * \param[in] значение в метрах
     ***********************************************************************************/
    void   setBase3Value(double val);
    /*!
     * \brief     Очистка значения графического элемента первой базы
     ***********************************************************************************/
    void   cleanBase1();
    /*!
     * \brief     Очистка значения графического элемента второй базы
     ***********************************************************************************/
    void   cleanBase2();
    /*!
     * \brief     Очистка значения графического элемента третьей базы
     ***********************************************************************************/
    void   cleanBase3();
    /*!
     * \brief     Активировать эдиты для ввода значения баз в главном окне
     ***********************************************************************************/
    void setEnable();
    /*!
     * \brief     Деактивировать эдиты для ввода значения баз в главном окне
     ***********************************************************************************/
    void setDisable();
    /*!
     * \brief     Активировать чекбокс для использования ввода расстояния между устройствами
     ***********************************************************************************/
    void setCheckEnable();
    /*!
     * \brief     Деактивировать чекбокс для использования ввода расстояния между устройствами
     ***********************************************************************************/
    void setCheckDisable();
    /*!
     * \brief     Посчёт и отображение растояния между блоками на основе введённых значений баз при расположении "соосно"
     ***********************************************************************************/
    void setCollinearDistance();
    /*!
     * \brief     Посчёт и отображение растояния между блоками на основе введённых значений баз при расположении "параллельно"
     ***********************************************************************************/
    void setParallelDistance();
    /*!
     * \brief     Очистка эдита расстояния между блоками
     ***********************************************************************************/
    void cleanDistance();
    /* Сигналы */
signals:
    /*!
     * \brief     Сигнал нажатия на кнопку для открытия окна "Ввода баз вручную"
     ***********************************************************************************/
    void manualBtnSignal();
    /* Открытые слоты */
public slots:
    /*!
     * \brief     Активировать кнопку открытия окна "Ввода баз вручную"
     ***********************************************************************************/
    void setBtnEnable();
    /*!
     * \brief     Деактивировать кнопку открытия окна "Ввода баз вручную"
     ***********************************************************************************/
    void setBtnDisable();

private:
    /* Закрытые члены */
    QLineEdit   *base1;            //! указатель на лайнэдит первой базы (адаптер)
    QLineEdit   *base2;            //! указатель на лайнэдит второй базы (адаптер)
    QLineEdit   *base3;            //! указатель на лайнэдит третьей базы (адаптер)
    QPushButton *baseBtn;          //! указатель на кнопку открытия окна "Ввода баз вручную" (адаптер)
    QLineEdit   *mDistance;        //! указатель на эдит для ввода расстояния между блоками (адаптер)
    QCheckBox   *mCheck;           //! указатель на чекбокс для разрешения ввода растояния между блоками (адаптер)
    QTimer      *distanceMonitor;  //! таймер для мониторинга изменения значения эдита для ввода расстояни между блоками
    QString     lastDistanceStr;   //! последняя запомненная строка эдита
    bool        collinearPosition; //! true - установлена позиция соосно, false - параллельно
};

#endif // BASESWIDGET_H
