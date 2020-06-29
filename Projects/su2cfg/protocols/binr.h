#ifndef BINR_H
#define BINR_H

#include <QByteArray>

//! \todo Необходим рефакторинг

//! \brief Список ошибок при разборе BINR
//! \todo  Необходимо внести в класс \ref Binr
typedef enum
{
    BINR_ERR_ID,
    BINR_ERR_CRC_FRAME,
    BINR_ERR_CRC,
    BINR_ERR_EOP,
    BINR_ERR_MAX_ID
}BINR_ERR, *BINR_ERRptr;

/* Описание класса */
/*!
 * \brief  Класс протокола BINR нижнего уровня
 * \remark Предоставляет базовый разборщик протокла BINR
 * \remark Не содержит анализа конкретных идентификаторов пакетов, производит разбор по полям
 ***********************************************************************************/
class Binr
{
    /* Открытые методы */
public:
    /*!
     * \brief  Конструктор по умолчанию
     ***********************************************************************************/
    Binr();
    /*!
     * \brief     Распаковщик BINR
     * \param[in] байт данных
     * \return    true - в процессе разбора был выделен целый пакет BINR
     * \todo      возможно стоит переделать на сигнал, то есть генерируется сигнал при успешном разборе
     ***********************************************************************************/
    bool          decode(unsigned char data);
    /*!
     * \brief     Упаковщик BINR
     * \param[in] значение которое будет подставлено в поле идентификатора пакета
     * \remark    для того чтобы добавить данные в пакет необходимо воспользоваться функцией \ref putDataInNextPkg
     * \return    массив данных представленных в виде BINR пакета
     ***********************************************************************************/
    QByteArray    encode(unsigned char id);
    /*!
     * \brief     Сброс машины состояния разборщика
     ***********************************************************************************/
    void          reset();
    /*!
     * \brief     Включение/отключение CRC
     * \remark    актуально только для упаковщика так как распаковщик автоматически определяет наличие CRC
     * \param[in] true - CRC включено, false - отключено
     ***********************************************************************************/
    void          crcEnable(bool state);
    /*!
     * \brief     Метод для добавления данных в следующий сформированный пакет BINR
     * \param[in] массив данных которые будут включены в следующий сформированный пакет BINR
     ***********************************************************************************/
    void          putDataInNextPkg(QByteArray data);
    /*!
     * \brief     Получения массива байт поля данных из последнего разобранного пакета BINR
     * \return    массив байт поля данных из последнего разобранного пакета BINR
     * \todo      Возможно стоит заменить данными передаваемыми в составе сигнала
     ***********************************************************************************/
    QByteArray    getDataFromLastPkg(void);
    /*!
     * \brief     Получение идентификатора последнего разобранного пакета BINR
     * \return    идентификатор последнего разобранного пакета BINR
     * \todo      Возможно стоит заменить данными передаваемыми в составе сигнала
     ***********************************************************************************/
    unsigned char getLastDecodePkgId() const;
    /*!
     * \brief     Получение статуса ошибки
     * \return    true - если была обнаружена ошибка
     * \todo      Возможно стоит заменить сигналом
     ***********************************************************************************/
    bool          getErrorStatus() const;
    /*!
     * \brief     Получение идентификатора последней ошибки
     * \return    идентификатор ошибки
     * \todo      Возможно стоит заменить данными передаваемыми в составе сигнала
     ***********************************************************************************/
    BINR_ERR      getLastError() const;
    /*!
     * \brief     Получение статуса CRC
     * \return    true - CRC включено, false - отключено
     * \todo      Возможно стоит упразднить
     ***********************************************************************************/
    bool          getCrcStatus() const;
    /*!
     * \brief     Получение идентификатора последнего созданного опакета BINR
     * \return    идентификатор последнего созданного пакета
     ***********************************************************************************/
    unsigned char getLastEncodeId() const;
private:
    /* Закрытые члены */
    unsigned short inCrc;               //! CRC входящего пакета BINR
    QByteArray     inputData;           //! массив для хранения входящих данных
    QByteArray     cleanInputData;      //! массив для хранения "чистых" байт из поля принятого поля данных
    QByteArray     outputData;          //! массив данных подготовленных для отправки (упаковки)
    bool           crcStatus;           //! статус использования CRC
    bool           outputDataStatus;    //! признак того что в следующем исходящем пакете добавлено поле данных
    unsigned char  decoderState;        //! статус состояния разборщика
    unsigned char  lastDecodePkgId;     //! идентификатор последнего разобранного пакета BINR
    BINR_ERR       lastErrorId;         //! идентификатор последней возникшей ошибки
    mutable  bool  errStatus;           //! статус состояния ошибки при разборе
    unsigned char  lastEndcodeId;       //! идентификатор последнего созданного пакета BINR
    /* Закрытые методы */
    /*!
     * \brief     Убирает двойные десятки из поля данных последнего полученного пакета BINR
     ***********************************************************************************/
    void          makeCleanData();
};

#endif // BINR_H