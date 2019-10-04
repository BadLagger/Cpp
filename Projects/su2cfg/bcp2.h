#ifndef BCP2_H
#define BCP2_H

#include <QObject>
#include <QTimer>
#include "binr.h"
#include "comport.h"

//! \todo ДОБАВИТЬ ОБРАБОТКУ ОШИБОК ПО BINR

/*!
 * \brief  Класс протокола BCP2 верхнего уровня
 * \remark Предоставляет интерфейс для обмена информацией по протоколу BCP2
 * \remark Наследует класс базового разборщика BINR так как протокол BCP2 является бинароподобным
 ***********************************************************************************/
class Bcp2 : public QObject, Binr
{
    Q_OBJECT

public:
    /* Открытые списки */
    //! \brief Список адресов устройств BCP2
    enum Addresses{
        Address_RNPI_1 = 0,
        Address_RNPI_2,
        Address_RNPI_3,
        Address_RNPI_4,
        Address_All,
        Address_NAP,
        Address_Max,
    };
    //! \brief Список идентификаторов портов BCP2
    enum Ports{
      Port_Current = 0,
      Port_1,
      Port_2,
      Port_Max
    };
    //! \brief Список протоколов обмена BCP2
    enum Protocols{
      Protocol_Current = 0,
      Protocol_Off,
      Protocol_NMEA,
      Protocol_RTCM,
      Protocol_BINR,
      Protocol_BCP2 = 6,
      Protocol_Max
    };
    //! \brief Список режимов работы для BCP2 (битовые маски)
    enum Workmodes{
      Workmode_MasterCollinear  = (1 << 0),
      Workmode_MasterFree       = (1 << 1),
      Workmode_MasterFreeLittle = (1 << 2),
      Workmode_SlaveCollinear   = (1 << 4),
      Workmode_SlaveParallel    = (1 << 5),
      Workmode_Ortogonal        = (1 << 6),
      Workmode_Unknown
    };

    /* Открытые структуры */
    //! \brief Структура для конфигурирования настроек портов BCP2
    typedef struct{
        Ports             portid;   // идентификатор порта
        unsigned long     speed;    // скорость обмена
        Protocols         protocol; // идентификатор протокола
    }PORTSETS, *PORTSETSptr;
    //! \brief Структура антенной конфигурации BCP2
    typedef struct{
        unsigned long base1LenMm;
        unsigned long base2LenMm;
        unsigned long base3LenMm;
    }ANTENNACFG, *ANTENNACFGptr;

    /* Открытые методы */
    /*!
     * \brief  Конструктор по умолчанию
     ***********************************************************************************/
    Bcp2();
    /*!
     * \brief     Конструктор с адаптером порта
     * \param[in] указатель на внешнюю структуру порта с которым предстоит работать
     ***********************************************************************************/
    Bcp2(ComPort *port);
    /*!
     * \brief     Адаптер для задания внешнего таймера ожидания ответа
     * \remark    Возможно стоит полностью инкапсулировать
     * \param[in] указатель на внешний таймер для ожидания ответа на запрос
     ***********************************************************************************/
    void              setWaitAnswerTimer(QTimer *timer);

    /* Методы-запросы */
    /*!
     * \brief     Запрос версии устройства
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        versionRequest(Addresses addrId);
    /*!
     * \brief     Запрос настроек порта
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] идентификатор порта для которого запрашиваются настройки
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        portSettingsRequest(Ports portId);

    /*!
     * \brief     Запрос конфигурации антенны
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        antennaCfgRequest();
    /*!
     * \brief     Установка настроек порта
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] ссылка на структуру с настройками порта
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        setPortSettingsRequest(const PORTSETS &sets);
    /*!
     * \brief     Запрос режима работы СУ 02
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        workModeRequest(Addresses addrId);
    /*!
     * \brief     Установка режима работы СУ 02
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] байт режима работы
     * \todo      необходимо пересмотреть аргумент и сделать через структуру для скрытия
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        setWorkModeRequest(Addresses addrId, unsigned char mode);
    /*!
     * \brief     Установка антенной конфигурации СУ-М
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] структура антенной конфигурации
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        setAntennaCfg(ANTENNACFG &cfg);

    /* Методы-конверторы */
    /*!
     * \brief     Получение строки с названием протокола по его идентификатору
     * \param[in] идентификатор протокола
     * \return    строка с названием протокола
     ***********************************************************************************/
    QString           getProtocolStrNameById(Protocols protocolId);
    /*!
     * \brief     Получение идентификатора протокола по строке с названием протокола
     * \param[in] строка с названием протокола
     * \return    идентификатор протокола
     ***********************************************************************************/
    Protocols         getProtocolIdNameByStr(QString name);

