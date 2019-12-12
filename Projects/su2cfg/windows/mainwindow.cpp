#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <math.h>

#define VERSION " (01.01)"

// Таблица с настройками портов для перебора
//! \todo Пересмотреть. По хорошему должно быть константой.
static MainWindow::PORTSETS portSetsTab[MAX_PORT_SPEED_TAB_SIZE] =
{
 /* {921600, ProtocolType_BCP2},
  {921600, MainWindow::ProtocolType_NMEA},*/
  {460800, MainWindow::ProtocolType_BCP2},
  {460800, MainWindow::ProtocolType_NMEA},
  {230400, MainWindow::ProtocolType_BCP2},
  {230400, MainWindow::ProtocolType_NMEA},
  {115200, MainWindow::ProtocolType_BCP2},
  {115200, MainWindow::ProtocolType_NMEA},
  {57600, MainWindow::ProtocolType_BCP2},
  {57600, MainWindow::ProtocolType_NMEA},
  {56000, MainWindow::ProtocolType_BCP2},
  {56000, MainWindow::ProtocolType_NMEA},
  {38400, MainWindow::ProtocolType_BCP2},
  {38400, MainWindow::ProtocolType_NMEA},
  {19200, MainWindow::ProtocolType_BCP2},
  {19200, MainWindow::ProtocolType_NMEA},
  {14400, MainWindow::ProtocolType_BCP2},
  {14400, MainWindow::ProtocolType_NMEA},
  {9600, MainWindow::ProtocolType_BCP2},
  {9600, MainWindow::ProtocolType_NMEA},
  {4800, MainWindow::ProtocolType_BCP2},
  {4800, MainWindow::ProtocolType_NMEA},
  {2400, MainWindow::ProtocolType_BCP2},
  {2400, MainWindow::ProtocolType_NMEA},
  {1200, MainWindow::ProtocolType_BCP2},
  {1200, MainWindow::ProtocolType_NMEA},
  {600, MainWindow::ProtocolType_BCP2},
  {600, MainWindow::ProtocolType_NMEA},
  {300, MainWindow::ProtocolType_BCP2},
  {300, MainWindow::ProtocolType_NMEA},
  {150, MainWindow::ProtocolType_BCP2},
  {150, MainWindow::ProtocolType_NMEA},
};
/*!
 * \brief     Конструктор класса MainWindow
 * \remark    Основные связи элементов ГУИ и логики приложения находятся здесь
 * \param[in] указатель на родительский виджет
 ***********************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString title = "Конфигуратор СУ 02";
    title += VERSION;
    setWindowTitle(title);
    setWindowIcon(QIcon(":MainIcon"));
// Инициализация элементов для задания расстояния вручную
  //  ui->checkBox_2->setEnabled(false);  //! \todo заготовка
   // ui->lineEdit->setEnabled(false);    //! \todo заготовка
// Инициализация локализации в английской раскладке для работы с double с точкой вместо запятой в строковом представлении
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
// Инициализация окна подробной информациии
    infoWindow = new InfoWindow(this);
    ui->ShowInfoBtn->setEnabled(false);

    basesManualWindow = new BasesManualWindow(this);
// Инициализация чекбокса темпа решения
    ui->SolutionTempCbox->setEnabled(false);
    ui->SolutionTempCbox->clear();
    updateSolutionTempCbox = false;

    sendFloatSol[0] = false;
    sendFloatSol[1] = false;
    sendFloatSol[2] = false;
// Инициализация элементов и логики Com порта
    comPortInitialization();
// Инициализация элементов и логики BCP2
    bcp2Initialization();
// Инициализация элементов и логики NMEA
    nmeaInitialization();
// Инициализация элементов и логики спец. графических элементов для отображения устройств СУ 02
    su2GraphicalElementsInitialization();
// Инициализация элементов и логики спец. графических элементов для отображения информации о базах
    su2BasesElementsInitialization();
// Инициализация элементов и логики отладочных средств
    dbgInitialization();
}
/*!
 * \brief     Деструктор класса MainWindow
 ***********************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}
/*!
 * \brief     Установка значения комбобокса по заданной строке
 * \remark    метод находит заданную строку в списке комбобокса, и если строка найдена
 *            то устанавливает текущее значение комбобокса этой строкой
 * \param[in] указатель на комбобокс к котором будет осуществлён поиск
 * \param[in] строка которую необходимо искать в списке комбобокса и устанавливать
 ***********************************************************************************/
void MainWindow::setComboxValueByStr(QComboBox *cbox, QString str)
{
    int maxCount = cbox->count();
    for(int i = 0; i < maxCount; i++){
        if(!str.compare(cbox->itemText(i))){
            cbox->setCurrentIndex(i);
            break;
        }
    }
}
/*!
 * \brief     Попытка поменять установки порта
 * \param[in] ссылка на вспомогательную структуру порта
 ***********************************************************************************/
void MainWindow::tryChangePortSettings(PORTELEMET &port)
{
    if(((port.cbox.speed->currentText().compare(port.label.speed->text()) != 0)
    || (port.cbox.protocol->currentText().compare(port.label.protocol->text()) != 0))) {

      if(!port.cbox.speed->currentIndex()) {
          QMessageBox errMsg;
          errMsg.setText("Не выбрана скорость для " + port.name);
          errMsg.exec();
          return;
      }

      if(!port.cbox.protocol->currentIndex()) {
          QMessageBox errMsg;
          errMsg.setText("Не выбран протокол для " + port.name);
          errMsg.exec();
          return;
      }
      if(portCurrentSets->protocol == ProtocolType_BCP2){
          Bcp2::PORTSETS newPortSets;

          newPortSets.portid   = port.bcp2Id;
          newPortSets.speed    = port.cbox.speed->currentText().toULong();
          newPortSets.protocol =  bcp2->getProtocolIdNameByStr(port.cbox.protocol->currentText());
#ifdef QT_DEBUG
          inDialog->print(bcp2->setPortSettingsRequest(newPortSets));
#else
          bcp2->setPortSettingsRequest(newPortSets);
#endif
          if(port.bcp2Id == Bcp2::Debug){
              reconnectSettings.speed = newPortSets.speed;
              if(newPortSets.protocol == Bcp2::Protocol_NMEA)
                  reconnectSettings.protocol = MainWindow::ProtocolType_NMEA;
              else
                  reconnectSettings.protocol = ProtocolType_BCP2;
          }

      } else {
          NMEA_PORTSETS newPortSets;

          if(port.nmeaId == NmeaPortId_3)              // Для управляющего порта всегда устанавливать текущий, так как по NMEA может быть подключен второй порт
              newPortSets.id     = NmeaPortId_Current;
          else
            newPortSets.id       = port.nmeaId;

          newPortSets.speed    = port.cbox.speed->currentText().toULong();
          newPortSets.protocol =  nmea->getProtocolIdNameByStr(port.cbox.protocol->currentText());
#ifdef QT_DEBUG
          inDialog->print(nmea->setPortSettingsRequest(newPortSets).toUtf8());
#else
          nmea->setPortSettingsRequest(newPortSets);
#endif

          reconnectSettings.speed = newPortSets.speed;
          if(newPortSets.protocol == NmeaProtocolType_NMEA)
              reconnectSettings.protocol = MainWindow::ProtocolType_NMEA;
          else
              reconnectSettings.protocol = ProtocolType_BCP2;
      }
    }
}
/*!
 * \brief     Установка комбобоксов отвечающих за порт в активное состояние
 * \param[in] ссылка на вспомогательную структуру порта
 ***********************************************************************************/
