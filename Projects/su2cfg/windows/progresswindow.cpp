#include "progresswindow.h"
#include "ui_progresswindow.h"
#include <QMessageBox>

//! \brief Строки для отображения строк во время ожидания окончания калибровки
const QString lblText[PROGRESS_MAX] =
{
  "Выполняется калибровка",
  "Выполняется калибровка.",
  "Выполняется калибровка..",
  "Выполняется калибровка...",
};
/*!
 * \brief     Конструктор класса ProgressWindow
 * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
 * \param[in] указатель на родительский виджет
 ***********************************************************************************/
ProgressWindow::ProgressWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
   // setWindowModality(Qt::ApplicationModal);

    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);

    //ui->progressBar->setTextVisible(false);

    progressStatus = PROGRESS_MIN;
    ui->progressBar->setRange(0, 99);
    ui->progressBar->setValue(0);

    progressTimer = new QTimer();
    progressTimer->setInterval(200);

    connect(progressTimer, &QTimer::timeout, [=]{
        if(progressStatus < PROGRESS_MAX)
           progressStatus++;
        else
           progressStatus = PROGRESS_MIN;

        ui->label->setText(lblText[progressStatus % PROGRESS_MAX]);
       // ui->progressBar->setValue(progressStatus);
        update();
        emit  connectionStatusRequestSignal();
        emit  requestCalibrationStatusSignal();
    });

    connect(ui->pushButton, &QPushButton::clicked, [=]{
         progressTimer->stop();
         emit cancelCalibration();
    });

    connect(this, &ProgressWindow::cancelCalibrationConfirm, this, &ProgressWindow::close);
}

/*!
 * \brief     Деструктор класса ProgressWindow
 ***********************************************************************************/
ProgressWindow::~ProgressWindow()
{
    delete ui;
}
/*!
 * \brief     Слот отображения окна
 ***********************************************************************************/
void ProgressWindow::startSlot()
{
    progressStatus = PROGRESS_MIN;

    ui->progressBar->setValue(progressStatus);
    ui->label->setText(lblText[progressStatus]);
    ui->ConnStatusLbl->setText("н/д");
    currentValue = 0;
    ui->progressBar->setValue(0);
    show();
    progressTimer->start();
}
/*!
 * \brief     Окончание калибровки
 ***********************************************************************************/
void ProgressWindow::endCalibration()
{
    QMessageBox okMsg(QMessageBox::Information, "Информация", "Калибровка окончена", QMessageBox::Ok, this, Qt::Dialog);
    currentValue = 99;
    ui->progressBar->setValue(currentValue);
    okMsg.exec();
    progressTimer->stop();
    close();
}
/*!
 * \brief     Установка статуса соединения
 * \param[in] статус: true - соединение установлено
 ***********************************************************************************/
void ProgressWindow::setConnectionStatus(bool status)
{
    if(status)
        ui->ConnStatusLbl->setText("есть");
    else
        ui->ConnStatusLbl->setText("нет");
}
/*!
 * \brief     Установка прогресса калибровки
 * \param[in] значение прогресса
 ***********************************************************************************/
void ProgressWindow::setStatus(int val)
{
    if((val > currentValue) && (ui->ConnStatusLbl->text().compare("есть") == 0)){
        currentValue = val;
        ui->progressBar->setValue(val);
    }
}
