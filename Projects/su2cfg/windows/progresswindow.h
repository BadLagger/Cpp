#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QMainWindow>
#include <QTimer>

//! \brief Начальное состояние прогресса
#define PROGRESS_MIN (0)
//! \brief Максимальное состояние прогресса
#define PROGRESS_MAX (4)

namespace Ui {
class ProgressWindow;
}
/* Описание класса */
/*!
 * \brief Класс окна ожидания окончания калибровки
 ***********************************************************************************/
class ProgressWindow: public QMainWindow
{
    Q_OBJECT
public:
    /* Открытые методы */
    /*!
     * \brief     Конструктор класса ProgressWindow
     * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
     * \param[in] указатель на родительский виджет
     ***********************************************************************************/
     explicit ProgressWindow(QWidget *parent = nullptr);
    /*!
     * \brief     Деструктор класса ProgressWindow
     ***********************************************************************************/
     ~ProgressWindow();
    /*!
     * \brief     Окончание калибровки
     ***********************************************************************************/
    void endCalibration();
    /*!
     * \brief     Установка статуса соединения
     * \param[in] статус: true - соединение установлено
     ***********************************************************************************/
    void setConnectionStatus(bool status);
    /*!
     * \brief     Установка прогресса калибровки
     * \param[in] значение прогресса
     ***********************************************************************************/
    void setStatus(int val);
    /* Открытые слоты */
public slots:
    /*!
     * \brief     Слот отображения окна
     ***********************************************************************************/
    void startSlot();
signals:
    /* Сигналы */
    /*!
     * \brief     Сигнал на запрос отмены калибровки
     ***********************************************************************************/
    void cancelCalibration();
    /*!
     * \brief     Сигнал подтверждения отмены калибровки
     ***********************************************************************************/
    void cancelCalibrationConfirm();
    /*!
     * \brief     Сигнал запроса статуса соединения со сторым блоком
     ***********************************************************************************/
    void connectionStatusRequestSignal();
    /*!
     * \brief     Сигнал запроса статуса калибровки
     ***********************************************************************************/
    void requestCalibrationStatusSignal();
private:
    /* Закрытые члены */
    Ui::ProgressWindow *ui;    //! указатель на форму окна
    int progressStatus;       //! состояние прогресса от PROGRESS_MIN до PROGRESS_MAX
    QTimer *progressTimer;     //! таймер обновления элементов окна
    int  currentValue;
};

#endif // PROGRESSWINDOW_H