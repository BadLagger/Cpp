#ifndef BINR2_H
#define BINR2_H

//! \brief Максимальная длинна внутренних буферов в байтах
#define MAX_INTERNAL_BUFFER 256*7
//! \brief Максимальное значение поля длины пакета
#define BINR2_MAX_DATA_SIZE 2047

/* Описание класса */
class Binr2{
	public:
		/* Открытые типы и списки */
		//! \brief Список ошибок распаковщика
		enum DecodeErrors{
			DecodeError_NoStartFrame = 0,  //!< отсутствует начало пакета
			DecoderError_NoSoF_2,          //!< второй байт преамбулы неправильный
			DecoderError_NoSoF_3,          //!< третий байт преамбулы неправильный
			DecoderError_NoSoF_4,          //!< четвёртый байт преамбулы неправильный
			DecoderError_LenFrame,            //!< превышено значение поля длины
			DecoderError_InternalBufferError, //!< входной буфер слишком мал для приёма такого пакета
			DecoderError_CRC,                 //!< ошибка CRC 
			DecodeError_UnknownState,         //!< неизвестное состояние распаковщика
			DecodeError_NoError               //!< ошибок не обнаружено
		};
		//! \brief Список ошибок упаковщика
		enum EncodeErrors{
			EncodeError_ImpossibleId = 0,    //!< недопустимый идентификатор пакета
			EncodeError_InternalBufferError, //!< внутрений буфер слишком мал для такого количества данных
			EncodeError_ExternalBufferError, //!< внешний буфер слишком мал 
			EncodeError_NoError
		};
		/* Открытые методы */
		/*!
		 *  \brief Конструктор класса
		 *********************************************************************/
		Binr2();
		/*!
		 *  \brief Деструктор класса
		 *********************************************************************/
		~Binr2();
		/* Распаковка */
		/*!
		 *  \brief     Распаковщик входящих данных
		 *  \param[in] байт из входящего потока
		 *  \return    true - пакет разобран, false - пакет не обнаружен
		 *********************************************************************/
		bool decode(unsigned char const byte);
		/*!
		 *  \brief     Получение последнего разобраного пакета
		 *  \return    идентификатор пакета (0х00 - 0хFF)
		 *********************************************************************/
		unsigned char getDecodePkgId(void) const;
		/*!
		 *  \brief      Получение данных из последнего разобранного пакета
		 *  \param[out] указатель на буфер в который будут выгружены данные
		 *  \param[in]  размер внешнего буфера в байтах
		 *  \return     количество данных выгруженных во внешний буфер
		 *********************************************************************/
		unsigned short getDecodeData(unsigned char *out, unsigned short const size);
		/*!
		 *  \brief      Получение значения из поля длинны данных пакета
		 *  \return     значение поля длинны последнего пакета
		 *********************************************************************/
		unsigned short getDecodeDataSizeField(void) const;
		/*!
		 *  \brief     Получение кода последней возникшей ошибки при разборе
		 *  \return    код ошибки \ref DecodeErrors
		 *********************************************************************/
		DecodeErrors getDecodeError(void) const;
		/*!
		 *  \brief     Сброс флага ошибок распаковщика
		 *********************************************************************/
		void         resetDecodeError(void);
		/* Упаковка */
		/*!
		 *  \brief      Упаковщик (генератор) исходящих пакетов
		 *  \param[out] ссылка на буфер в котором будет сформирован пакет
		 *  \param[in]  максимальный размер исходящего буфера в байтах
		 *  \return     размер сформированного пакета в исходящем буфере
		 *********************************************************************/
		unsigned short encode(unsigned char *out, unsigned short const size);
		/*!
		 *  \brief     Установка идентификатора пакета для формирования
		 *  \param[in] идентификатор пакета который будет сформирован (00 - FF)
		 *********************************************************************/
		void setEncodePkgId(unsigned char const id);
		/*!
		 *  \brief     Установка данных которые будут добавлены в следующий пакет
		 *  \param[in] ссылка на буфер с данными
		 *  \param[in] размер данных в байтах
		 *********************************************************************/
		void setEncodeData(unsigned char *in, unsigned short const size);
		/*!
		 *  \brief     Получение кода ошибки возникшей при формировании пакета
		 *  \return    код ошибки \ref EncodeErrors
		 *********************************************************************/
		EncodeErrors getEncodeError(void) const;
		/*!
		 *  \brief     Сброс флага ошибок упаковщика 
		 *********************************************************************/
		void resetEncodeError(void);
	private:
		/* Закрытыте типы и списки */
		//! \brief Список состояний распаковщика
		enum DecodeStates{
			DecodeState_SeekStartOfFrame = 0,  //!< поиск начала пакета
			DecodeState_WaitSoF_2,             //!< ожидание второго байта преамбулы
			DecodeState_WaitSoF_3,             //!< ожидание третьего байта преамбулы
			DecodeState_WaitSoF_4,             //!< ожидание четвёртого байта преамбулы
			DecodeState_WaitLength_1,          //!< ожидание первого байта длинны
			DecodeState_WaitLength_2,          //!< ожидание второго байта длинны
			DecodeState_GetData,               //!< получение поля данных
			DecodeState_WaitCRC_1,             //!< получение первого байта CRC
			DecodeState_WaitCRC_2,             //!< получение второго байта CRC
			DecodeState_MaxState
		};
		/* Закрытые члены */
		unsigned char  decoderBuffer[MAX_INTERNAL_BUFFER]; //!< буфер для хранения входящих данных
		unsigned char  decoderData[MAX_INTERNAL_BUFFER];   //!< буфер для хранения разобранных данных
		unsigned short decoderBufferLen;                   //!< количество байт во входящем буфере
		unsigned short decoderDataLen;                     //!< количество байт в буфере разобраных данных
		unsigned short decoderWaitLen;                     //!< количество ожидаемых данных
		unsigned short decoderCrc;                         //!< CRC пакета распаковки
		unsigned short decoderCurrentCrc;                  //!< фактическая CRC
		unsigned char  decoderId;                          //!< идентификатор последнего разобранного пакета
		DecodeErrors   decoderErr;                         //!< код ошибки разборщика 
		DecodeStates   decoderState;                       //!< состояние распаковщика
		
		unsigned char  encodeData[MAX_INTERNAL_BUFFER]; //!< буфер данных для упаковки
		unsigned short encodeDataLen;                   //!< количество байт в буфере даннных для упаковки 
		unsigned char  encodeId;                        //!< идентифкатор для упаковки
		EncodeErrors   encodeErr;                       //!< код ошибки упаковщика (формирователя)
		
		/* Закрытые методы */
		/*!
		 *  \brief          Вычисление CRC
		 *  \param[in, out] текущее значение CRC. которое необходимо модифицировать
		 *  \param[in]      байт данных, который будет обработан для высчитывания CRC 
		 *********************************************************************/
		void addCrc(unsigned short &crc, unsigned char const byte);
};

#endif // BINR2_H