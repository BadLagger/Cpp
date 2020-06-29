#include "bcp2.h"
#include <math.h>

/*!
 * \brief  Конструктор по умолчанию
 ***********************************************************************************/
Bcp2::Bcp2() :Binr()
{
    port = nullptr;
    waitAnswerTimer = nullptr;
    fixweek = 2046;
}
/*!
 * \brief     Конструктор с адаптером порта
 * \param[in] указатель на внешнюю структуру порта с которым предстоит работать
 ***********************************************************************************/
Bcp2::Bcp2(ComPort *port) :Binr()
{
    this->port = port;
    waitAnswerTimer = nullptr;
    fixweek = 2046;
}
/*!
 * \brief     Адаптер для задания внешнего таймера ожидания ответа
 * \remark    Возможно стоит полностью инкапсулировать
 * \param[in] указатель на внешний таймер для ожидания ответа на запрос
 ***********************************************************************************/
void  Bcp2::setWaitAnswerTimer(QTimer *timer)
{
    waitAnswerTimer = timer;
    // Обработка отсутствия ответа на запрос или команду
    connect(waitAnswerTimer, &QTimer::timeout, [=]{
        emit noAnswerTimeoutOnPkgSignal(getLastEncodeId());
    });
}
/* Методы-запросы */
/*!
 * \brief     Запрос версии устройства по BCP2
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
 * \brief     Запрос версии устройства по BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray Bcp2::versionRequest()
{
    QByteArray outData;

    crcEnable(true);
    putDataInNextPkg(outData);
    outData = encode(Request_Version);
    trySendToPort(outData);

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
  //  outData += char(1 << Address_NAP);
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
 * \brief     Запрос режима работы BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray Bcp2::workModeRequest()
{
    QByteArray outData;

    //if(addrId < Address_Max){
        crcEnable(true);
    //    outData += char(1 << addrId);
        putDataInNextPkg(outData);
        outData = encode(Request_Workmode);
        trySendToPort(outData);
   // }

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
 * \brief     Установка режима работы СУ 02
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \param[in] байт режима работы
 * \todo      необходимо пересмотреть аргумент и сделать через структуру для скрытия
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::setWorkModeRequest(unsigned char mode)
{
    QByteArray outData;

   // if(addrId < Address_Max){
        crcEnable(true);
       // outData += char(1 << addrId);
        outData += char(20);    // Магическое число обозначающее тип данных "Установка режима работы СУ-М"
        outData += char(mode);
        putDataInNextPkg(outData);
        outData = encode(Request_Workmode);
        trySendToPort(outData);
  //  }
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
/*!
 * \brief     Запрос выдачи вектора состояния BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \param[in] темп выдачи вектора состояния в итервалах решения
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray Bcp2::setVectorStateRequest(Addresses addrId, unsigned char temp)
{
    QByteArray outData;
    if(addrId < Address_Max){
        crcEnable(true);
        outData += char(1 << addrId);
        outData += char(temp);
        putDataInNextPkg(outData);
        outData = encode(Request_VectorState);
        trySendToPort(outData);
    }

    return outData;
}
/*!
 * \brief     Запрос выдачи вектора состояния BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] темп выдачи вектора состояния в итервалах решения
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray Bcp2::setVectorStateRequest(unsigned char temp)
{
    QByteArray outData;
    crcEnable(true);
    outData += char(temp);
    putDataInNextPkg(outData);
    outData = encode(Request_VectorState);
    trySendToPort(outData);

    return outData;
}

/*!
 * \brief     Установка темпа решения BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] темп решения (1, 2, 5, 10)
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray    Bcp2::setSolutionTempRequest(int temp)
{
    QByteArray outData;
    crcEnable(true);
    outData += 0x02;          // Тип данных указывающий что опция отвечает за Темп решения
    outData += char(temp);
    putDataInNextPkg(outData);
    outData = encode(Request_SetOptions);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Установка темпа решения BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \param[in] темп решения (1, 2, 5, 10)
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::setSolutionTempRequest(Addresses index, int temp)
{
    QByteArray outData;
    crcEnable(true);
    outData += char(1 << index);
    outData += 0x02;          // Тип данных указывающий что опция отвечает за Темп решения
    outData += char(temp);
    putDataInNextPkg(outData);
    outData = encode(Request_SetOptions);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Получение темпа решения для протокола BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray    Bcp2::getSolutionTempRequest()
{
    QByteArray outData;
    crcEnable(true);
    outData += 0x02;          // Тип данных указывающий что опция отвечает за Темп решения
    putDataInNextPkg(outData);
    outData = encode(Request_SetOptions);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Получение темпа решения для протокола BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::getSolutionTempRequest(Addresses index)
{
    QByteArray outData;
    crcEnable(true);
     outData += char(1 << index);
    outData += 0x02;             // Тип данных указывающий что опция отвечает за Темп решения
    putDataInNextPkg(outData);
    outData = encode(Request_SetOptions);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос угловой ориентации для протокола BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] адрес устройства для которого формируется запрос
 * \param[in] темп выдачи угловой ориентации в итервалах решения
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::setAngleOrientationRequest(Addresses addrId, unsigned char temp)
{
    QByteArray outData;
    if(addrId < Address_Max){
        crcEnable(true);
        outData += char(1 << addrId);
        outData += char(temp);
        putDataInNextPkg(outData);
        outData = encode(Request_AngleOrientation);
        trySendToPort(outData);
    }

    return outData;
}
/*!
 * \brief     Запрос угловой ориентации для протокола BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \param[in] темп выдачи угловой ориентации в итервалах решения
 * \return    массив байтов со сформированным сообщением BINR
 ***********************************************************************************/
