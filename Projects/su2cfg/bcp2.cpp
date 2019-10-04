#include "bcp2.h"
/*!
 * \brief  Конструктор по умолчанию
 ***********************************************************************************/
Bcp2::Bcp2() :Binr()
{
    port = nullptr;
    waitAnswerTimer = nullptr;
}
/*!
 * \brief     Конструктор с адаптером порта
 * \param[in] указатель на внешнюю структуру порта с которым предстоит работать
 ***********************************************************************************/
Bcp2::Bcp2(ComPort *port) :Binr()
{
    this->port = port;
    waitAnswerTimer = nullptr;
}
/*!
 * \brief     Адаптер для задания внешнего таймера ожидания ответа
 * \remark    Возможно стоит полностью инкапсулировать
 * \param[in] указатель на внешний таймер для ожидания ответа на запрос
 ***********************************************************************************/
void  Bcp2::setWaitAnswerTimer(QTimer *timer)
{
    waitAnswerTimer = timer;
}
/* Методы-запросы */
/*!
 * \brief     Запрос версии устройства
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::versionRequest(Addresses addrId)
{
    QByteArray outData;

    if(addrId < Address_Max){
        crcEnable(true);
        outData += char(1 << addrId);
        putDataInNextPkg(outData);
        outData = encode(Request_Version);
        trySendToPort(outData);
    }

    return outData;
}
/*!
 * \brief     Запрос настроек порта
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] идентификатор порта для которого запрашиваются настройки
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::portSettingsRequest(Ports portId)
{
    QByteArray outData;
    if(portId < Port_Max){
        crcEnable(true);
        outData += char(portId);
        putDataInNextPkg(outData);
        outData = encode(Request_PortSettings);
        trySendToPort(outData);
    }
    return outData;
}
/*!
 * \brief     Запрос конфигурации антенны
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::antennaCfgRequest()
{
    QByteArray outData;

    crcEnable(true);
    outData += char(6);                   // Магическое число обозначающее тип данных "Запрос конфигурации антенной системы"
    putDataInNextPkg(outData);
    outData = encode(Request_AntennaCfg);
    trySendToPort(outData);
    return outData;
}
/*!
 * \brief     Установка настроек порта
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] ссылка на структуру с настройками порта
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::setPortSettingsRequest(const PORTSETS &sets)
{
    QByteArray outData;
    if((sets.portid < Port_Max)
    && (sets.protocol < Protocol_Max)){
        crcEnable(true);
        outData += char(sets.portid);
        outData += char(sets.speed);
        outData += char(sets.speed >> 8);
        outData += char(sets.speed >> 16);
        outData += char(sets.speed >> 24);
        outData += char(sets.protocol);
        putDataInNextPkg(outData);
        outData = encode(Request_PortSettings);
        trySendToPort(outData);
    }
    return outData;
}
/*!
 * \brief     Запрос режима работы СУ 02
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::workModeRequest(Addresses addrId)
{
    QByteArray outData;

    if(addrId < Address_Max){
        crcEnable(true);
        outData += char(1 << addrId);
        putDataInNextPkg(outData);
        outData = encode(Request_Workmode);
        trySendToPort(outData);
    }

    return outData;
}
/*!
 * \brief     Установка режима работы СУ 02
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \param[in] байт режима работы
 * \todo      необходимо пересмотреть аргумент и сделать через структуру для скрытия
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::setWorkModeRequest(Addresses addrId, unsigned char mode)
{
    QByteArray outData;

    if(addrId < Address_Max){
        crcEnable(true);
        outData += char(1 << addrId);
        outData += char(20);    // Магическое число обозначающее тип данных "Установка режима работы СУ-М"
        outData += char(mode);
        putDataInNextPkg(outData);
        outData = encode(Request_Workmode);
        trySendToPort(outData);
    }
    return outData;
}
/*!
 * \brief     Установка антенной конфигурации СУ-М
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] структура антенной конфигурации
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::setAntennaCfg(ANTENNACFG &cfg)
{
    QByteArray outData;

    crcEnable(true);
    outData += char(5);                   // Магическое число обозначающее тип данных "Установка конфигурации антенной системы"
    outData += char(cfg.base1LenMm);
    outData += char(cfg.base1LenMm >> 8);
    outData += char(cfg.base1LenMm >> 16);
    outData += char(cfg.base1LenMm >> 24);
    outData += char(cfg.base2LenMm);
    outData += char(cfg.base2LenMm >> 8);
    outData += char(cfg.base2LenMm >> 16);
    outData += char(cfg.base2LenMm >> 24);
    outData += char(cfg.base3LenMm);
    outData += char(cfg.base3LenMm >> 8);
    outData += char(cfg.base3LenMm >> 16);
    outData += char(cfg.base3LenMm >> 24);
    putDataInNextPkg(outData);
    outData = encode(Request_AntennaCfg);
    trySendToPort(outData);

    return outData;
}
/* Методы-конверторы */
/*!
 * \brief     Получение строки с названием протокола по его идентификатору
 * \param[in] идентификатор протокола
 * \return    строка с названием протокола
 ***********************************************************************************/
