#include "comport.h"
#include <QTimer>
#include <QMessageBox>

/*!
 * \brief     Конструктор класса ComPort
 * \param[in] указатель на внешний комбобокс для отображения списка доступных портов
 ***********************************************************************************/
 ComPort::ComPort(QComboBox *cbox)
 {
     portlist = cbox;
     portlist->setEnabled(true);
// создание и инициализация таймера обновления списка портов в комбобоксе
     portUpdateTimer = new QTimer(this);
     portUpdateTimer->setInterval(100);
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
           portlist->clear();

           foreach(QSerialPortInfo i, portList){
               portlist->addItem(i.portName());
           }
         }
     });

     connect(portlist, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](){
         this->setPortName(portlist->currentText());
     });
// запуск таймера обновления списка портов в комбобоксе
     portUpdateTimer->start();
 }
 /*!
  * \brief     Установка комбобокса для выбора скорости порта
  * \param[in] указатель на внешний комбобокс
  ***********************************************************************************/
void  ComPort::setSpeedCbox(QComboBox *cbox)
{
    speed = cbox;
    speed->clear();
    speed->addItem("300");
    speed->addItem("600");
    speed->addItem("1200");
    speed->addItem("2400");
    speed->addItem("4800");
    speed->addItem("9600");
    speed->addItem("19200");
    speed->addItem("38400");
    speed->addItem("57600");
    speed->addItem("115200");
    speed->addItem("230400");
    speed->addItem("460800");
    speed->setEnabled(true);
    speed->setCurrentIndex(0);
    connect(speed, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](){
        this->setBaudRate(speed->currentText().toInt());
    });
}
/*!
 * \brief     Установка комбобокса для выбора чётности
 * \param[in] указатель на внешний комбобокс
 ***********************************************************************************/
void ComPort::setParityCbox(QComboBox *cbox)
{
    paritylist = cbox;
    paritylist->clear();
    paritylist->addItem("None");
    paritylist->addItem("Odd");
    paritylist->addItem("Even");
    paritylist->setEnabled(true);
    paritylist->setCurrentIndex(0);
    connect(paritylist, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        switch(index){
          case 1:  this->setParity(QSerialPort::OddParity);
            break;
          case 2:  this->setParity(QSerialPort::EvenParity);
            break;
          default: this->setParity(QSerialPort::NoParity);
        }
    });
}
/*!
 * \brief     Установка комбобокса для выбора битности
 * \param[in] указатель на внешний комбобокс
 ***********************************************************************************/
void ComPort::setBitCbox(QComboBox *cbox)
{
    bitlist = cbox;
    bitlist->clear();
    bitlist->addItem("5");
    bitlist->addItem("6");
    bitlist->addItem("7");
    bitlist->addItem("8");
    bitlist->setEnabled(true);
    bitlist->setCurrentIndex(3);
    connect(bitlist, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        switch(index){
          case 1:  this->setDataBits(QSerialPort::Data6);
            break;
          case 2:  this->setDataBits(QSerialPort::Data7);
            break;
          case 3:  this->setDataBits(QSerialPort::Data8);
            break;
          default: this->setDataBits(QSerialPort::Data5);
        }
    });
}
/*!
 * \brief     Установка комбобокса для стопбитов
 * \param[in] указатель на внешний комбобокс
 ***********************************************************************************/
void ComPort::setStopCbox(QComboBox *cbox)
{
    stoplist = cbox;
    stoplist->clear();
    stoplist->addItem("1");
    stoplist->addItem("1.5");
    stoplist->addItem("2");
    stoplist->setEnabled(true);
    stoplist->setCurrentIndex(0);

    connect(stoplist, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        switch(index){
          case 1:  this->setStopBits(QSerialPort::OneAndHalfStop);
            break;
          case 2:  this->setStopBits(QSerialPort::TwoStop);
            break;
          default: this->setStopBits(QSerialPort::OneStop);
        }
    });
}
/*!
 * \brief     Установка комбобокса для управления потоком
 * \param[in] указатель на внешний комбобокс
 ***********************************************************************************/
void ComPort::setFlowCbox(QComboBox *cbox)
{
    flowlist = cbox;
    flowlist->clear();
    flowlist->addItem("None");
    flowlist->addItem("XON/XOFF");
    flowlist->addItem("Hardware");
    flowlist->setEnabled(true);
    flowlist->setCurrentIndex(0);

    connect(flowlist, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        switch(index){
            case 1:  this->setFlowControl(QSerialPort::SoftwareControl);
            break;
            case 2:  this->setFlowControl(QSerialPort::HardwareControl);
            break;
            default: this->setFlowControl(QSerialPort::NoFlowControl);
        }
    });
}

 /*!
  * \brief  Открытие порта
 ***********************************************************************************/
 void ComPort::openPort()
 {
     QString str;
#ifdef QT_DEBUG
     str += "Port name: " + this->portName() + "\r\n";
     str += "Speed: " + QString::number(this->baudRate()) + "\r\n";
     str += "Parity: " + QString::number(this->parity()) + "\r\n";
     str += "StopBit: " +  QString::number(this->stopBits()) + "\r\n";
     str += "DataBit: " +  QString::number(this->dataBits()) + "\r\n";
     str += "FlowClrl:" + QString::number(this->flowControl()) + "\r\n";
     QMessageBox msg(QMessageBox::Warning, "Настройки", str, QMessageBox::Ok, nullptr, Qt::Dialog);
     msg.exec();
#endif
     if(this->open(QIODevice::ReadWrite)) {
         this->parity();
         portlist->setEnabled(false);
         speed->setEnabled(false);
         paritylist->setEnabled(false);
         bitlist->setEnabled(false);
         stoplist->setEnabled(false);
         flowlist->setEnabled(false);
         portUpdateTimer->stop();
         emit openSignal();
     } else {
         emit errorSignal(this->errorString());
     }
 }
 /*!
  * \brief  Закрытие порта
 ***********************************************************************************/
 void ComPort::closePort()
 {
     if(this->isOpen()){
         portlist->setEnabled(true);
         speed->setEnabled(true);
         paritylist->setEnabled(true);
         bitlist->setEnabled(true);
         stoplist->setEnabled(true);
         flowlist->setEnabled(true);
         this->close();
         portUpdateTimer->start();
         emit closeSignal();
     }
 }