void MainWindow::setCboxEnable(PORTELEMET &port)
{
    port.cbox.speed->setEnabled(true);
    port.cbox.protocol->setEnabled(true);
}
/*!
 * \brief     Установка комбобоксов отвечающих за порт в неактивное состояние
 * \remark    дополните
 * \param[in] ссылка на вспомогательную структуру порта
 ***********************************************************************************/
void MainWindow::setCboxDisable(PORTELEMET &port)
{
    port.cbox.speed->setEnabled(false);
    port.cbox.protocol->setEnabled(false);
    port.cbox.speed->setCurrentIndex(0);
    port.cbox.protocol->setCurrentIndex(0);
}
/*!
 * \brief     Обновление содержимого лейблов порта
 * \param[in] ссылка на вспомогательную структуру порта
 * \param[in] ссылка на структуру с информацией о настройках порта
 ***********************************************************************************/
void MainWindow::updatePortLabelContextBySets(PORTELEMET &port, Bcp2::PORTSETS &portSets)
{
    port.label.speed->setText(QString::number(portSets.speed));
    port.label.protocol->setText(bcp2->getProtocolStrNameById(portSets.protocol));
}

/* Закрытые методы */
/*!
 * \brief     Начало синхронизации (ЗАКРЫТЫЙ МЕТОД)
 * \todo      возможно необходимо пересмотреть и переделать
 ***********************************************************************************/
void MainWindow::StartSynchronization()
{
    QThread::msleep(100);
    if(portCurrentSets->protocol == ProtocolType_BCP2)
#ifdef QT_DEBUG
      inDialog->print(bcp2->versionRequest(Bcp2::Address_NAP));
    else
      inDialog->print(nmea->versionRequest().toUtf8());
#else
      bcp2->versionRequest(Bcp2::Address_NAP);
    else
      nmea->versionRequest();
#endif
    startSynchro = true;
    seekConnAttempts++;
}
/*!
 * \brief     Попытка установить настройки из таблицы настроек портов с учётом протокола (ЗАКРЫТЫЙ МЕТОД)
 * \todo      Пересмотреть. Возможно слишком много входящих параметров для закрытого метода
 * \param[in] указатель на объект порта
 * \param[in] указатель на начало таблицы настроек
 * \param[in] размер таблицы настроек
 * \param[in] индекс элемента таблицы которого необходимо установить
 * \return    true - в случае успеха, false - в случае неудачи
 ***********************************************************************************/
bool MainWindow::tryPortSets(ComPort *port, PORTSETSptr setsTab, int setsTabSize, int currentTabIndex)
{
    bool result = false;

    if((currentTabIndex < setsTabSize) && (port != nullptr) && (!port->isOpen())) {

        if(setsTab[currentTabIndex].protocol == ProtocolType_BCP2) {
#ifdef QT_DEBUG
            // переключение режима отображения дебага в 16-й вид
            outDialog->setHexView(true);
            inDialog->setHexView(true);
#endif
            port->setParitet(QSerialPort::OddParity); // Для протокола BINR/BCP2 нечет
            nmea->receiverOff();      // отключение приёма в обработчике NMEA
            bcp2->receiverOn();       // включение приёма в обработчике BCP2
            bcp2->resetHandler();
        } else {
#ifdef QT_DEBUG
            // переключение режима отображения дебага в символьный вид
            outDialog->setHexView(false);
            inDialog->setHexView(false);
#endif
            port->setParitet(QSerialPort::NoParity); // Для протокола NMEA призначка чётности нет
            bcp2->receiverOff();          // отключение приёма в обработчике BCP2
            nmea->receiverOn();           // включение приёма в обработчике NMEA
            nmea->resetHandler();
        }
        port->setBaudrate(int(setsTab[currentTabIndex].speed));

        portCurrentSets = &setsTab[currentTabIndex]; // Установка текущих настроек (считаем что если начальная проверка прошла, то настройки установлены)
        result = true;
    } else {
        // Неудачная попытка инициализации порта означает что либо порт несуществует,
        // либо закрыт либо требуемый индекс из таблицы настроек выходит за пределы таблицы
    }

    return result;
}
/*!
 * \brief     Попытка установить настройки режима работы для СУ 02 (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] структура с настройками режима работы
 ***********************************************************************************/
void MainWindow::trySetWorkMode(ModeCfg::MODECFG cfg)
{
    if(ui->ProtocolLabel->text().compare("BINR") == 0) {  // если текущий протокол BINR)
       bcp2->setWorkModeRequest(bcp2WorkModeFromModeCfg(cfg));
    } else {
       bcp2->setWorkModeRequest(Bcp2::Address_NAP, bcp2WorkModeFromModeCfg(cfg));
    }
/*#ifdef QT_DEBUG
        inDialog->print(bcp2->setWorkModeRequest(Bcp2::Address_NAP, bcp2WorkModeFromModeCfg(cfg)));
#else
        bcp2->setWorkModeRequest(Bcp2::Address_NAP, bcp2WorkModeFromModeCfg(cfg));
#endif*/

}
/*!
 * \brief     Попытка установить конфигурацию антенны для СУ 02 (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] длина первой базы в метрах
 * \param[in] длина второй базы в метрах
 * \param[in] длина третьей базы в метрах
 ***********************************************************************************/
