#include "comport.h"
#include <QTimer>

/*!
 * \brief     Конструктор класса ComPort
 * \param[in] пользовательская кнопка отвечающая за подключение/отключение порта
 * \param[in] пользовательский комбобокс отвечающий за отображения списка доступных портов
 ***********************************************************************************/
 ComPort::ComPort(QPushButton *userBtn, QComboBox *userCbox)
 {
     state            = false;       // по умолчанию порт отключен
     connectBtn       = userBtn;     //! \remark Отсутствует провека указателя, расчитано на использование "со здравым смыслом"
     portListCbox     = userCbox;    //! \remark Отсутствует провека указателя, расчитано на использование "со здравым смыслом"
// начальная инициализация надписей кнопок
     setConnectBtnText("Disconnect");
     setDisconnectBtnText("Connect");
// начальная инициализация строки с информацией об ошибке
     lastErrorStr = "No errors";
// создание и инициализация таймера обновления списка портов в комбобоксе
     portUpdateTimer = new QTimer(this);
     portUpdateTimer->setInterval(100);
// инициализация порта по умолчанию
     dataBits    = QSerialPort::Data8;
     baudRate    = 9600;
     flowControl = QSerialPort::NoFlowControl;
     parity      = QSerialPort::NoParity;
// обработка нажатия кнопки подключить/отключить
     connect(connectBtn, &QPushButton::clicked, [=]{
         emit userClicked(); // Сигнал о нажатии кнопки пользователем
         if(!state) {  // Включение порта
             openPort();
         }  else {     // Выключение порта
             closePort();
         }
     });
// Обработка обновления списка портов в комбобоксе
     connect(portUpdateTimer, &QTimer::timeout, [=]{
         bool update = false;   // флаг для обновления спика портов в комбобоксе

         QList<QSerialPortInfo> currentPortList = QSerialPortInfo::availablePorts();

         if(currentPortList.count() != portList.count()) {
             update = true;      // необходимо обновить список если количество текущих и запомненных портов не совпадает
         } else {
             for(int i = 0; i < currentPortList.count(); i++)   // Проверка не поменялись ли имена портов, и если поменялись то нужно обновить
             {
               if((portList.at(i)).portName().compare((currentPortList.at(i)).portName()) != 0)
                   update = true;
             }
         }

         if(update) {   // Обновление спика комбобокса
           portList = currentPortList;
           portListCbox->clear();

           foreach(QSerialPortInfo i, portList){
               portListCbox->addItem(i.portName());
           }
         }
     });
// запуск таймера обновления списка портов в комбобоксе
     portUpdateTimer->start();
 }
