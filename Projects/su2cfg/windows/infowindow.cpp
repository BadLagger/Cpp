#include "infowindow.h"
#include "ui_infowindow.h"
#include <math.h>
/*!
 * \brief     Конструктор класса InfoWindow
 * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
 * \param[in] указатель на родительский виджет (необходимо задать MainWindow)
 ***********************************************************************************/
InfoWindow::InfoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags closeFlag = Qt::WindowMinimizeButtonHint;
    flags = flags & (~closeFlag);
    setWindowFlags(flags);

    solutionTemp = 0;
    monitorRequest = false;

    updateTimer = new QTimer(this);
    updateTimer->setInterval(INFOWINDOW_UPDATE_TIME);
    updateTimer->start();

    monitorTimer = new QTimer(this);
    monitorTimer->setInterval(INFOMONITORING_TIME);

    connect(updateTimer, &QTimer::timeout, [=]{
        if(isVisible()) {
            update();
        }
    });

    connect(monitorTimer, &QTimer::timeout, [=]{
        monitorRequest = true;
        emit noInfoSignal();
    });
}
/*!
 * \brief     Деструктор класса InfoWindow
 ***********************************************************************************/
InfoWindow::~InfoWindow()
{
    updateTimer->stop();
    delete ui;
}
/*!
 * \brief     Включение мониторинга обновления информации
 * \remark    Если информация не будет обновлена с помощью слота \ref fillInfoSlot в течении
 *            \ref INFOMONITORING_TIME, то будет сгенерирован сигнал \ref noInfoSignal
 ***********************************************************************************/
void InfoWindow::startInfoMonitoring()
{
      monitorTimer->start();
}
/*!
 * \brief     Отключение мониторинга обновления информации
 ***********************************************************************************/
void InfoWindow::stopInfoMonitoring()
{
    monitorTimer->stop();
}
/*!
 * \brief     Сброс всех локальных данных (все ифнормационные поля устанавлиются в "н/д"
 *            и отключается мониторинг обновления информации)
 ***********************************************************************************/
void InfoWindow::resetAllValues()
{
    ui->LatLbl->setText("н/д");
    ui->LonLbl->setText("н/д");
    ui->AltLbl->setText("н/д");
    ui->TimeLbl->setText("н/д");
    ui->DateLbl->setText("н/д");
   // ui->SolLbl->setText("н/д");
    ui->NavStatusLbl->setText("н/д");
    ui->TempLbl->setText("н/д");
    ui->AngleStatusLbl->setText("н/д");

    monitorTimer->stop();
    solutionTemp = 0;
}
/*!
 * \brief     Интерфейс для получения значения темпа решения на основе разницы в
 *            значении данных полученных с помощью \ref fillInfoSlot
 ***********************************************************************************/
int  InfoWindow::getSolutionTemp()
{
    return solutionTemp;
}
/*!
 * \brief     Слот для обновления информации по вектору состояния навигационного решения
 * \param[in] вектор состояни навигационного решения
 ***********************************************************************************/
void InfoWindow::fillInfoSlot(Bcp2::VECTORSTATE vector)
{
    static int prevMsecValue = 0;
    int        currMsecValue = vector.time.msec;

    //! Посчёт темпа решения
    if(!monitorRequest){
        if(prevMsecValue >= vector.time.msec){
            currMsecValue += 1000;
        }

        int msSub = (currMsecValue - prevMsecValue);
        if(msSub > 0){
            if(msSub < 1000) {
                solutionTemp = 1000 / msSub;
            } else {
                solutionTemp = 1;
            }
        } else
            solutionTemp = 0;
         //ui->SolLbl->setText(QString::number(solutionTemp) + " Гц");
    } else {
         //ui->SolLbl->setText("н/д");
          monitorRequest = false;
    }

    prevMsecValue = vector.time.msec;

    //! Заполнение окна дополнительной информации значениями
    if(isVisible())
    {
        ui->NavStatusLbl->setText(QString("0x%1").arg(vector.status, 0, 16).rightJustified(2, '0'));
        ui->LatLbl->setText(getGradStringFromDouble(vector.latitude));
        ui->LonLbl->setText(getGradStringFromDouble(vector.longitude));
        ui->AltLbl->setText(QString::number(vector.altitude, 'f', 1) + " м");
        ui->TimeLbl->setText(QString::number(vector.time.hour).rightJustified(2, '0') + ":"
                           + QString::number(vector.time.min).rightJustified(2, '0') + ":"
                           + QString::number(vector.time.sec).rightJustified(2, '0') + ":"
                           + QString::number(vector.time.msec).rightJustified(3, '0'));
        ui->DateLbl->setText(QString::number(vector.date.day).rightJustified(2, '0') + "."
                           + QString::number(vector.date.mon).rightJustified(2, '0') + "."
                           + QString::number(vector.date.year).rightJustified(4, '0'));
    }

    monitorTimer->start();
}

/*!
 * \brief     Слот для обновления информации по угловому решению
 * \param[in] структура с угловой оринетацией
 ***********************************************************************************/
void InfoWindow::fillInfoSlot(Bcp2::ANGLEORIENTATION state)
{
    //! Заполнение окна дополнительной информации значениями
    if(isVisible())
    {
      ui->AngleStatusLbl->setText(QString("0x%1").arg(state.status, 0, 16).rightJustified(2, '0'));

      ui->CourseLbl->setText(QString::number(static_cast<double>(state.course), 'f', 2).rightJustified(5, '0') + QChar(176));
      ui->PitchLbl->setText(QString::number(static_cast<double>(state.pitch), 'f', 2).rightJustified(5, '0') + QChar(176));
      ui->RollLbl->setText(QString::number(static_cast<double>(state.roll), 'f', 2).rightJustified(5, '0') + QChar(176));
    }
}
/*!
 * \brief     Интерфейс для установки темпа решения
 * \param[in] значение темпа в герцах
 ***********************************************************************************/
void InfoWindow::setTemp(int temp)
{
    ui->TempLbl->setText(QString::number(temp) + " Гц");
}
/*!
 * \brief     Получение значения гладусов в строковом виде по вещественному значению(ЗАКРЫТЫЙ МЕТОД)
 * \param[in] значение градусов в вещественном виде
 * \return    строка со значением градусов для вывода
 ***********************************************************************************/
QString InfoWindow::getGradStringFromDouble(double val){
    int    grad = int(val);
    double min = (val  - grad)*60;

    if(grad < 0)
        min *= (-1);

    return QString(QString::number(grad).rightJustified(2, '0') + QChar(176) + QString::number(min, 'f', 4).rightJustified(7, '0'));
}