void MainWindow::trySetAntennaCfg(double base1, double base2, double base3)
{
    Bcp2::ANTENNACFG cfg;

    cfg.base1LenMm = static_cast<unsigned long>(base1*1000);
    if(!cfg.base1LenMm)
        cfg.base1LenMm = 120;
    cfg.base2LenMm = static_cast<unsigned long>(base2*1000);
    if(!cfg.base2LenMm)
        cfg.base2LenMm = 240;
    cfg.base3LenMm = static_cast<unsigned long>(base3*1000);
    if(!cfg.base3LenMm)
        cfg.base3LenMm = 360;
#ifdef QT_DEBUG
        inDialog->print(bcp2->setAntennaCfg(cfg));
#else
        bcp2->setAntennaCfg(cfg);
#endif

}
/*!
 * \brief     Получение индекса из таблицы настроек портов согласно заданым настройкам (ЗАКРЫТЫЙ МЕТОД)
 * \todo      Пересмотреть. Возвращаемое значание не очевидно в случае неудачи
 * \param[in] ссылка на структуру с задаными настройками
 * \return    индекс из таблицы настроек, либо максимальный размер таблицы
 ***********************************************************************************/
int MainWindow::getPortSettingsTabIndex(PORTSETS &sets) const
{
    for(int i = 0; i < MAX_PORT_SPEED_TAB_SIZE; i++){
        if((portSetsTab[i].speed == sets.speed)
        && (portSetsTab[i].protocol == sets.protocol)){
           return i;
        }
    }

    return MAX_PORT_SPEED_TAB_SIZE;
}
/*!
 * \brief     Преобразует идентификатор расположения блоков из BCP2 в индекс элемента комбобокса(ЗАКРЫТЫЙ МЕТОД)
 * \param[in] идентификатор расположения блоков BCP2
 * \return    индекс элемента комбобокса
 ***********************************************************************************/
ModeCfg::Positions MainWindow::modeCfgPositionFromBcp2WorkMode(Bcp2::Workmodes id)
{
    if(!(id & Bcp2::Workmode_Free)){
        if(id & Bcp2::Workmode_Collinear){
            if(id & Bcp2::Workmode_SlaveTurn)
                return ModeCfg::Position_Collinear_Turn;
            if(id & Bcp2::Workmode_SlaveCollinear)
                return ModeCfg::Position_Collinear;
            if (id & Bcp2::Workmode_SlaveParallel)
                    return ModeCfg::Position_Parallel;

        }
    }

    return ModeCfg::Position_Max;
}
/*!
 * \brief     Извлекает из поля работы СУ 02 BCP2 признак расположения ведомого(ЗАКРЫТЫЙ МЕТОД)
 * \param[in] идентификатор расположения блоков BCP2
 * \return    true - ведомый спереди, false - ведомый сзади
 ***********************************************************************************/
bool MainWindow::modeCfgSlaveForawardSignFormWorkMode(Bcp2::Workmodes id)
{
    if(id & Bcp2::Workmode_TypeA)
        return true;
    return false;
}
/*!
 * \brief     Преобразует настройки режима работы СУ2 из структуры в байтовое поле данных для BCP2(ЗАКРЫТЫЙ МЕТОД)
 * \param[in] структура с настройками режима
 * \return    байт данных для отправки по протоколу BCP2 в составе сообщения \ref setWorkModeRequest
 ***********************************************************************************/
unsigned char MainWindow::bcp2WorkModeFromModeCfg(ModeCfg::MODECFG cfg)
{
    unsigned char ret = 0;

    if(cfg.mode)
        ret = 1;
    else
        ret = 0;

    switch(cfg.position) {
        /*case ModeCfg::Position_Free:            ret |= ((Bcp2::Workmode_SlaveParallel | Bcp2::Workmode_Free) << 1);
            break;*/
        case ModeCfg::Position_Parallel:        ret |= ((Bcp2::Workmode_Collinear | Bcp2::Workmode_SlaveParallel) << 1);
                                                if(!cfg.slaveForwardSign)
                                                  ret |= (Bcp2::Workmode_TypeA << 1);
                                                else
                                                  ret |= (Bcp2::Workmode_TypeB << 1);
            break;
        case ModeCfg::Position_Collinear:       ret |= ((Bcp2::Workmode_Collinear | Bcp2::Workmode_SlaveCollinear) << 1);
                                                if(!cfg.slaveForwardSign)
                                                    ret |= (Bcp2::Workmode_TypeA << 1);
                                                else
                                                    ret |= (Bcp2::Workmode_TypeB << 1);
            break;
        case ModeCfg::Position_Collinear_Turn:  ret |= ((Bcp2::Workmode_Collinear | Bcp2::Workmode_SlaveTurn) << 1);
                                                if(!cfg.slaveForwardSign)
                                                    ret |= (Bcp2::Workmode_TypeA << 1);
                                                else
                                                    ret |= (Bcp2::Workmode_TypeB << 1);
            break;
        default:;
    }
    return ret;
}
/*!
 * \brief     Установить значения баз в графическом интерфейсе при коллинеарном размещении
 ***********************************************************************************/
void MainWindow::setBasesCollinearValues()
{
    basesWidget.setBase1Value(0.36);
    basesWidget.setBase2Value(0.64);
    basesWidget.setBase3Value(1);
}

/*!
 * \brief     Установить значения баз в графическом интерфейсе при паралельном размещении
 ***********************************************************************************/
