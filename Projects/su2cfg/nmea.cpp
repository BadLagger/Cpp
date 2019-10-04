#include "nmea.h"

Nmea::Nmea() : NmeaBase ()
{
    port = nullptr;
    waitAnswerTimer = nullptr;
}

Nmea::Nmea(ComPort *port) : NmeaBase ()
{
    this->port = port;
    waitAnswerTimer = nullptr;
}

void Nmea::resetHandler()
{
    reset();
}

QString Nmea::versionRequest()
{
    QString  retMsg;
    NMEADATA msg;

    msg.address += "ALVER";
    msg.data.clear();
    retMsg = encode(msg);
    trySendToPort(retMsg.toUtf8());

    return retMsg;
}

QString Nmea::setPortSettingsRequest(NMEA_PORTSETS sets)
{
    QString  retMsg;
    NMEADATA msg;

    msg.address += "PORZA";
    msg.data    += ('0' + sets.id);
    msg.data    += ',';
    msg.data    += QString::number(sets.speed);
    msg.data    += ',';
    msg.data    += ('0' + sets.protocol);//getProtocolStrNameById(sets.protocol);


    retMsg = encode(msg);
    trySendToPort(retMsg.toUtf8());

    return retMsg;
}

QString Nmea::getModeSettingsRequest()
{
    QString  retMsg;
    NMEADATA msg;

    msg.address += "XXGPQ";
    msg.data    += "VSU";
    retMsg = encode(msg);
    trySendToPort(retMsg.toUtf8());

    return retMsg;
}


void Nmea::receiverOn()
{
    if(port != nullptr){
        connect(port, &ComPort::readyRead, [=]{
            QByteArray data = port->readAll();
            emit recieveDataSignal(data);
            foreach(char i, data){
                if(decode(i)){
                    emit recieveMsgSignal(getLastDecodeMsg());
                }
                NmeaErrors errCode = getLastErrorCode();
                if(errCode != NmeaError_No)
                    emit receiverErrorSignal(errCode);
            }
        });
    }
}

void Nmea::receiverOff()
{
    if(port != nullptr){
        disconnect(port, &ComPort::readyRead, nullptr, nullptr);
    }
}

QString Nmea::getTalkerId(QString address)
{
    QString ret = "";

    if(address.at(0) == 'P') {
        ret += address.at(0);
    } else {
        if((address.at(0) == 'G') || (address.at(0) == 'A')){
            ret += address.at(0);
            if((address.at(1) == 'P') || (address.at(1) == 'L') || (address.at(1) == 'N') || (address.at(1) == 'A'))
                ret += address.at(1);
            else
                ret = "";
        }
    }

    return ret;
}

QString Nmea::getMessageId(QString address)
{
    QString ret = "";

    ret = getTalkerId(address);
    if(!ret.isEmpty()){
        ret = address.remove(0, ret.size());
    }

    return ret;
}

NmeaProtocolTypes Nmea::getProtocolIdNameByStr(QString name)
{
    NmeaProtocolTypes retId = NmeaProtocolType_Max;
    if(!name.compare("Off"))
        retId = NmeaProtocolType_Off;
    else {
        if(!name.compare("NMEA"))
            retId = NmeaProtocolType_NMEA;
        else {
            if(!name.compare("RTCM2"))
                retId = NmeaProtocolType_RTCM2;
            else {
                if(!name.compare("BINR"))
                    retId = NmeaProtocolType_BINR;
                else {
                    if(!name.compare("BCP2"))
                        retId = NmeaProtocolType_BCP2;
                    else {
                        if(!name.compare("RTCM3"))
                           retId = NmeaProtocolType_RTCM3;
                    }
                }
            }
        }
    }
    return retId;
}

QString       Nmea::getProtocolStrNameById(NmeaProtocolTypes protocolId)
{
    QString retStr = "";

    switch(protocolId){
    case NmeaProtocolType_RTCM3: retStr = "RTCM3";
        break;
    case NmeaProtocolType_Off: retStr = "Off";
        break;
    case NmeaProtocolType_NMEA: retStr = "NMEA";
        break;
    case NmeaProtocolType_RTCM2: retStr = "RTCM2";
        break;
    case NmeaProtocolType_BINR: retStr = "BINR";
        break;
    case NmeaProtocolType_BCP2: retStr = "BCP2";
        break;
    default: retStr = "n/a";;
    }
    return retStr;
}

void  Nmea::setWaitAnswerTimer(QTimer *timer)
{
    waitAnswerTimer = timer;
}

void Nmea::trySendToPort(QByteArray data)
{
    if(port && port->isOpen()){
        port->write(data);
        if(waitAnswerTimer != nullptr)
            waitAnswerTimer->start();
    }
}