QByteArray    Bcp2::setAngleOrientationRequest(unsigned char temp)
{
    QByteArray outData;
    crcEnable(true);
    outData += char(temp);
    putDataInNextPkg(outData);
    outData = encode(Request_AngleOrientation);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос даты, времени и часового пояса BCP2/BINR
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2/BINR
 ***********************************************************************************/
QByteArray    Bcp2::getTimezoneRequest()
{
    QByteArray outData;
    crcEnable(true);
    outData = encode(Request_Timezone);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос на начало автокалибровки BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::startAutocalibrateRequest()
{
    QByteArray outData;
    crcEnable(true);
    outData += 0x01;          // Тип данных указывающий на запуск автокалибровки
    putDataInNextPkg(outData);
    outData = encode(Request_AntennaCfg);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос на отмену автокалибровки BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::cancelAutocalibrateRequest()
{
    QByteArray outData;
    crcEnable(true);
    outData += 0x02;           // Тип данных указывающий на отмену автокалибровки
    putDataInNextPkg(outData);
    outData = encode(Request_AntennaCfg);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос результатов тестирования BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::testResultsRequest()
{
    QByteArray outData;
    crcEnable(true);
     outData += QChar(0);           // запрос
    putDataInNextPkg(outData);
    outData = encode(Request_Test);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос плавающего решения для базовой линии по BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::floatSolutionRequest(unsigned char bases)
{
    QByteArray outData;
    crcEnable(true);
    outData += char(bases);           // запрос
    putDataInNextPkg(outData);
    outData = encode(Request_FloatSolution);
    trySendToPort(outData);

    return outData;
}
/*!
 * \brief     Запрос фиксированного решения для базовой линии по BCP2
 * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
 * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
 * \return    массив байтов со сформированным сообщением BCP2
 ***********************************************************************************/
QByteArray    Bcp2::fixSolutionRequest(int i)
{
    QByteArray outData;
    crcEnable(true);
    outData += char(i);           // запрос
    putDataInNextPkg(outData);
    outData = encode(Request_FixSolution);
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
}
/*!
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
/*!
 * \brief     Получение строкового названия состояния по байту статуса
 * \return    строка с описанием состояния
 ***********************************************************************************/
QString       Bcp2::getSolutionStr(unsigned char status)
{
    if(status & Solution_Present){
        if(status & Solution_2D)
            return "2D";
        if(status & Solution_RAIM)
            return "RAIM";
        return "решение есть";
    }
    return "н/д";
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
                      case Answer_Test:
                                                if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit receiveTestResultSignal(getTestResultFromData(getDataFromLastPkg()));
                        break;
                      case Answer_Timezone:     if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit recieverTimezoneSignal(getTimezoneFromData(getDataFromLastPkg()));
                        break;
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
                     case Answer_VectorState:   if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                emit receiveVectorStateSignal(getVectorStateFromData(getDataFromLastPkg()));
                        break;
                     case Answer_AntennaCfg:    if((getDataFromLastPkg().at(0) == 5) || (getDataFromLastPkg().at(0) == 6)){
                                                  if(waitAnswerTimer)
                                                    waitAnswerTimer->stop();
                                                  emit receiveAntennaCfgSignal(getAntennaCfgFromData(getDataFromLastPkg()));
                                                 } else {
                                                   if((getDataFromLastPkg().at(0) == 0)){        // окончание калибровки
                                                       emit receiveCalibrationOk(getCalibrationFromData(getDataFromLastPkg()));
                                                   } else if((getDataFromLastPkg().at(0) == 1)){ // начало автокалибровки
                                                       emit receiveCalibrationStartSignal();
                                                   } else if((getDataFromLastPkg().at(0) == 2)){ // отмена автокалибровки
                                                       emit receiveCalibrationCancelSignal();
                                                   }
                                                 }
                        break;
                     case Answer_Options:       if(waitAnswerTimer)
                                                  waitAnswerTimer->stop();
                                                if((getDataFromLastPkg().at(0) == 2)){ // Темп решения
                                                    emit receiveTempSolutionSignal((getDataFromLastPkg().at(1)));
                                                }
                        break;
                    case Answer_AngleOrientation: if(waitAnswerTimer)
                                                    waitAnswerTimer->stop();
                                                  emit receiveAngleOrientationSignal(getAngleOrientationFromData(getDataFromLastPkg()));
                        break;
                    case Answer_FloatSolution:    if(waitAnswerTimer)
                                                    waitAnswerTimer->stop();
                                                  emit receiveFloatSolutionSignal(getFloatSolutionFromData(getDataFromLastPkg()));
                        break;
                    case Answer_FixSolution:      if(waitAnswerTimer)
                                                    waitAnswerTimer->stop();
                                                  emit receiveFixSolutionSignal(getFixSolutionFromData(getDataFromLastPkg()));
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
        retSets.speed  = TO_ULONG(data.at(1)) & 0xFF;
        retSets.speed |= (TO_ULONG(int(data.at(2)) << 8) & 0xFF00);
        retSets.speed |= (TO_ULONG(int(data.at(3)) << 16) & 0xFF0000);
        retSets.speed |= (TO_ULONG(int(data.at(4)) << 24) & 0xFF000000);
        retSets.protocol = Protocols(data.at(5));
    }

    return retSets;
}
/*!
 * \brief     Конвертирует массив байт с конфигурацией антенны в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с конфигурацией антенны
 * \return    структура с конфигурацией антенны
 ***********************************************************************************/
Bcp2::ANTENNACFG Bcp2::getAntennaCfgFromData(QByteArray data)
{
    ANTENNACFG retCfg = {0 , 0, 0};

    retCfg.base1LenMm  = TO_ULONG(data.at(1)) & 0xFF;
    retCfg.base1LenMm |= (TO_ULONG(int(data.at(2)) << 8) & 0xFF00);
    retCfg.base1LenMm |= (TO_ULONG(int(data.at(3)) << 16) & 0xFF0000);
    retCfg.base1LenMm |= (TO_ULONG(int(data.at(4)) << 24) & 0xFF000000);

    retCfg.base2LenMm  = TO_ULONG(data.at(5)) & 0xFF;
    retCfg.base2LenMm |= (TO_ULONG(int(data.at(6)) << 8) & 0xFF00);
    retCfg.base2LenMm |= (TO_ULONG(int(data.at(7)) << 16) & 0xFF0000);
    retCfg.base2LenMm |= (TO_ULONG(int(data.at(8)) << 24) & 0xFF000000);

    retCfg.base3LenMm  = TO_ULONG(data.at(9)) & 0xFF;
    retCfg.base3LenMm |= (TO_ULONG(int(data.at(10)) << 8) & 0xFF00);
    retCfg.base3LenMm |= (TO_ULONG(int(data.at(11)) << 16) & 0xFF0000);
    retCfg.base3LenMm |= (TO_ULONG(int(data.at(12)) << 24) & 0xFF000000);

    return retCfg;
}
/*!
 * \brief     Конвертирует массив байт с вектором состояния в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с вектором состояния
 * \return    структура с вектором состояния
 ***********************************************************************************/
Bcp2::VECTORSTATE Bcp2::getVectorStateFromData(QByteArray data)
{
    VECTORSTATE   retVector;


    retVector.latitude          = doubleFromByteArray(data) * 180 / M_PI;
    retVector.longitude         = doubleFromByteArray(data.mid(8, 8)) * 180 / M_PI;
    retVector.altitude          = doubleFromByteArray(data.mid(16, 8));
    retVector.rms               = floatFromByteArray(data.mid(24, 4));

    double conv = convLDtimeToDouble(data.mid(28, 10));
    retVector.time = getTime(conv);
    retVector.gpsWeekNum        = shortFromByteArray(data.mid(38, 2));
    retVector.date = getDate(conv, retVector.gpsWeekNum);
    retVector.latitudeVelocity  = doubleFromByteArray(data.mid(40, 8));
    retVector.longitudeVelocity = doubleFromByteArray(data.mid(48, 8));
    retVector.altitudeVelocity  = doubleFromByteArray(data.mid(56, 8));
    retVector.clockOffset       = floatFromByteArray(data.mid(64, 4));
    retVector.status            = TO_UCHAR(data.mid(68, 1).at(0));

    return retVector;
}
/*!
 * \brief     Конвертирует массив байт с угловой ориентацией в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с угловой ориентацией
 * \return    структура с угловой ориентацией
 ***********************************************************************************/
Bcp2::ANGLEORIENTATION Bcp2::getAngleOrientationFromData(QByteArray data)
{
    ANGLEORIENTATION ret;

    ret.weekTime   = doubleFromByteArray(data);
    ret.gpsWeekNum = shortFromByteArray(data.mid(8,  2));
    ret.course     = floatFromByteArray(data.mid(10, 4));
    ret.pitch      = floatFromByteArray(data.mid(14, 4));
    ret.roll       = floatFromByteArray(data.mid(18, 4));
    ret.courseRms  = floatFromByteArray(data.mid(22, 4));
    ret.pitchRms   = floatFromByteArray(data.mid(26, 4));
    ret.rollRms    = floatFromByteArray(data.mid(30, 4));
    ret.status     = TO_UCHAR(data.mid(34, 1).at(0));

    return ret;
}
/*!
 * \brief     Конвертирует массив байт с датой, временем и часовым поясом в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с датой, временем и часовым поясом
 * \return    структура с датой, временем и часовым поясом
 ***********************************************************************************/
Bcp2::TIMEDATATIMEZONE Bcp2::getTimezoneFromData(QByteArray data)
{
    TIMEDATATIMEZONE ret;

    ret.weekTime                = uLongFromByteArray(data);
    ret.day                     = TO_UCHAR(data.mid(4, 1).at(0));
    ret.mon                     = TO_UCHAR(data.mid(5, 1).at(0));
    ret.year                    = TO_USHORT(shortFromByteArray(data.mid(6, 2)));
    ret.timezoneCorrectionHours = char(data.mid(8, 1).at(0));
    ret.timezomeCorrectionMin   = char(data.mid(9, 1).at(0));

    return ret;
}
/*!
 * \brief     Конвертирует массив байт с результатом калибровки в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с датой, временем и часовым поясом
 * \return    структура с результатом калибровки
 ***********************************************************************************/
Bcp2::CALIBRATION      Bcp2::getCalibrationFromData(QByteArray data)
{
    CALIBRATION ret;

    ret.ant = getAntennaCfgFromData(data);

    ret.course  = TO_ULONG(data.at(13)) & 0xFF;
    ret.course |= (TO_ULONG(int(data.at(14)) << 8) & 0xFF00);
    ret.course |= (TO_ULONG(int(data.at(15)) << 16) & 0xFF0000);
    ret.course |= (TO_ULONG(int(data.at(16)) << 24) & 0xFF000000);

    ret.pitch  = TO_ULONG(data.at(17)) & 0xFF;
    ret.pitch |= (TO_ULONG(int(data.at(18)) << 8) & 0xFF00);
    ret.pitch |= (TO_ULONG(int(data.at(19)) << 16) & 0xFF0000);
    ret.pitch |= (TO_ULONG(int(data.at(20)) << 24) & 0xFF000000);

    ret.roll  = TO_ULONG(data.at(21)) & 0xFF;
    ret.roll |= (TO_ULONG(int(data.at(22)) << 8) & 0xFF00);
    ret.roll |= (TO_ULONG(int(data.at(23)) << 16) & 0xFF0000);
    ret.roll |= (TO_ULONG(int(data.at(24)) << 24) & 0xFF000000);

    return ret;
}
/*!
 * \brief     Конвертирует массив байт с результатами тестирования в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с датой, временем и часовым поясом
 * \return    структура с результатами тестирования
 ***********************************************************************************/
Bcp2::TESTSRESULT      Bcp2::getTestResultFromData(QByteArray data)
{
    TESTSRESULT ret = {false, false, false, false};
    if(data.at(1) & (1 << 0))
        ret.csm1 = true;
    if(data.at(1) & (1 << 1))
        ret.csm2 = true;
    if(data.at(1) & (1 << 2))
        ret.csm3 = true;
    if(data.at(1) & (1 << 7))
        ret.connection = true;

    return ret;
}
/*!
 * \brief     Конвертирует массив байт с данными плавающего решения в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с датой, временем и часовым поясом
 * \return    структура  с данными плавающего решения
 ***********************************************************************************/
Bcp2::FLOATSOLUTION    Bcp2::getFloatSolutionFromData(QByteArray data)
{
    FLOATSOLUTION ret;

    ret.numBase         = TO_UCHAR(data.at(0));
    ret.gpsTime         = doubleFromByteArray(data.mid(1,  8));
    ret.x               = doubleFromByteArray(data.mid(9,  8));
    ret.y               = doubleFromByteArray(data.mid(17,  8));
    ret.z               = doubleFromByteArray(data.mid(25,  8));

    ret.vx              = doubleFromByteArray(data.mid(33,  8));
    ret.vy              = doubleFromByteArray(data.mid(41,  8));
    ret.vz              = doubleFromByteArray(data.mid(49,  8));

    ret.nkaGps          = TO_UCHAR(data.at(57));
    ret.nkaGlonass      = TO_UCHAR(data.at(58));

    ret.fisherStatistic[0] = doubleFromByteArray(data.mid(59,  8));
    ret.fisherStatistic[1] = doubleFromByteArray(data.mid(67,  8));
    ret.fisherLimite[0]    = doubleFromByteArray(data.mid(75,  8));
    ret.fisherLimite[1]    = doubleFromByteArray(data.mid(83,  8));


    return ret;
}
/*!
 * \brief     Конвертирует массив байт с данными фиксированного решения в структуру (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт с датой, временем и часовым поясом
 * \return    структура  с данными фиксированного решения
 ***********************************************************************************/
Bcp2::FIXSOLUTION      Bcp2::getFixSolutionFromData(QByteArray data)
{
    FIXSOLUTION ret;

    if(data.at(0) & SolutionBaseLine_1)
        ret.line = SolutionBaseLine_1;
    else if(data.at(0) & SolutionBaseLine_2)
        ret.line = SolutionBaseLine_2;
    else if(data.at(0) & SolutionBaseLine_3)
        ret.line = SolutionBaseLine_3;
    else if(data.at(0) & SolutionBaseLine_4)
        ret.line = SolutionBaseLine_4;
    else ret.line = SolutionBaseLine_Max;

    ret.gpsTime = doubleFromByteArray(data.mid(1,  8));
    ret.x       = doubleFromByteArray(data.mid(9,  8));
    ret.y       = doubleFromByteArray(data.mid(17,  8));
    ret.z       = doubleFromByteArray(data.mid(25,  8));

    ret.nkaGps          = TO_UCHAR(data.at(33));
    ret.nkaGlonass      = TO_UCHAR(data.at(34));

    if(TO_UCHAR(data.at(35)))
        ret.nkaSign = true;
    else
        ret.nkaSign = false;

    return ret;
}
/*!
 * \brief     Корректирует время опорной недели (для фикса ролловер)
 * \param[in] структура с датой (предположительно полученной с помощью 46-го пакета)
 ***********************************************************************************/
void          Bcp2::setFixWeekCorrection(DATE date)
{
    unsigned char dayInMon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    short year, day, i;
    unsigned short week;

    if((date.year % 100) < 80) year = (date.year % 100) + 20;
    else year = (date.year % 100) - 80;

    day = year / 4;
    day *= 1461;
    year %= 4;
    day = day + (year * 365 + 1);
    if(year == 0 && date.mon < 3) day--;

    for(i = 0; i < date.mon - 1; i++) day += dayInMon[i];

    day = day + short(date.day - 6);
    week = TO_USHORT(day / 7);

    if(week != fixweek)
        fixweek = week;
}
/*!
 * \brief     Конвертирует первые 8-мь байт массива в double (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт
 * \return    число типа double
 ***********************************************************************************/
double Bcp2::doubleFromByteArray(QByteArray data)
{
    double ret = 0;
    data.truncate(8);
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> ret;
    return ret;
}
/*!
 * \brief     Конвертирует первые 4-ре байта массива в float (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт
 * \return    число типа float
 ***********************************************************************************/
float  Bcp2::floatFromByteArray(QByteArray data)
{
    float ret;

    memcpy(&ret, data.data(), 4); //! старая конструкция

    return ret;
}
/*!
 * \brief     Конвертирует первые 2-ва  байта массива в short (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт
 * \return    число типа short
 ***********************************************************************************/
short  Bcp2::shortFromByteArray(QByteArray data)
{
    short ret = 0;
    data.truncate(2);
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> ret;
    return ret;
}
/*!
 * \brief     Конвертирует первые 4-ре  байта массива в unsigned long (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] массив байт
 * \return    число типа unsigned long
 ***********************************************************************************/
unsigned long  Bcp2::uLongFromByteArray(QByteArray data)
{
    qint32 ret = 0;
    data.truncate(4);
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> ret;
    return static_cast<unsigned long>(ret);
}
/*!
 * \brief     Конвертирует первые 10-ть  байт массива из формата FP80 в double (ЗАКРЫТЫЙ МЕТОД)
 * \remark    используется для преобразования времени из пакета 88
 * \param[in] массив байт
 * \return    число типа double
 ***********************************************************************************/
double Bcp2::convLDtimeToDouble(QByteArray data)
{
    double   ret = 0.;

    if(TO_UCHAR(data.at(9))
    || TO_UCHAR(data.at(8))) {

        unsigned char byte[10];
        for(int i = 0; i < 10; i++) {
            byte[i] = TO_UCHAR(data.at( 9 - i ));
        }

        bool sign = false;
        if(byte[0] >= 128) {
            sign = true;
            byte[0] -= 128;
        }

        unsigned short val_1 = 16 * (((byte[0] - 64) * 256 + byte[1]) + 1024);
        unsigned char  val_2 = TO_UCHAR((val_1 - (val_1 % 256))/256);
        if(sign)
            val_2 += 128;

        QByteArray result;
        result += char(val_2);

        val_2   = TO_UCHAR(val_1 % 256) + ((byte[2] - 128 - ((byte[2] - 128) % 8)))/8;
        result += char(val_2);

        for(int i = 2; i < 8; i++) {
            result += char(TO_UCHAR((byte[i] % 8) * 32) + TO_UCHAR((byte[i+1] - (byte[i+1] % 8)) / 8));
        }

        QByteArray swapResult;
        for(int i = 7; i >= 0; i--){
            swapResult += result.at(i);
        }

        ret = doubleFromByteArray(swapResult);
    }

    return ret;
}
/*!
 * \brief     Получение значения недели с учётом fixweek (ЗАКРЫТЫЙ МЕТОД)
 * \remark    в рамках доработки rollover
 * \param[in] номер недели полученный из сообщения вектора состояния
 * \return    значение недели с учётом fixweek
 ***********************************************************************************/
unsigned short Bcp2::getFixWeek(unsigned short week)
{
    while(week < fixweek){
        week += 1024;
    }
    return week;
}
/*!
 * \brief     Получение времени по значению полученному с помощью \ref convLDtimeToDouble (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] значение времени полученное с помощью \ref convLDtimeToDouble
 * \return    структура с данными времени
 ***********************************************************************************/
Bcp2::TIME   Bcp2::getTime(double ldtime)
{
    unsigned long conv = TO_ULONG(ldtime / 1000);
    TIME          ret  = {0,0,0,0};

    ret.hour = (conv % 86400) / 3600;
    ret.min  = ((conv % 86400) % 3600) / 60;
    ret.sec  = conv % 60;
    ret.msec = TO_ULONG(ldtime) % 1000;
    return ret;
}
/*!
 * \brief     Получение даты по значению полученному с помощью \ref convLDtimeToDouble (ЗАКРЫТЫЙ МЕТОД)
 * \param[in] значение даты полученное с помощью \ref convLDtimeToDouble
 * \return    структура с данными даты
 ***********************************************************************************/
Bcp2::DATE   Bcp2::getDate(double ldtime, short gpsWeekNum)
{
    unsigned char monDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned long weekday = TO_ULONG(ldtime / 86400000),
                  quartNum,
                  numYear;
    DATE          ret = {0, 0, 0};

    weekday  += getFixWeek(gpsWeekNum % 1024) * 7 + 5;
    quartNum = TO_ULONG(weekday / 1461);
    weekday  -= quartNum * 1461;
    if(weekday < 366){
        numYear    = 0;
        monDays[1] = 29;
    } else {
        weekday -= 366;
        numYear = weekday / 365 + 1;
        weekday -= (numYear - 1) * 365;
        monDays[1] = 28;
    }

    ret.year = int(1980 + (quartNum * 4) + numYear);
    int i = 0, j = 0;
    while(i <= int(weekday)){
        i += monDays[j++];
    }
    ret.mon = j;
    ret.day = int(weekday) - i + monDays[j - 1] + 1;

    return ret;
}