void MainWindow::setBasesParallelValues()
{
    basesWidget.setBase1Value(0.36);
    basesWidget.setBase2Value(1.02);
    basesWidget.setBase3Value(1.07);
}
/*!
 * \brief     Инициализация элементов и логики COM порта (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::comPortInitialization()
{
    setFixWeekCorrection = false;
// Создание  и инициализация порта
    port = new ComPort(ui->connBtn, ui->PortCb);
    port->setDisconnectBtnText("Открыть");
    port->setConnectBtnText("Закрыть");
    portSpeedIndex = 0;
// Инициализация вспомогательных структур для работы с однотипными комбобоксами и лейблами портов
    port1.name           = "порт 1";
    port1.bcp2Id         = Bcp2::Debug;
    port1.nmeaId         = NmeaPortId_3;
    port1.label.speed    = ui->StatusLabel;
    port1.label.protocol = ui->ProtocolLabel;
    port1.cbox.speed     = ui->port1SpeedCb;
    port1.cbox.protocol  = ui->port1ProtocolCb;

    port2.name           = "порт 2";
    port2.bcp2Id         = Bcp2::Port_1;
    port2.nmeaId         = NmeaPortId_1;
    port2.label.speed    = ui->port2SpeedLbl;
    port2.label.protocol = ui->port2ProtocolLbl;
    port2.cbox.speed     = ui->port2SpeedCb;
    port2.cbox.protocol  = ui->port2ProtocolCb;
// начальная инициализация выбора установок портов
    setCboxDisable(port1);
    setCboxDisable(port2);
    ui->SetBtn->setEnabled(false);
// Создание таймера ожидания ответа на запрос или команду
    waitAnswerTimer = new QTimer(this);
    waitAnswerTimer->setInterval(1000);
// Создание и инициализация таймера для перебора скоростей
    seekConnTimer = new QTimer(this);
    seekConnTimer->setInterval(200);
    seekTimerState = false;
    connSuccess    = false;
    startSynchro   = false;
    seekConnAttempts = 0;
// Обработка нажатия кнопки порта "подключить/отключить"
    connect(port, &ComPort::userClicked, [=]{
        if(!port->getState()) {
#ifdef QT_DEBUG
            ui->ReceiveTe->clear();
            ui->SendTe->clear();
            dbgPrintTimer->start();
#endif
            if(ui->portUserSpeedCbox->currentIndex() != 0){
                PORTSETS sets = {ui->portUserSpeedCbox->currentText().toULong(), ProtocolType_BCP2};
                portSpeedIndex = getPortSettingsTabIndex(sets);
            }
            tryPortSets(port, portSetsTab, MAX_PORT_SPEED_TAB_SIZE, portSpeedIndex);
        } else {
            if(seekTimerState) {
                seekConnTimer->stop();
                seekTimerState = false;
            }
#ifdef QT_DEBUG
            dbgPrintTimer->stop();
#endif
            //modeCfg->setCboxState(false);
            ModeCfg::MODECFG reset = {false, false, ModeCfg::Position_Collinear};
            modeCfg->setMode(reset);
            portSpeedIndex = 0;
          }
      });
// Обработка успешного открытия порта
      connect(port, &ComPort::openSignal, [=]{
          port1.label.speed->setText(QString::number(port->getBaudrate()));
          if(portCurrentSets->protocol == ProtocolType_BCP2)
              port1.label.protocol->setText("BCP2");
          else
              port1.label.protocol->setText("NMEA");
          //! \remark В поле  port1.label.protocol может отображаться BINR, но уже после удачного соединения и по результату ответа на запрос настроек порта. Перебор фактически ведётся только для BCP2 и NMEA.

          if(!seekTimerState) {
              seekTimerState = true;
              seekConnTimer->start();
          }

          StartSynchronization();
      });
// Обработка успешного закрытия порта
      connect(port, &ComPort::closeSignal, [=]{
          waitAnswerTimer->stop();
          connSuccess  = false;
          startSynchro = false;
          port1.label.speed->setText("нет соединения");
          port1.label.protocol->setText("н/д");
          port2.label.speed->setText("н/д");
          port2.label.protocol->setText("н/д");
 #ifdef QT_DEBUG
          ui->getBytesLbl->setText("0");
 #endif
          ui->versionLbl->setText("н/д");
          ui->SumModeLbl->setText("н/д");
          ui->SumPosLbl->setText("н/д");

          basesWidget.cleanBase1();
          basesWidget.cleanBase2();
          basesWidget.cleanBase3();
          basesWidget.setDisable();
          basesWidget.setBtnDisable();
          setCboxDisable(port1);
          setCboxDisable(port2);
          ui->SetBtn->setEnabled(false);
          modeCfg->setDisable();
          msWidget.setEnable(false);
          ui->ShowInfoBtn->setEnabled(false);
          infoWindow->close();
          basesManualWindow->unexpectedShutdown();

          infoWindow->stopInfoMonitoring();
          ui->NavSolutionLbl->setText("н/д");
          ui->SolutionTempCbox->setEnabled(false);
          //ui->SolutionTempCbox->setCurrentIndex(0);
          ui->SolutionTempCbox->clear();
          updateSolutionTempCbox = false;
          basesWidget.setCheckDisable();
          setFixWeekCorrection = false;
      });
// Обработка ошибок возникших во время работы порта
      connect(port, &ComPort::errorSignal, [=]{
          QMessageBox errorMsg;

          if(seekTimerState)
          {
             seekConnTimer->stop();
             seekTimerState = false;
          }
          portSpeedIndex = 0;

          errorMsg.setText(port->getLastErrMsg());
          errorMsg.exec();
      });
// Обработка нажатия кнопки "установить"
      connect(ui->SetBtn, &QPushButton::clicked, [=]{
          if(portCurrentSets->protocol == ProtocolType_BCP2){  // Все настройки кроме настроек управляющего порта можно устанавливать только если управляющий порт настроен на BCP2
              trySetWorkMode(modeCfg->getMode());
              QThread::msleep(100);
              tryChangePortSettings(port2);
              QThread::msleep(100);
              trySetAntennaCfg(basesWidget.getBase1Value(), basesWidget.getBase2Value(), basesWidget.getBase3Value());
              QThread::msleep(100);
              tryChangeSolutionTemp();
              QThread::msleep(100);
          }
          tryChangePortSettings(port1);  //! \warning Последовательность важна.. Смена настроек первого порта должна вызываться последней...
      });
// Обработка таймера перебора скоростей порта
      connect(seekConnTimer, &QTimer::timeout, [=]{
          bool stop = true;

          if(!connSuccess) {
              if(seekConnAttempts < SEEK_CONN_ATEMPTS_MAX){
                  StartSynchronization();
                  stop = false;
              } else {
                  seekConnAttempts = 0;
                  port->closePort();
                  portSpeedIndex++;
                  if(portSpeedIndex < MAX_PORT_SPEED_TAB_SIZE){
                      tryPortSets(port, portSetsTab, MAX_PORT_SPEED_TAB_SIZE, portSpeedIndex);
                      port->openPort();
                      stop = false;
                  }
              }
           } else {
              seekConnTimer->stop();
              seekTimerState = false;
              return;
          }

          if(stop){
              seekConnTimer->stop();
              seekTimerState = false;
              portSpeedIndex = 0;
          }
      });
}
/*!
 * \brief     Инициализация элементов и логики BCP2 (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::bcp2Initialization()
{
// Создание обработчика BCP2
    bcp2 = new Bcp2(port);
    bcp2->setWaitAnswerTimer(waitAnswerTimer); // при формировании и отправки запроса таймер будет задействован
// Обработка успешного приёма версии устройства
    connect(bcp2, &Bcp2::recieveVersionSignal, [=](QByteArray versionData){
        for(int i = 0; i < versionData.size(); i++){
            if(versionData.at(i) == 0x00)
                versionData.remove(i, 1);
            ui->versionLbl->setText(versionData.data());
        }
        connSuccess= true;
#ifdef QT_DEBUG
        inDialog->print(bcp2->portSettingsRequest(port2.bcp2Id));
        inDialog->print(bcp2->setAngleOrientationRequest(1));
#else
        bcp2->portSettingsRequest(port2.bcp2Id);
        bcp2->setAngleOrientationRequest(1);
#endif // QT_DEBUG
        setCboxEnable(port1);
        setCboxEnable(port2);
        ui->SetBtn->setEnabled(true);
        setComboxValueByStr(port1.cbox.speed,    port1.label.speed->text());
        setComboxValueByStr(port1.cbox.protocol, port1.label.protocol->text());
        infoWindow->resetAllValues();
        ui->ShowInfoBtn->setEnabled(true);
        infoWindow->startInfoMonitoring();
        ui->SolutionTempCbox->addItem("1 Гц");
        ui->SolutionTempCbox->addItem("2 Гц");
        ui->SolutionTempCbox->addItem("5 Гц");
        ui->SolutionTempCbox->addItem("10 Гц");
        ui->SolutionTempCbox->setEnabled(true);
    });
// Обработка успешного приёма настроек порта
    connect(bcp2, &Bcp2::recievePortSettingsSignal, [=](Bcp2::PORTSETS portSets){
        static bool getPort1SetsFlag = false;

        if(portSets.portid == port2.bcp2Id){
            // обработка запроса или установки скорости порта 2
            updatePortLabelContextBySets(port2, portSets);
            setComboxValueByStr(port2.cbox.speed,    port2.label.speed->text());
            setComboxValueByStr(port2.cbox.protocol, port2.label.protocol->text());
            getPort1SetsFlag = true;
#ifdef QT_DEBUG
        inDialog->print(bcp2->portSettingsRequest(port1.bcp2Id));
#else
        bcp2->portSettingsRequest(port1.bcp2Id);
#endif

        } else {
            // обработка запроса или установки скорости порта 1
            if(getPort1SetsFlag) {
                // получение настроек
                if(portCurrentSets->speed == portSets.speed) {
                  if((portCurrentSets->protocol == ProtocolType_BCP2)
                  && (portSets.protocol == Bcp2::Protocol_BINR))
                      updatePortLabelContextBySets(port1, portSets);

                }
                setComboxValueByStr(port1.cbox.speed,    QString::number(portSets.speed));
                setComboxValueByStr(port1.cbox.protocol, bcp2->getProtocolStrNameById(portSets.protocol));
                getPort1SetsFlag = false;

/*
#ifdef QT_DEBUG
                inDialog->print(bcp2->workModeRequest(Bcp2::Address_NAP));
               // inDialog->print(bcp2->workModeRequest());
#else
                bcp2->workModeRequest(Bcp2::Address_NAP);
#endif*/
                if(ui->ProtocolLabel->text().compare("BINR") == 0) {  // если текущий протокол BINR
#ifdef QT_DEBUG
                inDialog->print(bcp2->getSolutionTempRequest());
#else
                bcp2->workModeRequest();
                bcp2->getSolutionTempRequest();
#endif
                } else {
#ifdef QT_DEBUG
                  inDialog->print(bcp2->getSolutionTempRequest(Bcp2::Address_NAP));
#else
                  bcp2->workModeRequest(Bcp2::Address_NAP);
                  bcp2->getSolutionTempRequest(Bcp2::Address_NAP);
#endif
                }
            } else {
                // установка настроек
                port->closePort();
                QThread::msleep(100);
                portSpeedIndex = getPortSettingsTabIndex(reconnectSettings) - 1;
                seekConnTimer->start();
          }
        }
    });