QString       Bcp2::getProtocolStrNameById(Protocols protocolId)
{
    QString retStr = "";

    switch(protocolId){
    case Protocol_Current: retStr = "Current";
        break;
    case Protocol_Off: retStr = "Off";
        break;
    case Protocol_NMEA: retStr = "NMEA";
        break;
    case Protocol_RTCM: retStr = "RTCM";
        break;
    case Protocol_BINR: retStr = "BINR";
        break;
    case Protocol_BCP2: retStr = "BCP2";
        break;
    default: retStr = "n/a";;
    }
    return retStr;
}/*!
 * \brief     Получение идентификатора протокола по строке с названием протокола
 * \param[in] строка с названием протокола
 * \return    идентификатор протокола
 ***********************************************************************************/
Bcp2::Protocols Bcp2::getProtocolIdNameByStr(QString name)
{
    Protocols retId = Protocol_Max;
    if(!name.compare("Current"))
        retId = Protocol_Current;
    else {
        if(!name.compare("Off"))
            retId = Protocol_Off;
        else {
            if(!name.compare("NMEA"))
                retId = Protocol_NMEA;
            else {
                if(!name.compare("RTCM"))
                    retId = Protocol_RTCM;
                else {
                    if(!name.compare("BINR"))
                        retId = Protocol_BINR;
                    else {
                        if(!name.compare("BCP2"))
                           retId = Protocol_BCP2;
                    }
                }
            }
        }
    }
    return retId;
}
/* Управляющие методы */
/*!
 * \brief     Сброс внутренних состояний обработчика BCP2
 ***********************************************************************************/
void Bcp2::resetHandler()
{
    reset();
}
/*!
 * \brief     Включает внутрений обработчик для приёма данных из порта
 * \remark    Реализовано для случая когда на одном порту несколько протоколов
 ***********************************************************************************/
void Bcp2::receiverOn()
{
    if(port != nullptr){
        connect(port, &ComPort::readyRead, [=]{
            QByteArray data = port->readAll();
            emit recieveDataSignal(data);
            foreach(char i, data){
                if(decode(static_cast<unsigned char>(i))){
                    switch(getLastDecodePkgId()){
                      case Answer_PortSettings: if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit recievePortSettingsSignal(getPortSettingsFromData(getDataFromLastPkg()));
                        break;
                      case Answer_Workmode:     if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit recieveWorkmodeSignal(getDataFromLastPkg());
                        break;
                      case Answer_Version:      if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit recieveVersionSignal(getDataFromLastPkg());
                        break;
                     case Answer_AntennaCfg:    if((getDataFromLastPkg().at(0) == 5) || (getDataFromLastPkg().at(0) == 6)){
                                                  if(waitAnswerTimer)
                                                    waitAnswerTimer->stop();
                                                  emit receiveAntennaCfgSignal(getAntennaCfgFromData(getDataFromLastPkg()));
                                                }
                        break;
                    }
                }
            }
        });
    }
}
/*!
 * \brief     Выключает внутрений обработчик для приёма данных из порта
 * \remark    Реализовано для случая когда на одном порту несколько протоколов
 ***********************************************************************************/
