#ifndef BINR_H
#define BINR_H

#include <QByteArray>

typedef enum
{
    BINR_ERR_ID,
    BINR_ERR_CRC_FRAME,
    BINR_ERR_CRC,
    BINR_ERR_EOP,
    BINR_ERR_MAX_ID
}BINR_ERR, *BINR_ERRptr;

class Binr
{
    unsigned short inCrc;
    QByteArray     inputData;
    QByteArray     cleanInputData;
    QByteArray     outputData;
    bool           crcStatus;
    bool           outputDataStatus;
    unsigned char  decoderState;
    unsigned char  lastDecodePkgId;
    BINR_ERR       lastErrorId;
    mutable  bool  errStatus;
public:
    Binr();
    bool          decode(unsigned char data);
    QByteArray    encode(unsigned char id);
    void          reset();
    void          crcEnable(bool state);
    void          putDataInNextPkg(QByteArray data);
    QByteArray    getDataFromLastPkg(void);
    unsigned char getLastDecodePkgId() const;
    bool          getErrorStatus() const;
    BINR_ERR      getLastError() const;
    bool          getCrcStatus() const;
private:
    void          makeCleanData();
};

#endif // BINR_H