// Обработка успешного приёма режима работы
    connect(bcp2, &Bcp2::recieveWorkmodeSignal, [=](QByteArray workModeData){
        ModeCfg::MODECFG     newModeCfg;

        newModeCfg.mode = bool(workModeData.at(1) & 0x01);
        newModeCfg.position = modeCfgPositionFromBcp2WorkMode(Bcp2::Workmodes(workModeData.at(1) >> 1));
        newModeCfg.slaveForwardSign = modeCfgSlaveForawardSignFormWorkMode(Bcp2::Workmodes(workModeData.at(1) >> 1));
        modeCfg->setEnable();
        modeCfg->setMode(newModeCfg);

#ifdef QT_DEBUG
        inDialog->print(bcp2->antennaCfgRequest());
#else
        bcp2->antennaCfgRequest();
#endif

        if(newModeCfg.mode){
            ui->SumModeLbl->setText("включён");
        }
        else {
            ui->SumModeLbl->setText("отключён");
            ui->SumPosLbl->setText("н/д");
            return;
        }

        switch(newModeCfg.position){
        case ModeCfg::Position_Collinear_Turn: ui->SumPosLbl->setText("соосно развёрнуты");
                                               if(newModeCfg.mode)
                                                 msWidget.setEnable(true);
            break;
        case ModeCfg::Position_Collinear: ui->SumPosLbl->setText("соосно");
                                          if(newModeCfg.mode)
                                              msWidget.setEnable(true);
            break;
        case ModeCfg::Position_Parallel: ui->SumPosLbl->setText("параллельно");
                                         if(newModeCfg.mode)
                                             msWidget.setEnable(true);
            break;
       /* case ModeCfg::Position_Free: ui->SumPosLbl->setText("свободно");

            break;*/
        default:  ui->SumPosLbl->setText("н/д");
        }

        basesManualWindow->setsBlockTypeConfirm();
    });
// Обработка успешного приёма установки антенной конфигурации
    connect(bcp2, &Bcp2::receiveAntennaCfgSignal, [=](Bcp2::ANTENNACFG cfg){
        basesWidget.setBase1Value(static_cast<double>(cfg.base1LenMm/1000.));
        basesWidget.setBase2Value(static_cast<double>(cfg.base2LenMm/1000.));
        basesWidget.setBase3Value(static_cast<double>(cfg.base3LenMm/1000.));
        if(modeCfg->getMode().position == ModeCfg::Position_Parallel)
            basesWidget.setParallelDistance();
        else
            if((modeCfg->getMode().position == ModeCfg::Position_Collinear) || (modeCfg->getMode().position == ModeCfg::Position_Collinear_Turn)){
                basesWidget.setCollinearDistance();
            } else
                basesWidget.cleanDistance();

    });
// Обработка успешного приёма навигационной информации
    connect(bcp2, &Bcp2::receiveVectorStateSignal, [=](Bcp2::VECTORSTATE vector){
        if(vector.status & Bcp2::Solution_Present)
        {
            ui->NavSolutionLbl->setText("есть");
            if(!setFixWeekCorrection){
#ifdef QT_DEBUG
               inDialog->print(bcp2->getTimezoneRequest());
#else
               bcp2->getTimezoneRequest();
#endif

            }
        }
        else
        {
            ui->NavSolutionLbl->setText("нет");
            setFixWeekCorrection = false;
        }
       infoWindow->fillInfoSlot(vector);
    });
// Обработка нажатия кнопки открытия окна дополнительной инфомарции
    connect(ui->ShowInfoBtn, &QPushButton::clicked, [=]{
       infoWindow->show();
    });