void Bcp2::receiverOff()
{
    if(port != nullptr){
        disconnect(port, &ComPort::readyRead, nullptr, nullptr);
    }
}
/* Закрытые методы */
/*!
 * \brief      Попытка отправить массив данных в порт (ЗАКРЫТЫЙ МЕТОД)
 * \param[in]  ссылка на массив данных
 ***********************************************************************************/
void Bcp2::trySendToPort(const QByteArray &data) const
{
    if(port && port->isOpen()){
        port->write(data);
        if(waitAnswerTimer != nullptr)
            waitAnswerTimer->start();
    }
}
/*!
 * \brief     Конвертирует массив байт с настройками порта в структуру
 * \param[in] массив байт с настройками порта
 * \return    структура с настройками порта
 ***********************************************************************************/
Bcp2::PORTSETS Bcp2::getPortSettingsFromData(QByteArray data)
{
    PORTSETS retSets = {Port_Max, 0, Protocol_Max};

    if(data.at(0) < Port_Max)
    {
        retSets.portid = Ports(data.at(0));
        retSets.speed  = static_cast<unsigned long>(data.at(1)) & 0xFF;
        retSets.speed |= (static_cast<unsigned long>(int(data.at(2)) << 8) & 0xFF00);
        retSets.speed |= (static_cast<unsigned long>(int(data.at(3)) << 16) & 0xFF0000);
        retSets.speed |= (static_cast<unsigned long>(int(data.at(4)) << 24) & 0xFF000000);
        retSets.protocol = Protocols(data.at(5));
    }

    return retSets;
}
/*!
 * \brief     Конвертирует массив байт с конфигурацией антенны в структуру
 * \param[in] массив байт с настройками порта
 * \return    структура с конфигурацией антенны
 ***********************************************************************************/
Bcp2::ANTENNACFG Bcp2::getAntennaCfgFromData(QByteArray data)
{
    ANTENNACFG retCfg = {0 , 0, 0};

    retCfg.base1LenMm  = static_cast<unsigned long>(data.at(1)) & 0xFF;
    retCfg.base1LenMm |= (static_cast<unsigned long>(int(data.at(2)) << 8) & 0xFF00);
    retCfg.base1LenMm |= (static_cast<unsigned long>(int(data.at(3)) << 16) & 0xFF0000);
    retCfg.base1LenMm |= (static_cast<unsigned long>(int(data.at(4)) << 24) & 0xFF000000);

    retCfg.base2LenMm  = static_cast<unsigned long>(data.at(5)) & 0xFF;
    retCfg.base2LenMm |= (static_cast<unsigned long>(int(data.at(6)) << 8) & 0xFF00);
    retCfg.base2LenMm |= (static_cast<unsigned long>(int(data.at(7)) << 16) & 0xFF0000);
    retCfg.base2LenMm |= (static_cast<unsigned long>(int(data.at(8)) << 24) & 0xFF000000);

    retCfg.base3LenMm  = static_cast<unsigned long>(data.at(9)) & 0xFF;
    retCfg.base3LenMm |= (static_cast<unsigned long>(int(data.at(10)) << 8) & 0xFF00);
    retCfg.base3LenMm |= (static_cast<unsigned long>(int(data.at(11)) << 16) & 0xFF0000);
    retCfg.base3LenMm |= (static_cast<unsigned long>(int(data.at(12)) << 24) & 0xFF000000);

    return retCfg;
}
