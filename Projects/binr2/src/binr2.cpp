#include "binr2.h"
//! \brief Таблица для подсчёта CRC
static const unsigned short Table_CRC[256]={
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,0x8108,0x9129,0xA14A,
	0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,
	0x72F7,0x62D6,0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,0x2462,
	0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,0xA56A,0xB54B,0x8528,0x9509,
	0xE5EE,0xF5CF,0xC5AC,0xD58D,0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,
	0x46B4,0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,0x48C4,0x58E5,
	0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,
	0x9969,0xA90A,0xB92B,0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
	0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,0x6CA6,0x7C87,0x4CE4,
	0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,
	0x8D68,0x9D49,0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,0xFF9F,
	0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,0x9188,0x81A9,0xB1CA,0xA1EB,
	0xD10C,0xC12D,0xF14E,0xE16F,0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,
	0x6067,0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,0x02B1,0x1290,
	0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,
	0xE54F,0xD52C,0xC50D,0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,0x26D3,0x36F2,0x0691,
	0x16B0,0x6657,0x7676,0x4615,0x5634,0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,
	0xB98A,0xA9AB,0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,0xCB7D,
	0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,0x4A75,0x5A54,0x6A37,0x7A16,
	0x0AF1,0x1AD0,0x2AB3,0x3A92,0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,
	0x8DC9,0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,0xEF1F,0xFF3E,
	0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,
	0x3EB2,0x0ED1,0x1EF0
	};
/*!
 *  \brief Конструктор класса
 *****************************************************************************/
Binr2::Binr2()
{
	// инициализация распаковщика
	decoderErr   = DecodeError_NoError;
	decoderState = DecodeState_SeekStartOfFrame;
	decoderId  = 0x00;
	decoderBufferLen = 0;
	decoderWaitLen = 0;
	decoderCrc = 0;
	decoderCurrentCrc = 0;
	for(int i = 0; i < MAX_INTERNAL_BUFFER; i++){
		decoderBuffer[i] = 0;
	}
	decoderDataLen = 0;
	for(int i = 0; i < MAX_INTERNAL_BUFFER; i++){
		decoderData[i] = 0;
	}
	
	// инициализация упаковщика
	encodeErr = EncodeError_NoError;
	encodeId  = 0x00;
	encodeDataLen = 0;
	for(int i = 0; i < MAX_INTERNAL_BUFFER; i++){
		encodeData[i] = 0;
	}
}

/*!
 *  \brief Деструктор класса
 *****************************************************************************/
Binr2::~Binr2()
{
	
}
/* Распаковка */
/*!
 *  \brief     Распаковщик входящих данных
 *  \param[in] байт из входящего потока
 *  \return    true - пакет разобран, false - пакет не обнаружен
 *****************************************************************************/
bool Binr2::decode(unsigned char const byte)
{
	bool ret = false;
	
	switch(decoderState){
		
		case DecodeState_SeekStartOfFrame:
			if(byte == 0xA5){
				decoderState = DecodeState_WaitSoF_2;
			} else {
				decoderErr = DecodeError_NoStartFrame;
			}
		break;
		
		case DecodeState_WaitSoF_2:
			if(byte == 0x5A){
				decoderState = DecodeState_WaitSoF_3;
			} else {
				decoderState = DecodeState_SeekStartOfFrame;
				decoderErr   = DecoderError_NoSoF_2;
			}
		break;
		
		case DecodeState_WaitSoF_3:
			if(byte == 0xB5){
				decoderState = DecodeState_WaitSoF_4;
			} else {
				decoderState = DecodeState_SeekStartOfFrame;
				decoderErr   = DecoderError_NoSoF_3;
			}
		break;
		
		case DecodeState_WaitSoF_4:
			if(byte == 0x5B){
				decoderState = DecodeState_WaitLength_1;
			} else {
				decoderState = DecodeState_SeekStartOfFrame;
				decoderErr   = DecoderError_NoSoF_4;
			}
		break;
		
		case DecodeState_WaitLength_1:
			decoderWaitLen = byte;
			decoderState   = DecodeState_WaitLength_2;
		break;
		
		case DecodeState_WaitLength_2:
			decoderWaitLen |= (unsigned short)(byte << 8);
			decoderCurrentCrc = 0;
			decoderBufferLen  = 0;
			decoderDataLen    = 0;
			if(decoderWaitLen > 0)
				decoderState   = DecodeState_GetData;
			else
				decoderState   = DecodeState_WaitCRC_1;
			
			if(decoderWaitLen > BINR2_MAX_DATA_SIZE){
				decoderState = DecodeState_SeekStartOfFrame;
				decoderErr = DecoderError_LenFrame;
			}else {
				if(decoderWaitLen > MAX_INTERNAL_BUFFER){
					decoderState = DecodeState_SeekStartOfFrame;
					decoderErr = DecoderError_InternalBufferError;
				}
			}
		break;
		
		case DecodeState_GetData:
			if(!decoderBufferLen){
				decoderId = byte;
			}
			
			decoderBuffer[decoderBufferLen] = byte;
			if(decoderBufferLen > 0){
				decoderData[decoderDataLen++] = byte;
			}
			addCrc(decoderCurrentCrc, byte);
			decoderBufferLen++;
			if(decoderBufferLen >= decoderWaitLen){
				decoderState = DecodeState_WaitCRC_1;
			}
		break;
		
		case DecodeState_WaitCRC_1:
			decoderCrc = byte;
			decoderState = DecodeState_WaitCRC_2;
		break;
		
		case DecodeState_WaitCRC_2:
			decoderCrc |= (unsigned short)(byte << 8);
			if(decoderCrc == decoderCurrentCrc){
				ret = true;
			} else {
				decoderErr = DecoderError_CRC;
			}
			decoderState = DecodeState_SeekStartOfFrame;
		break;
		
		default: decoderErr = DecodeError_UnknownState;
	}
	
	return ret;
}