// Обработка отсутствия данных для окна InfoWindow
    connect(infoWindow, &InfoWindow::noInfoSignal, [=]{
#ifdef QT_DEBUG
        inDialog->print(bcp2->setVectorStateRequest(Bcp2::Address_RNPI_1, 1));
#else
        if(ui->ProtocolLabel->text().compare("BINR") == 0)  // если текущий протокол BINR
            bcp2->setVectorStateRequest(1);
        else
            bcp2->setVectorStateRequest(Bcp2::Address_RNPI_1, 1);
#endif
        ui->NavSolutionLbl->setText("н/д");

    });
// Обработка выбора темпа решения в комбобоксе пользователем
    connect(ui->SolutionTempCbox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        int temp = getSolutionTempByCboxIndex(index);
        if(temp && temp != infoWindow->getSolutionTemp()){
            updateSolutionTempCbox = true;
        } else {
            updateSolutionTempCbox = false;
        }
    });
// Обработка отсутствия ответа на запрос или команду
    connect(bcp2, &Bcp2::noAnswerTimeoutOnPkgSignal, [=](unsigned char id){
            QMessageBox msgBox;

           // if(id == 0x26) {
                port->closePort();
                portSpeedIndex = 0;
                waitAnswerTimer->stop();

                msgBox.setText("Потеряна связь с прибором.\r\nНет ответа на пакет: 0х" + QString("%1").arg(id, 0, 16).rightJustified(2, '0'));
                msgBox.exec();
           // }
     });
// Обработка получения параметра темпа решения
    connect(bcp2, &Bcp2::receiveTempSolutionSignal, [=](int temp){
        switch(temp){
          case 2:    ui->SolutionTempCbox->setCurrentIndex(1);
            break;
          case 5:    ui->SolutionTempCbox->setCurrentIndex(2);
            break;
          case 10:    ui->SolutionTempCbox->setCurrentIndex(3);
            break;
          default:   ui->SolutionTempCbox->setCurrentIndex(0);
        }
        infoWindow->setTemp(temp);
    });
// Обработка получения угловой оринетации
    connect(bcp2, &Bcp2::receiveAngleOrientationSignal, [=](Bcp2::ANGLEORIENTATION state){
        if(state.status & Bcp2::Angle_Present)
            ui->AngleSolutionLbl->setText("есть");
        else
            ui->AngleSolutionLbl->setText("нет");

        infoWindow->fillInfoSlot(state);
    });
// Обработка успешного приёма даты, времени и часового пояса
    connect(bcp2, &Bcp2::recieverTimezoneSignal, [=](Bcp2::TIMEDATATIMEZONE timezone){
       if((timezone.day != 0)
       && (timezone.mon != 0)
       && (timezone.year != 0)){
           Bcp2::DATE trueDate = {timezone.day, timezone.mon, timezone.year};
           bcp2->setFixWeekCorrection(trueDate);
           setFixWeekCorrection = true;
       }
    });
}
/*!
 * \brief     Инициализация элементов и логики NMEA (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::nmeaInitialization()
{
// Создание обработчика NMEA
    nmea = new Nmea(port);
    nmea->setWaitAnswerTimer(waitAnswerTimer); // при формировании и отправки запроса таймер будет задействован
    // Обработка успешного разбора NMEA
    //! \todo вынести в отдельный слот так как конект будет распухать со временем, а возможно будет несколько стратегий обработки
    connect(nmea, &Nmea::recieveMsgSignal, [=](NMEADATA msg){
        QStringList  dataList;
        QString msgId = nmea->getMessageId(msg.address);

        if(msgId.compare("VER") == 0) {
            waitAnswerTimer->stop();
            dataList = msg.data.split(",");
            ui->versionLbl->setText(dataList.at(1) + " " + dataList.at(2));
            connSuccess= true;
            setCboxEnable(port1);
            //  setCboxEnable(port2);
            ui->SetBtn->setEnabled(true);
            setComboxValueByStr(port1.cbox.speed,    port1.label.speed->text());
            setComboxValueByStr(port1.cbox.protocol, port1.label.protocol->text());
    /*#ifdef QT_DEBUG
                inDialog->print(nmea->getModeSettingsRequest().toUtf8());
    #else
                nmea->getModeSettingsRequest();
    #endif*/

        }

        if(msgId.compare("ORZA") == 0) {
            waitAnswerTimer->stop();
            QThread::msleep(100);
            port->closePort();
            portSpeedIndex = getPortSettingsTabIndex(reconnectSettings) - 1;
            seekConnTimer->start();
        }

        if(msgId.compare("OVSU") == 0){
            waitAnswerTimer->stop();
        }
    });
// Обработка ошибок при работе с протоколом NMEA
//! \todo Реализовать обработку либо вынести под QT_DEBUG
    connect(nmea, &Nmea::receiverErrorSignal, [](NmeaErrors code) {
        switch(code) {
          case NmeaError_MaxMsgLen:
            break;
          case NmeaError_NoAddressFrame:
            break;
          case NmeaError_WrongSymbolInAddressFrame:
            break;
          case NmeaError_WrongSymbolInDataFrame:
            break;
          case NmeaError_WrongCheckSumLen:
            break;
          case NmeaError_WrongSymbolInCheckSum:
            break;
          case NmeaError_NoCR:
            break;
          case NmeaError_NoLF:
            break;
          case NmeaError_CheckSum:
            break;
          case NmeaError_UnknownDecoderState:
            break;
          default:;
       }
    });
}
/*!
 * \brief     Инициализация элементов и логики спец. графических элементов для отображения устройств СУ 02 (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::su2GraphicalElementsInitialization()
{
// Создание и инициализация элемента "Режим работы"
    modeCfg = new ModeCfg();
    modeCfg->setUsageCheckbox(ui->checkBox);
    modeCfg->setPositionCombox(ui->comboBox);
    modeCfg->setActiveBlockCombox(ui->comboBox_2);
    modeCfg->setDisable();
    //ui->checkBox->setEnabled(true);
// Инициализация элементов графического отображения расположения блоков
    ui->pushButton->setOrientation(BlockBtn::Horizontal);
    ui->pushButton->setDirectionEnable(true);
    ui->pushButton->setDirection(BlockBtn::Left);
    ui->pushButton_2->setOrientation(BlockBtn::Horizontal);
    ui->pushButton_2->setDirectionEnable(true);
    ui->pushButton_2->setDirection(BlockBtn::Left);
// Инициализация графики
    msWidget.setMasterElement(ui->pushButton);
    msWidget.setSlaveElement(ui->pushButton_2);
    msWidget.setEnable(false);
// Обработка изменения положения графического отображения блоков устройств
    connect(modeCfg, &ModeCfg::positionChangedSignal, [=](ModeCfg::Positions id){

        basesWidget.setCheckDisable();

        if(id == ModeCfg::Position_Collinear) {  // при соосном расположении
            msWidget.setEnable(true);
            msWidget.setElementsArrangmentSlot(MasterSlaveWidget::Collinear);
            msWidget.setBlock1Active();
            setBasesCollinearValues();
            basesWidget.setCollinearDistance();
            basesWidget.setDisable();
            basesWidget.setCheckEnable();
            basesWidget.setBtnEnable();
        } else {
            if(id == ModeCfg::Position_Parallel){ // при параллельном расположении
                msWidget.setEnable(true);
                msWidget.setElementsArrangmentSlot(MasterSlaveWidget::Parallel);
                msWidget.setBlock1Active();
                setBasesParallelValues();
                basesWidget.setParallelDistance();
                basesWidget.setDisable();
                basesWidget.setCheckEnable();
                basesWidget.setBtnEnable();
            } else {
                if(id == ModeCfg::Position_Collinear_Turn){  // при соосном обратном
                    msWidget.setEnable(true);
                    msWidget.setElementsArrangmentSlot(MasterSlaveWidget::CollinearTurn);
                    msWidget.setBlock1Active();
                    setBasesCollinearValues();
                    basesWidget.setCollinearDistance();
                    basesWidget.setDisable();
                    basesWidget.setCheckEnable();
                    basesWidget.setBtnEnable();
                } else {
                   /* if(id == ModeCfg::Position_Free) {
                        basesWidget.setEnable();
                        basesWidget.setBase1Value(1.0);
                        basesWidget.setBase2Value(1.0);
                        basesWidget.setBase3Value(1.0);
                    } else {*/
                        basesWidget.cleanBase1();
                        basesWidget.cleanBase2();
                        basesWidget.cleanBase3();
                  //  }
                    basesWidget.cleanDistance();
                    msWidget.setBlocksInnactive();
                    msWidget.setEnable(false);
                }
                                                        // в остальных случаях элемент недоступен
            }
        }
    });