    /* Управляющие методы */
    /*!
     * \brief     Сброс внутренних состояний обработчика BCP2
     ***********************************************************************************/
    void              resetHandler();
    /*!
     * \brief     Включает внутрений обработчик для приёма данных из порта
     * \remark    Реализовано для случая когда на одном порту несколько протоколов
     ***********************************************************************************/
    void              receiverOn();
    /*!
     * \brief     Выключает внутрений обработчик для приёма данных из порта
     * \remark    Реализовано для случая когда на одном порту несколько протоколов
     ***********************************************************************************/
    void              receiverOff();

    /* Сигналы */
signals:
    /*!
     * \brief      Сигнал о приёме каких либо данных с порта
     * \param[in]  массив принятых байт
     ***********************************************************************************/
    void recieveDataSignal(QByteArray data);
    /*!
     * \brief      Сигнал об успешном приёме ответа на запрос настроек порта
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  структура с принятыми настройками порта
     ***********************************************************************************/
    void recievePortSettingsSignal(PORTSETS portSets);
    /*!
     * \brief      Сигнал об успешном приёме ответа на запрос режима работы СУ 02
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  массив байт с принятыми настройками режима
     * \todo       сигнал должен передавать готовую структуру с настройками
     ***********************************************************************************/
    void recieveWorkmodeSignal(QByteArray data);
    /*!
     * \brief      Сигнал об успешном приёме ответа на запрос версии
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  массив байт с принятой версией
     * \todo       сигнал должен передавать не байты, но строку с версией
     ***********************************************************************************/
    void recieveVersionSignal(QByteArray data);
    /*!
     * \brief      Сигнал об успешном приёме ответа на установку конфигурации антенны
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  структура с конфигурацией антенны
     ***********************************************************************************/
    void receiveAntennaCfgSignal(ANTENNACFG cfg);

private:
    /* Закрытые списки */
    //! \brief Идентификаторы запросных пакетов
    enum Requests{
      Request_PortSettings = 0x0B,
      Request_Workmode     = 0x0D,
      Request_Version      = 0x1B,
      Request_AntennaCfg   = 0xA6,
      Request_Unknown
    };
    //! \brief Идентификаторы ответных пакетов
    enum Answers{
      Answer_PortSettings = 0x50,
      Answer_Workmode     = 0x51,
      Answer_Version      = 0x70,
      Answer_AntennaCfg   = 0xAF,
      Answer_Unknown
    };

    /* Закрытые члены */
    ComPort *port;             //! Указатель на объект порта (адаптер)
    QTimer  *waitAnswerTimer;  //! Указатель на объект таймера (возможно необходимо полностью инкапсулировать??)

    /* Закрытые методы */
    /*!
     * \brief      Попытка отправить массив данных в порт (ЗАКРЫТЫЙ МЕТОД)
     * \param[in]  ссылка на массив данных
     ***********************************************************************************/
    void              trySendToPort(const QByteArray &data) const;
    /*!
     * \brief     Конвертирует массив байт с настройками порта в структуру
     * \param[in] массив байт с настройками порта
     * \return    структура с настройками порта
     ***********************************************************************************/
    PORTSETS          getPortSettingsFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с конфигурацией антенны в структуру
     * \param[in] массив байт с настройками порта
     * \return    структура с конфигурацией антенны
     ***********************************************************************************/
    ANTENNACFG getAntennaCfgFromData(QByteArray data);
};

#endif // BCP2_H
