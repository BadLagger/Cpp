#include "nmeabase.h"

NmeaBase::NmeaBase()
{
    decoderState  = NmeaDecoderState_Idle;
    lastErrorCode = NmeaError_No;
}

bool NmeaBase::decode(char ch)
{
    bool result = false;

    switch(decoderState){
    case NmeaDecoderState_Idle: if(ch == '$'){
                                    csNum = 0;
                                    csStr.clear();
                                    currentDecodeMsg.address.clear();
                                    currentDecodeMsg.data.clear();
                                    decoderState = NmeaDecoderState_GetStart;
                                }
        break;
    case NmeaDecoderState_GetStart: if(ch == ','){
                                      if(!currentDecodeMsg.address.isEmpty())
                                          decoderState = NmeaDecoderState_GetAddress;
                                      else {
                                          decoderState  = NmeaDecoderState_Idle;
                                          lastErrorCode = NmeaError_NoAddressFrame;
                                      }
                                    } else {
                                        if(((ch >= '0') && (ch <= '9'))
                                        || ((ch >= 'A') && (ch <= 'Z'))){
                                            currentDecodeMsg.address += ch;
                                        } else {
                                            decoderState  = NmeaDecoderState_Idle;
                                            lastErrorCode = NmeaError_WrongSymbolInAddressFrame;
                                        }
                                    }
        break;
    case NmeaDecoderState_GetAddress: if(ch == '*')
                                        decoderState = NmeaDecoderState_GetData;
                                      else {
                                        if((ch >= ' ') && (ch <= '~'))
                                            currentDecodeMsg.data += ch;
                                        else {
                                            decoderState  = NmeaDecoderState_Idle;
                                            lastErrorCode = NmeaError_WrongSymbolInDataFrame;
                                        }
                                      }
        break;
     case NmeaDecoderState_GetData:   if(ch == '\r'){
                                        if(csStr.size() != 2){
                                            lastErrorCode = NmeaError_WrongCheckSumLen;
                                            decoderState  = NmeaDecoderState_Idle;
                                        } else {
                                            if(csStr.toInt() != int(csNum))
                                                decoderState = NmeaDecoderState_GetCR;
                                            else{
                                                lastErrorCode = NmeaError_CheckSum;
                                                decoderState  = NmeaDecoderState_Idle;
                                            }
                                        }
                                      } else {
                                        if(((ch >= '0') && (ch <= '9'))
                                        || ((ch >= 'A') && (ch <= 'F'))){
                                            if(csStr.size() < 2)
                                                csStr += ch;
                                            else{
                                                lastErrorCode = NmeaError_NoCR;
                                                decoderState  = NmeaDecoderState_Idle;
                                            }
                                        } else {
                                            lastErrorCode = NmeaError_WrongSymbolInCheckSum;
                                            decoderState  = NmeaDecoderState_Idle;
                                        }
                                      }
        break;
      case NmeaDecoderState_GetCR:  if(ch == '\n'){
                                      lastDecodeMsg = currentDecodeMsg;
                                      result = true;
                                    }
                                    else
                                        lastErrorCode = NmeaError_NoLF;
                                    decoderState  = NmeaDecoderState_Idle;
        break;
      default:                      lastErrorCode = NmeaError_UnknownDecoderState;
                                    decoderState  = NmeaDecoderState_Idle;
    }

    if((decoderState  > NmeaDecoderState_Idle) && (decoderState < NmeaDecoderState_GetData))
        csNum ^= ch;

    if((currentDecodeMsg.address.size() + currentDecodeMsg.data.size() > NMEABASE_MAX_MSG_LEN)){
            result        = false;
            lastErrorCode = NmeaError_MaxMsgLen;
            decoderState  = NmeaDecoderState_Idle;
            currentDecodeMsg.address.clear();
            currentDecodeMsg.data.clear();
    }

    return result;
}

NmeaString NmeaBase::encode(NMEADATA &msg)
{
    unsigned char csNum = 0;
    NmeaString retMsg;

    retMsg += '$';
    retMsg += msg.address;
    if(msg.data.size() > 0) {
        retMsg += ',';
        retMsg += msg.data;
    }

    for(int i = 1; i < retMsg.size(); i++){
        csNum ^= retMsg.at(i).cell();
    }
    retMsg += '*';
    retMsg += QString("%1").arg(csNum, 0, 16).rightJustified(2, '0').right(2).toUpper();
    retMsg += "\r\n";
    return retMsg;
}

NMEADATA   NmeaBase::getLastDecodeMsg()
{
    return lastDecodeMsg;
}

NmeaErrors NmeaBase::getLastErrorCode()
{
    NmeaErrors ret = lastErrorCode;
    lastErrorCode = NmeaError_No;
    return ret;
}

void NmeaBase::reset()
{
    decoderState  = NmeaDecoderState_Idle;
    lastErrorCode = NmeaError_No;
}