// Обработка активации первого блока на графическойм элементе
    connect(&msWidget, &MasterSlaveWidget::block1ActivateSignal, [=]{
        modeCfg->setActiveBlock1();
    });
// Обработка активации второго блока на графическойм элементе
    connect(&msWidget, &MasterSlaveWidget::block2ActivateSignal, [=]{
        modeCfg->setActiveBlock2();
    });
// Обработка выбора блока в комбобоксе
    connect(modeCfg, &ModeCfg::activeBlockChangedSignal, [=](int index){
       if(index == 0)
           msWidget.setBlock1Active();
       if(index == 1)
           msWidget.setBlock2Active();
    });
// Обработка чекбокса
    connect(modeCfg, &ModeCfg::checkBoxSignal, [=](int state){
        if(state == Qt::Checked){
            basesWidget.setBtnEnable();
        }  else {
            basesWidget.setDisable();
            basesWidget.setBtnDisable();
        }
    });
// Обработка нажатия кнопки "Ввод вручную"
    connect(&basesWidget, &BasesWidget::manualBtnSignal, [=]{
        BasesManualWindow::BASESINFO info = {ui->comboBox->currentText(),
                                             basesWidget.getBase1Value(),
                                             basesWidget.getBase2Value(),
                                             basesWidget.getBase3Value()};

        basesManualWindow->startWindow(info);
    });
// Обработка установки значений при вводе их в окне "Ввода баз вручную"
    connect(basesManualWindow, &BasesManualWindow::setBasesValues, [=](BasesManualWindow::BASESINFO vals){
        basesWidget.setBase1Value(vals.base1);
        basesWidget.setBase2Value(vals.base2);
        basesWidget.setBase3Value(vals.base3);
        if(modeCfg->getMode().position == ModeCfg::Position_Parallel)
            basesWidget.setParallelDistance();
        else
            if((modeCfg->getMode().position == ModeCfg::Position_Collinear) || (modeCfg->getMode().position == ModeCfg::Position_Collinear_Turn)){
                basesWidget.setCollinearDistance();
            } else
                basesWidget.cleanDistance();
    });
// Обработка запроса пользователя на начало калибровки
    connect(basesManualWindow, &BasesManualWindow::startCalibration, [=]{
        bcp2->startAutocalibrateRequest();
    });
// Обработка запроса пользователя на отмену калибровки
    connect(basesManualWindow, &BasesManualWindow::cancelCalibration, [=]{
        bcp2->cancelAutocalibrateRequest();
    });
// Обработка подтвеждения начала калибровки
    connect(bcp2, &Bcp2::receiveCalibrationStartSignal, [=]{
        emit basesManualWindow->startCalibrationConfirm();
    });
// Обработка подтверждения отмены калибровки
    connect(bcp2, &Bcp2::receiveCalibrationCancelSignal, [=]{
       emit basesManualWindow->cancelCalibrationConfirm();
    });
// Обработка окончания калибровки
    connect(bcp2, &Bcp2::receiveCalibrationOk, [=](Bcp2::CALIBRATION data){
        basesWidget.setBase1Value(static_cast<double>(data.ant.base1LenMm/1000.));
        basesWidget.setBase2Value(static_cast<double>(data.ant.base2LenMm/1000.));
        basesWidget.setBase3Value(static_cast<double>(data.ant.base3LenMm/1000.));
        if(modeCfg->getMode().position == ModeCfg::Position_Parallel)
            basesWidget.setParallelDistance();
        else{
            if((modeCfg->getMode().position == ModeCfg::Position_Collinear) || (modeCfg->getMode().position == ModeCfg::Position_Collinear_Turn)){
                basesWidget.setCollinearDistance();
            } else {
                basesWidget.cleanDistance();
            }
        }
        if(ui->ProtocolLabel->text().compare("BINR") == 0) {  // если текущий протокол BINR)
           bcp2->workModeRequest();
        } else {
           bcp2->workModeRequest(Bcp2::Address_NAP);
        }
        //bcp2->antennaCfgRequest();
        //! \todo что-то делать с данными курса, дифферента и крена
        basesManualWindow->endCalibration();
    });
// Обработка запроса статуса соединения со вторым блоком
    connect(basesManualWindow, &BasesManualWindow::connectionStatusRequestSignal, [=]{
       bcp2->testResultsRequest();
    });
// Обработка получение ответа на запрос тестов
    connect(bcp2, &Bcp2::receiveTestResultSignal, [=](Bcp2::TESTSRESULT test){
        basesManualWindow->connectionStatus(test.connection);
    });