/*!
 *  \brief     Получение последнего разобраного пакета
 *  \return    идентификатор пакета (0х00 - 0хFF)
 *****************************************************************************/
unsigned char Binr2::getDecodePkgId(void) const
{
	return decoderId;
}

/*!
 *  \brief      Получение данных из последнего разобранного пакета
 *  \param[out] ссылка на буфер в который будут выгружены данные
 *  \param[in]  размер внешнего буфера в байтах
 *  \return     количество данных выгруженных во внешний буфер
 *****************************************************************************/
unsigned short Binr2::getDecodeData(unsigned char *out, unsigned short const size)
{
	if(decoderDataLen < size){
		for(int i = 0; i < decoderDataLen; i++){
			out[i] = decoderData[i];
		}
		return decoderDataLen;
	}
	return 0;
}

/*!
 *  \brief      Получение значения из поля длинны данных пакета
 *  \return     значение поля длинны последнего пакета
 *****************************************************************************/
unsigned short Binr2::getDecodeDataSizeField(void) const
{
	return decoderWaitLen;
}
/*!
 *  \brief     Получение кода последней возникшей ошибки при разборе
 *  \return    код ошибки \ref DecodeErrors
 *****************************************************************************/
Binr2::DecodeErrors Binr2::getDecodeError(void) const
{
	return decoderErr;
}
/*!
 *  \brief     Сброс флага ошибок распаковщика
 *****************************************************************************/
void Binr2::resetDecodeError(void)
{
	decoderErr = DecodeError_NoError;
}
/* Упаковка */
/*!
 *  \brief      Упаковщик (генератор) исходящих пакетов
 *  \param[out] ссылка на буфер в котором будет сформирован пакет
 *  \param[in]  максимальный размер исходящего буфера в байтах
 *  \return     размер сформированного пакета в исходящем буфере
 *****************************************************************************/
unsigned short Binr2::encode(unsigned char *out, unsigned short const size)
{
	int len = 0;
	unsigned short crc = 0;
	
	if(size >= (encodeDataLen + 9)){  /* Длина данных + 4 байта преабула + 2 байта длины + 1 байт id + 2 байта CRC*/
		out[0] = 0xA5;
		out[1] = 0x5A;
		out[2] = 0xB5;
		out[3] = 0x5B;
		out[4] = (unsigned char)(encodeDataLen + 1);
		out[5] = (unsigned char)((encodeDataLen + 1) >> 8);
		out[6] = encodeId;
		addCrc(crc, encodeId);
		for(len = 0; len < encodeDataLen; len++) {
			out[7 + len] = encodeData[len];
			addCrc(crc, encodeData[len]);
		}
		len += 7;
		out[len++] = (unsigned char) crc;
		out[len++] = (unsigned char) (crc >> 8);
	} else {
		encodeErr = EncodeError_ExternalBufferError;
	}
	
	return len;
}

/*!
 *  \brief     Установка идентификатора пакета для формирования
 *  \param[in] идентификатор пакета который будет сформирован (00 - FF)
 *****************************************************************************/
void Binr2::setEncodePkgId(unsigned char const id)
{
	if(id != 0){
		encodeId = id;
	}
    else{
		encodeErr = EncodeError_ImpossibleId;
	}
}

/*!
 *  \brief     Установка данных которые будут добавлены в следующий пакет
 *  \param[in] ссылка на буфер с данными
 *  \param[in] размер данных в байтах
 *****************************************************************************/
void Binr2::setEncodeData(unsigned char *in, unsigned short const size)
{
	if(size < MAX_INTERNAL_BUFFER){
		encodeDataLen = size;
		for(int i = 0; i < size; i++){
			encodeData[i] = in[i];
		}
	} else {
		encodeErr = EncodeError_InternalBufferError;
	}
}

/*!
 *  \brief     Получение кода ошибки возникшей при формировании пакета
 *  \return    код ошибки \ref EncodeErrors
 ****************************************************************************/
Binr2::EncodeErrors Binr2::getEncodeError(void) const
{
	return encodeErr;
}
/*!
 *  \brief     Сброс флага ошибок упаковщика 
 *****************************************************************************/
void Binr2::resetEncodeError(void)
{
	encodeErr = EncodeError_NoError;
}

/* Закрытые методы */
/*!
 *  \brief          Вычисление CRC
 *  \param[in, out] текущее значение CRC. которое необходимо модифицировать
 *  \param[in]      байт данных, который будет обработан для высчитывания CRC 
 *********************************************************************/
void Binr2::addCrc(unsigned short &crc, unsigned char const byte)
{
	unsigned short int cval = ( ( crc >> 8 ) ^ byte ) & 0xff; //получить объединяющую величину
	crc = ( crc << 8 ) ^ Table_CRC[cval];                    // новый CRC
}