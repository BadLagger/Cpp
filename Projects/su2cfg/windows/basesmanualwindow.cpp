#include "basesmanualwindow.h"
#include "ui_basesmanualwindow.h"
#include "customs/customdoublevalidator.h"
/*!
 * \brief     Конструктор класса BasesManualWindow
 * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
 * \param[in] указатель на родительский виджет
 ***********************************************************************************/
BasesManualWindow::BasesManualWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BasesManualWindow)
{
    ui->setupUi(this);
//  Установка флагов окна
    sendSetBlock = false;
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowMinimizeButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);
// Таймер обновления окна
    updateTimer = new QTimer(this);
    updateTimer->setInterval(200);
    connect(updateTimer, &QTimer::timeout, [=]{
        if(isVisible())
          update();
        else
          updateTimer->stop();
    });
// Блокировка всех остальных окон при открытии окна
   // setWindowModality(Qt::ApplicationModal);
// Установка формата ввода для эдитов баз
    ui->base1LineEdit->setValidator(new CustomDoubleValidator(0, 99.99, 10));
    ui->base2LineEdit->setValidator(new CustomDoubleValidator(0, 99.99, 10));
    ui->base3LineEdit->setValidator(new CustomDoubleValidator(0, 99.99, 10));
// Создание окна с прогресс баром
    calibrationWindow = new ProgressWindow(this);
// Создание кнопки с предупреждением перед началом калибровки
    calibrationMsgBox= new QMessageBox(QMessageBox::Warning,
                                       "Предупреждение",
                                       "Вы уверены, что хотите выполнить калибровку?",
                                       QMessageBox::Ok | QMessageBox::Cancel, this, Qt::Dialog);
    calibrationMsgBox->button(QMessageBox::Cancel)->setText("Отмена");
// Создание окна с дополнительными настройками
    addSetsWindow = new AdditionalSetsWindow(this);
// Обработка нажатия кнопки калибровки
    connect(ui->pushButton_3, &QPushButton::clicked, [=]{

        if((ui->positionLbl->text().compare("соосно") == 0)
        || (ui->positionLbl->text().compare("соосно развёрнуты") == 0)){
            addSetsWindow->showWindow();
        } else {
            showPreCalibrationWarningWindow();
        }
    });

// Обработка нажатия кнопки установить
    connect(ui->pushButton, &QPushButton::clicked, [=]{
        BASESINFO values = {ui->positionLbl->text(),
                            ui->base1LineEdit->text().toDouble(),
                            ui->base2LineEdit->text().toDouble(),
                            ui->base3LineEdit->text().toDouble()};
        emit setBasesValues(values);
        close();
    });
// Обработка отмены калибровки от пользователя
    connect(calibrationWindow, &ProgressWindow::cancelCalibration, this, &BasesManualWindow::cancelCalibration);
// Обработка подтверждения начала калибровки от прибора
    connect(this, &BasesManualWindow::startCalibrationConfirm, calibrationWindow, &ProgressWindow::startSlot);
// Обработка подтверждения отмены калибровки от прибора
    connect(this, &BasesManualWindow::cancelCalibrationConfirm, calibrationWindow, &ProgressWindow::cancelCalibrationConfirm);
// Обработка запроса состояния связи из окна калибровки
    connect(calibrationWindow, &ProgressWindow::connectionStatusRequestSignal, this, &BasesManualWindow::connectionStatusRequestSignal);
// Обработка запроса статуса калибровки
    connect(calibrationWindow, &ProgressWindow::requestCalibrationStatusSignal, this, &BasesManualWindow::requestCalibrationStatusSignal);
// Обработка выбора типа блока в окне дополнительных настроек
    connect(addSetsWindow, &AdditionalSetsWindow::setBlockSignal, [=](AdditionalSetsWindow::BlockSets set){
        sendSetBlock = true;
        if(set == AdditionalSetsWindow::Set_Block1){
            emit requestBlockTypeSignal(false);
        } else {
            emit requestBlockTypeSignal(true);
        }
    });
// Обработка нажатия кнопки сброса настроек по умолчанию
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        if((ui->positionLbl->text().compare("параллельно") == 0)){
            ui->base1LineEdit->setText("0.36");
            ui->base2LineEdit->setText("1.02");
            ui->base3LineEdit->setText("1.07");
        } else {
            ui->base1LineEdit->setText("0.36");
            ui->base2LineEdit->setText("0.64");
            ui->base3LineEdit->setText("1.00");
        }
    });
}

/*!
 * \brief     Деструктор класса InfoWindow
 ***********************************************************************************/
BasesManualWindow::~BasesManualWindow()
{
    delete ui;
}
/*!
 * \brief     Запуск окна
 * \remark    Окно блокирует остальные окна до закрытия
 * \param[in] структура на информацию для отображения (текущее расположение и базы)
 ***********************************************************************************/
void BasesManualWindow::startWindow(BASESINFO info)
{
    ui->positionLbl->setText(info.pos);
    ui->base1LineEdit->setText(QString::number(info.base1, 'f', 3));
    ui->base2LineEdit->setText(QString::number(info.base2, 'f', 3));
    ui->base3LineEdit->setText(QString::number(info.base3, 'f', 3));
    sendSetBlock = false;
    show();
    updateTimer->start();
}
/*!
 * \brief     Окночание калибровки
 * \param[in] структура со значениями баз после автокалибровки
 ***********************************************************************************/
void BasesManualWindow::endCalibration()
{
    calibrationWindow->endCalibration();
    close();
}
/*!
 * \brief     Закрытие окна в случа возникновения ошибки
 ***********************************************************************************/
void BasesManualWindow::unexpectedShutdown()
{
    calibrationWindow->close();
    close();
}
/*!
 * \brief     Установка статуса соединения
 * \param[in] статус: true - соединение установлено
 ***********************************************************************************/
void BasesManualWindow::connectionStatus(bool status)
{
    calibrationWindow->setConnectionStatus(status);
}
/*!
 * \brief     Установка статуса калибровки
 * \param[in] 0-99 - значение статуса калибровки
 ***********************************************************************************/
void BasesManualWindow::calibrationStatus(int val)
{
    calibrationWindow->setStatus(val);
}
/*!
 * \brief     Вывод предупреждающего окна перед началом калибровки (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void BasesManualWindow::showPreCalibrationWarningWindow()
{
    int ret = calibrationMsgBox->exec();

    if(ret == QMessageBox::Ok){
        emit startCalibration();
    }
}
/*!
 * \brief     Сигнал о запросе статуса калибровки
 ***********************************************************************************/
void BasesManualWindow::setsBlockTypeConfirm()
{
    if(sendSetBlock){
        sendSetBlock = false;
        addSetsWindow->close();
        showPreCalibrationWarningWindow();
    }
}