// Обработка запроса статуса калибровки
    connect(basesManualWindow, &BasesManualWindow::requestCalibrationStatusSignal, [=]{
        //bcp2->floatSolutionRequest(/*Bcp2::SolutionBaseLine_1 | Bcp2::SolutionBaseLine_2 |*/ Bcp2::SolutionBaseLine_4);
        if(!sendFloatSol[0]){
            bcp2->floatSolutionRequest(Bcp2::SolutionBaseLine_1);
            sendFloatSol[0] = true;
        } else {
            if(!sendFloatSol[1]){
                bcp2->floatSolutionRequest(Bcp2::SolutionBaseLine_2);
                sendFloatSol[1] = true;
            } else {
                if(!sendFloatSol[2]){
                    bcp2->floatSolutionRequest(Bcp2::SolutionBaseLine_3);
                    sendFloatSol[2] = true;
                }
            }

        }
    });
// Обработка приёма данных плавающего решения
    connect(bcp2, &Bcp2::receiveFloatSolutionSignal, [=](Bcp2::FLOATSOLUTION solution){
        static bool base1 = false,
                    base2 = false,
                    base3 = false;
        static int  base1Val = 0,
                    base2Val = 0,
                    base3Val = 0;

        if(solution.numBase == 0){
            sendFloatSol[1] = false;
            if(!base1){
                base1    = true;
                base2    = false;
                if(int(solution.fisherLimite[0] * 1000) > 0){
                  base1Val = ((int(1000* solution.fisherLimite[0]) - (int(1000 * solution.fisherStatistic[0]) % int(1000* solution.fisherLimite[0]))) * 100) / int(1000* solution.fisherLimite[0]);
                  base1Val /= 3;
                } else {
                    if((int(solution.fisherLimite[0]) == 0) && (int(solution.fisherStatistic[0]) == 0)){
                        base1Val = 33;
                    }
                }
            }
        }

        if((solution.numBase == 1) && !base2){
          sendFloatSol[2] = false;
          if(!base2){
              base2    = true;
              base3    = false;
              if(int(solution.fisherLimite[0] * 1000) > 0){
                base2Val = ((int(1000* solution.fisherLimite[0]) - (int(1000 * solution.fisherStatistic[0]) % int(1000* solution.fisherLimite[0]))) * 100) / int(1000* solution.fisherLimite[0]);
                base2Val /= 3;
              } else {
                  if((int(solution.fisherLimite[0]) == 0) && (int(solution.fisherStatistic[0]) == 0)){
                      base2Val = 33;
                  }
               }
          }
        }

        if((solution.numBase == 2) && !base3){
          sendFloatSol[0] = false;
          if(!base3){
              base3    = true;
              base1    = false;
              if(int(solution.fisherLimite[0] * 1000) > 0){
                base3Val = ((int(1000* solution.fisherLimite[0]) - (int(1000 * solution.fisherStatistic[0]) % int(1000* solution.fisherLimite[0]))) * 100) / int(1000* solution.fisherLimite[0]);
                base3Val /= 3;
                basesManualWindow->calibrationStatus(base1Val + base2Val + base3Val);
              } else{
                  if((int(solution.fisherLimite[0]) == 0) && (int(solution.fisherStatistic[0]) == 0)){
                      base3Val = 33;
                      basesManualWindow->calibrationStatus(base1Val + base2Val + base3Val);
                  }
               }
          }
        }
    });
    connect(basesManualWindow, &BasesManualWindow::requestBlockTypeSignal,[=](bool sign){
        ModeCfg::MODECFG cfg = modeCfg->getMode();

        cfg.slaveForwardSign = sign;
        trySetWorkMode(cfg);
    });
// Обработка приёма данных фиксированного решения
    /*connect(bcp2, &Bcp2::receiveFixSolutionSignal, [=](Bcp2::FIXSOLUTION solution){
        int i = 0;
        i = 1;
    });*/
}
/*!
 * \brief     Инициализация элементов и логики графических элементов для отображения информации о базах (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::su2BasesElementsInitialization()
{
    basesWidget.setManualButton(ui->BaseManualBtn);
    basesWidget.setManualCheckbox(ui->checkBox_2);
    basesWidget.setManualLineedit(ui->lineEdit);
    basesWidget.setBase1(ui->Base1LineEdit);
    basesWidget.setBase2(ui->Base2LineEdit);
    basesWidget.setBase3(ui->Base3LineEdit);
    basesWidget.cleanBase1();
    basesWidget.cleanBase2();
    basesWidget.cleanBase3();
    basesWidget.setDisable();
}
/*!
 * \brief     Инициализация элементов и логики отладочного режима (ЗАКРЫТЫЙ МЕТОД)
 ***********************************************************************************/
void MainWindow::dbgInitialization()
{
// Создание вывода для пользователя
#ifdef QT_DEBUG
    outDialog = new CByteArrayTextEdit(ui->ReceiveTe);
    inDialog = new CByteArrayTextEdit(ui->SendTe);
    // Создание таймера для дебага
    dbgPrintTimer = new QTimer(this);
    dbgPrintTimer->setInterval(100);
// Обработка приёма данных из порта
    connect(bcp2, &Bcp2::recieveDataSignal, [=](QByteArray inData){
        outDialog->print(inData);
    });
    connect(nmea, &Nmea::recieveDataSignal, [=](QByteArray inData){
        outDialog->print(inData);
    });
    connect(dbgPrintTimer,  &QTimer::timeout, [=]{
        ui->getBytesLbl->setText(QString::number(outDialog->size()));
    });
#else
    ui->label_3->hide();
    ui->getBytesLbl->hide();
    ui->ReceiveGb->hide();
    ui->SendGb->hide();
    setMinimumWidth(730);
    setMaximumWidth(730);
#endif
}

/*!
 * \brief     Получение значения темпа решения по значению индекса комбобокса
 * \param[in] Значение индекса комбобкса в соответствии и индексаи SolutionTempCbox
 ***********************************************************************************/
int MainWindow::getSolutionTempByCboxIndex(int index)
{
    int temp = 0;
    switch(index) {
      case 0: temp = 1;
        break;
      case 1: temp = 2;
        break;
      case 2: temp = 5;
        break;
      case 3: temp = 10;
        break;
      default:;
    }
    return temp;
}

/*!
 * \brief     Попытка изменить настройки темпа решения (ЗАКРЫТЫЙ МЕТОД)
 ************************************************************************************/
void MainWindow::tryChangeSolutionTemp()
{
    if(updateSolutionTempCbox){

        updateSolutionTempCbox = false;
        int temp = getSolutionTempByCboxIndex(ui->SolutionTempCbox->currentIndex());
        if(temp){
          if(ui->ProtocolLabel->text().compare("BINR") == 0)  // если текущий протокол BINR
              bcp2->setSolutionTempRequest(temp);
          else
              bcp2->setSolutionTempRequest(Bcp2::Address_NAP, temp);
        }
    }
}
