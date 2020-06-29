#ifndef NMEABASE_H
#define NMEABASE_H

#include <QString>

#define NMEABASE_MAX_MSG_LEN 120

//! \brief
typedef QString NmeaString;

typedef struct
{
    NmeaString address;
    NmeaString data;
}NMEADATA, *NMEADATAptr;

typedef enum
{
    NmeaError_No       = 0,
    NmeaError_MaxMsgLen,
    NmeaError_NoAddressFrame,
    NmeaError_WrongSymbolInAddressFrame,
    NmeaError_WrongSymbolInDataFrame,
    NmeaError_WrongCheckSumLen,
    NmeaError_WrongSymbolInCheckSum,
    NmeaError_NoCR,
    NmeaError_NoLF,
    NmeaError_CheckSum,
    NmeaError_UnknownDecoderState,
    NmeaError_Max
}NmeaErrors;

typedef enum
{
    NmeaDecoderState_Idle = 0,
    NmeaDecoderState_GetStart,
    NmeaDecoderState_GetAddress,
    NmeaDecoderState_GetData,
    NmeaDecoderState_GetCR,
    NmeaDecoderState_Max
}NmeaDecoderStates;

class NmeaBase
{
    NMEADATA           lastDecodeMsg,
                       currentDecodeMsg;
    NmeaErrors         lastErrorCode;
    NmeaDecoderStates  decoderState;
    NmeaString         csStr;
    unsigned char      csNum;
public:
    NmeaBase();
    bool       decode(char ch);
    NmeaString encode(NMEADATA &msg);
    NMEADATA   getLastDecodeMsg();
    NmeaErrors getLastErrorCode();
    void       reset();
};

#endif // NMEABASE_H
