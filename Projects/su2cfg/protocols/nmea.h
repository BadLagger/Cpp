#ifndef NMEA_H
#define NMEA_H

#include <QObject>
#include <QTimer>
#include "nmeabase.h"
#include "customs/comport.h"

typedef enum
{
    NmeaPortId_Current = 0,
    NmeaPortId_1,
    NmeaPortId_2,
    NmeaPortId_3,
    NmeaPortId_Max
}NmeaPortIds;

typedef enum
{
    NmeaProtocolType_Off = 0,
    NmeaProtocolType_NMEA,
    NmeaProtocolType_RTCM2,
    NmeaProtocolType_BINR,
    NmeaProtocolType_BCP2,
    NmeaProtocolType_RTCM3,
    NmeaProtocolType_Max
}NmeaProtocolTypes;

typedef struct
{
    NmeaPortIds       id;
    unsigned long     speed;
    NmeaProtocolTypes protocol;
}NMEA_PORTSETS, *NMEA_PORTSETSptr;

class Nmea : public QObject, NmeaBase
{
    Q_OBJECT

    ComPort *port;
    QTimer  *waitAnswerTimer;

public:
    Nmea();
    Nmea(ComPort *port);
    void setWaitAnswerTimer(QTimer *timer);
    void resetHandler();
    QString versionRequest();
    QString setPortSettingsRequest(NMEA_PORTSETS sets);
    QString getModeSettingsRequest();
    NmeaProtocolTypes getProtocolIdNameByStr(QString name);
    QString           getProtocolStrNameById(NmeaProtocolTypes protocolId);
    void    receiverOn();
    void    receiverOff();
    QString getTalkerId(QString address);
    QString getMessageId(QString address);
signals:
    void recieveMsgSignal(NMEADATA      msg);
    void recieveDataSignal(QByteArray   data);
    void receiverErrorSignal(NmeaErrors code);
private:
    void trySendToPort(QByteArray data);
};

#endif // NMEA_H