/*!
 * \brief Установка текста для отображения на кнопке при НЕАКТИВНОМ подключении
 * \param[in] строка текста для отображения на кнопке
 ***********************************************************************************/
 void  ComPort::setDisconnectBtnText(const QString str)
 {
     disconnectBtnTxt = str;
     if(!state)
         connectBtn->setText(disconnectBtnTxt);
 }
 /*!
  * \brief Установка текста для отображения на кнопке при АКТИВНОМ подключении
  * \param[in] строка текста для отображения на кнопке
 ***********************************************************************************/
 void  ComPort::setConnectBtnText(const QString str)
 {
     connectBtnTxt = str;
     if(state)
         connectBtn->setText(connectBtnTxt);
 }
 /*!
  * \brief      Установка настройки длины слова порта (битность)
  * \remark     Использовать вместо setDataBits
  * \param[in]  битность
 ***********************************************************************************/
 void ComPort::setDatabits(QSerialPort::DataBits bits)
 {
     if(!state){
         dataBits = bits;
     } else {
       lastErrorStr = "Перед изменением настройки битности порта необходимо закрыть порт";
       emit errorSignal();
     }
 }
 /*!
  * \brief      Установка настройки скорости порта
  * \remark     Использовать вместо setBaudRate
  * \param[in]  скорость
 ***********************************************************************************/
 void ComPort::setBaudrate(int brate)
 {
     if(!state){
         baudRate = brate;
     } else {
       lastErrorStr = "Перед изменением настройки скорости порта необходимо закрыть порт";
       emit errorSignal();
     }
 }
 /*!
  * \brief      Установка настройки управления потоком порта
  * \remark     Использовать вместо setFlowControl
  * \param[in]  признак управления потоком
 ***********************************************************************************/
 void ComPort::setFlowcontol(QSerialPort::FlowControl flowctrl)
 {
     if(!state){
         flowControl = flowctrl;
     } else {
       lastErrorStr = "Перед изменением настройки управления потока порта необходимо закрыть порт";
       emit errorSignal();
     }
 }
 /*!
  * \brief      Установка настройки чет/нечет порта
  * \remark     Использовать вместо setParity
  * \param[in]  признак управления потоком
 ***********************************************************************************/
 void ComPort::setParitet(QSerialPort::Parity parity)
 {
     if(!state){
         this->parity = parity;
     } else {
       lastErrorStr = "Перед изменением настройки чет/нечет порта необходимо закрыть порт";
       emit errorSignal();
     }
 }
 /*!
  * \brief  Открытие порта
 ***********************************************************************************/
 void ComPort::openPort()
 {
     state = true;
     connectBtn->setText(connectBtnTxt);
     portUpdateTimer->stop();
     portListCbox->setEnabled(false);
     tryToConnect();
 }
 /*!
  * \brief  Закрытие порта
 ***********************************************************************************/
 void ComPort::closePort()
 {
     state = false;
     connectBtn->setText(disconnectBtnTxt);
     portListCbox->setEnabled(true);
     portUpdateTimer->start();
     if(this->isOpen()) {
         this->close();
     };
     emit closeSignal();
 }
 /*!
  * \brief      Получение текущего значения битности данных порта
  * \return     битность
 ***********************************************************************************/
 QSerialPort::DataBits ComPort::getDatabits() const
 {
     return dataBits;
 }
 /*!
  * \brief      Получение текущего значения скорости порта
  * \return     скорость
 ***********************************************************************************/
 int ComPort::getBaudrate() const
 {
     return baudRate;
 }
 /*!
  * \brief      Получение текущего настройки управления потоком порта
  * \return     признак управления потоком
 ***********************************************************************************/
 QSerialPort::FlowControl ComPort::getFlowcontrol() const
 {
     return flowControl;
 }
 /*!
  * \brief      Получение текущего значения чет/нечет порта
  * \return     битность
 ***********************************************************************************/
 QSerialPort::Parity ComPort::getParitet() const
 {
     return parity;
 }
 /*!
  * \brief  Получение текущего состояния подключения
  * \return true - если порт подключен, false - если порт отключен
 ***********************************************************************************/
 bool  ComPort::getState() const
 {
     return state;
 }
 /*!
  * \brief  Получение информации о последней возникшей ошибки
  * \return текст с описанием проблемы
 ***********************************************************************************/
 QString ComPort::getLastErrMsg() const
 {
     return lastErrorStr;
 }
 /*!
  * \brief  Попытка установить подключение (Закрытый метод)
 ***********************************************************************************/
 void ComPort::tryToConnect()
 {
     if(!portListCbox->currentText().isEmpty()) {
         this->setPortName(portListCbox->currentText());
         this->setDataBits(dataBits);
         this->setBaudRate(baudRate);
         this->setFlowControl(flowControl);
         this->setParity(parity);
         if(this->open(QIODevice::ReadWrite)) {
             emit openSignal();
         }
         else {
             lastErrorStr = "Ошибка открытия порта: " + this->errorString();
             emit errorSignal();
             closePort();
         }
     } else {
         lastErrorStr = "Порт не выбран";
         emit errorSignal();
         closePort();
     }
 }
