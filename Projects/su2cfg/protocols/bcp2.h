#ifndef BCP2_H
#define BCP2_H

#include <QObject>
#include <QTimer>
#include "binr.h"
#include "customs/comport.h"

//! \todo ДОБАВИТЬ ОБРАБОТКУ ОШИБОК ПО BINR

//! \brief макрос для преобразования к unsigned char
#define TO_UCHAR(x)  (static_cast<unsigned char>(x))
//! \brief макрос для преобразования к unsigned short
#define TO_USHORT(x) (static_cast<unsigned short>(x))
//! \brief макрос для преобразования к unsigned long
#define TO_ULONG(x)  (static_cast<unsigned long>(x))

/* Описание класса */
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
      Debug,
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
      Workmode_Collinear        = (1 << 0),
      Workmode_Free             = (1 << 1),
      Workmode_TypeA            = (1 << 2),  // ведомый впереди при соосном
      Workmode_TypeB            = (1 << 3),  // ведомый сзади   при соосном
      Workmode_SlaveCollinear   = (1 << 4),  // при соосном блоки смотрят в одну сторону
      Workmode_SlaveParallel    = (1 << 5),
      Workmode_SlaveTurn        = (1 << 6),  // при соосном блоки смотрят друг на друга
      Workmode_Unknown
    };
    //! \brief Список статуса навигационного решения
    enum Solutions{
        Solution_Present = (1 << 0),
        Solution_2D      = (1 << 1),
        Solution_RAIM    = (1 << 4),
        Solution_Unknown
    };
    //! \brief Список статуса углового решения
    enum Angles{
        Angle_Present      = (1 << 0),
        Angle_Mediana      = (1 << 1),  // усреднённое значение
        Angle_IMU          = (1 << 2),  // использование IMU
        Angle_Inclinometer = (1 << 4),
        Angle_Stop         = (1 << 5),
        Angle_SU2          = (1 << 8),
        Angle_Unknown
    };
    //! \brief Список базовых линий для запроса фиксированного и плавающего решений
    enum SolutionBaseLines{
      SolutionBaseLine_1 = (1 << 0),
      SolutionBaseLine_2 = (1 << 1),
      SolutionBaseLine_3 = (1 << 2),
      SolutionBaseLine_4 = (1 << 3),
      SolutionBaseLine_Max,
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
    //! \brief Структура для времени
    typedef struct{
        int hour;
        int min;
        int sec;
        int msec;
    }TIME, *TIMEptr;
    //! \brief Структура для даты
    typedef struct{
        int day;
        int mon;
        int year;
    }DATE, *DATEptr;
    //! \brief Структура вектора состояния BCP2
    typedef struct{
        double        latitude;
        double        longitude;
        double        altitude;
        float         rms;
        TIME          time;
        DATE          date;
        short         gpsWeekNum;
        double        latitudeVelocity;
        double        longitudeVelocity;
        double        altitudeVelocity;
        float         clockOffset;   // отклонение периода опорного генератора, мс
        unsigned char status;
    }VECTORSTATE, *VECTORSTATEptr;
    //! \brief Сруктура угловой ориентации BCP2
    typedef struct{
        double        weekTime;
        short         gpsWeekNum;
        float         course;
        float         pitch;
        float         roll;
        float         courseRms;
        float         pitchRms;
        float         rollRms;
        unsigned char status;
    }ANGLEORIENTATION, *ANGLEORIENTATIONptr;
    //! \brief Структура даты, времени и часового пояса
    typedef struct{
        unsigned long  weekTime;
        unsigned char  day;
        unsigned char  mon;
        unsigned short year;
        char           timezoneCorrectionHours;
        char           timezomeCorrectionMin;
    }TIMEDATATIMEZONE, *TIMEDATATIMEZONEptr;
    //! \brief Структура данных при успешной калибровке
    typedef struct{
        ANTENNACFG ant;
        long       course;
        long       pitch;
        long       roll;
    }CALIBRATION, *CALIBRATIONptr;
    //! \brief Структура с результатом тестирования
    typedef struct{
        bool csm1;   // тест CSM1
        bool csm2;   // тест CSM2
        bool csm3;   // тест CSM3
        bool connection; // Соединение со вторым блоком
    }TESTSRESULT, *TESTSRESULTptr;
    //! \brief Структура с данными плавающего решения
    typedef struct{
        double            gpsTime;
        double            x;
        double            y;
        double            z;
        double            vx;
        double            vy;
        double            vz;
        double            fisherStatistic[2];
        double            fisherLimite[2];
        unsigned char     numBase;
        unsigned char     nkaGps;
        unsigned char     nkaGlonass;
    }FLOATSOLUTION, *FLOATSOLUTIONptr;
    //! \brief Структура с данными фиксированного решения
    typedef struct{
        SolutionBaseLines line;
        double            gpsTime;
        double            x;
        double            y;
        double            z;
        unsigned char     nkaGps;
        unsigned char     nkaGlonass;
        bool              nkaSign;
    }FIXSOLUTION, *FIXSOLUTIONptr;
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
     * \brief     Запрос версии устройства по BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray        versionRequest();
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
     * \brief     Запрос режима работы BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray    workModeRequest();
    /*!
     * \brief     Установка режима работы СУ 02
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] байт режима работы
     * \todo      необходимо пересмотреть аргумент и сделать через структуру для скрытия
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    setWorkModeRequest(Addresses addrId, unsigned char mode);
    /*!
     * \brief     Установка режима работы СУ 02
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] байт режима работы
     * \todo      необходимо пересмотреть аргумент и сделать через структуру для скрытия
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    setWorkModeRequest(unsigned char mode);
    /*!
     * \brief     Установка антенной конфигурации СУ-М
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] структура антенной конфигурации
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        setAntennaCfg(ANTENNACFG &cfg);
    /*!
     * \brief     Запрос выдачи вектора состояния BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] темп выдачи вектора состояния в итервалах решения
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    setVectorStateRequest(Addresses addrId, unsigned char temp);
    /*!
     * \brief     Запрос выдачи вектора состояния BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] темп выдачи вектора состояния в итервалах решения
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray    setVectorStateRequest(unsigned char temp);
    /*!
     * \brief     Установка темпа решения для протокола BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] темп решения (1, 2, 5, 10)
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray        setSolutionTempRequest(int temp);
    /*!
     * \brief     Установка темпа решения для протокола BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] темп решения (1, 2, 5, 10)
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        setSolutionTempRequest(Addresses index, int temp);
    /*!
     * \brief     Получение темпа решения для протокола BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray        getSolutionTempRequest();
    /*!
     * \brief     Получение темпа решения для протокола BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray        getSolutionTempRequest(Addresses index);
    /*!
     * \brief     Запрос угловой ориентации для протокола BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] адрес устройства для которого формируется запрос
     * \param[in] темп выдачи угловой ориентации в итервалах решения
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    setAngleOrientationRequest(Addresses addrId, unsigned char temp);
    /*!
     * \brief     Запрос угловой ориентации для протокола BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \param[in] темп выдачи угловой ориентации в итервалах решения
     * \return    массив байтов со сформированным сообщением BINR
     ***********************************************************************************/
    QByteArray    setAngleOrientationRequest(unsigned char temp);
    /*!
     * \brief     Запрос даты, времени и часового пояса BCP2/BINR
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2/BINR
     ***********************************************************************************/
    QByteArray    getTimezoneRequest();
    /*!
     * \brief     Запрос на начало автокалибровки BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    startAutocalibrateRequest();
    /*!
     * \brief     Запрос на отмену автокалибровки BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    cancelAutocalibrateRequest();
    /*!
     * \brief     Запрос результатов тестирования BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    testResultsRequest();
    /*!
     * \brief     Запрос плавающего решения для базовой линии по BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    floatSolutionRequest(unsigned char bases);
    /*!
     * \brief     Запрос фиксированного решения для базовой линии по BCP2
     * \remark    Если задан адаптер порта, то запрос автоматически отсылается в порт
     * \remark    Если задан адаптер таймера ожидания ответа, то он будет активирован
     * \return    массив байтов со сформированным сообщением BCP2
     ***********************************************************************************/
    QByteArray    fixSolutionRequest(int i);
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
    /*!
     * \brief     Получение строкового названия состояния по байту статуса
     * \return    строка с описанием состояния
     ***********************************************************************************/
    QString       getSolutionStr(unsigned char status);

    /*!
     * \brief     Корректирует время опорной недели (для фикса ролловер)
     * \param[in] структура с датой (предположительно полученной с помощью 46-го пакета)
     ***********************************************************************************/
    void          setFixWeekCorrection(DATE date);
    /* Управляющие методы */
    /*!
     * \brief     Сброс внутренних состояний обработчика BCP2
     ***********************************************************************************/
    void          resetHandler();
    /*!
     * \brief     Включает внутрений обработчик для приёма данных из порта
     * \remark    Реализовано для случая когда на одном порту несколько протоколов
     ***********************************************************************************/
    void          receiverOn();
    /*!
     * \brief     Выключает внутрений обработчик для приёма данных из порта
     * \remark    Реализовано для случая когда на одном порту несколько протоколов
     ***********************************************************************************/
    void          receiverOff();

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
    /*!
     * \brief      Сигнал об успешном приёме вектора состояния
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  структура с вектором состояния
     ***********************************************************************************/
    void receiveVectorStateSignal(VECTORSTATE state);
    /*!
     * \brief      Сигнал об успешном приёме темпа решения
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  темп решения в герцах
     ***********************************************************************************/
    void receiveTempSolutionSignal(int temp);
    /*!
     * \brief      Сигнал об успешном приёме угловой ориентации
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  структура с угловой оринетацией
     ***********************************************************************************/
    void receiveAngleOrientationSignal(ANGLEORIENTATION state);
    /*!
     * \brief      Сигнал об успешном приёме даты, времени и часового пояса
     * \remark     Если задан адаптер таймера ожидания ответа, то он будет дизактивирован
     * \param[in]  структура датой, временем и часовым поясом
     ***********************************************************************************/
    void recieverTimezoneSignal(TIMEDATATIMEZONE timezone);
    /*!
     * \brief      Сигнал об отсутствии ответа на запросный пакет по таймауту
     * \param[in]  идентификатор пакета на который не получен ответ
     ***********************************************************************************/
    void noAnswerTimeoutOnPkgSignal(unsigned char id);
    /*!
     * \brief      Сигнал о начале калибровки
     ***********************************************************************************/
    void receiveCalibrationStartSignal();
    /*!
     * \brief      Сигнал об отмене калибровки
     ***********************************************************************************/
    void receiveCalibrationCancelSignal();
    /*!
     * \brief      Сигнал о приёме результатов тестирования
     ***********************************************************************************/
    void receiveTestResultSignal(TESTSRESULT test);
    /*!
     * \brief      Сигнал об успешном завершении калибровки
     ***********************************************************************************/
    void receiveCalibrationOk(CALIBRATION data);
    /*!
     * \brief      Сигнал об приёме данных плавающего решения
     ***********************************************************************************/
    void receiveFloatSolutionSignal(FLOATSOLUTION data);
    /*!
     * \brief      Сигнал об приёме данных фиксированного решения
     ***********************************************************************************/
    void receiveFixSolutionSignal(FIXSOLUTION data);
#ifdef QT_DEBUG
protected:
#else
private:
#endif
    /* Закрытые списки */
    //! \brief Идентификаторы запросных пакетов
    enum Requests{
      Request_PortSettings     = 0x0B,
      Request_Workmode         = 0x0D,
      Request_Test             = 0x11,
      Request_Version          = 0x1B,
      Request_Timezone         = 0x23,
      Request_VectorState      = 0x27,
      Request_AngleOrientation = 0xA4,
      Request_AntennaCfg       = 0xA6,
      Request_FloatSolution    = 0xBA,
      Request_FixSolution      = 0xBC,
      Request_SetOptions       = 0xD7,
      Request_Unknown
    };
    //! \brief Идентификаторы ответных пакетов
    enum Answers{
      Answer_Test             = 0x43,
      Answer_Timezone         = 0x46,
      Answer_PortSettings     = 0x50,
      Answer_Workmode         = 0x51,
      Answer_Version          = 0x70,
      Answer_VectorState      = 0x88,
      Answer_AngleOrientation = 0xA5,
      Answer_AntennaCfg       = 0xAF,
      Answer_FixSolution      = 0xBD,
      Answer_FloatSolution    = 0xCA,
      Answer_Options          = 0xE7,
      Answer_Unknown
    };

    /* Закрытые члены */
    ComPort *port;             //! Указатель на объект порта (адаптер)
    QTimer  *waitAnswerTimer;  //! Указатель на объект таймера (возможно необходимо полностью инкапсулировать??)
    unsigned short fixweek;    //! Значение опорной недели

    /* Закрытые методы */
    /*!
     * \brief      Попытка отправить массив данных в порт (ЗАКРЫТЫЙ МЕТОД)
     * \param[in]  ссылка на массив данных
     ***********************************************************************************/
    void              trySendToPort(const QByteArray &data) const;
    /*!
     * \brief     Конвертирует массив байт с настройками порта в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с настройками порта
     * \return    структура с настройками порта
     ***********************************************************************************/
    PORTSETS          getPortSettingsFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с конфигурацией антенны в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с конфигурацией антенны
     * \return    структура с конфигурацией антенны
     ***********************************************************************************/
    ANTENNACFG getAntennaCfgFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с вектором состояния в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с вектором состояния
     * \return    структура с вектором состояния
     ***********************************************************************************/
    VECTORSTATE getVectorStateFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с угловой ориентацией в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с угловой ориентацией
     * \return    структура с угловой ориентацией
     ***********************************************************************************/
    ANGLEORIENTATION getAngleOrientationFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с датой, временем и часовым поясом в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с датой, временем и часовым поясом
     * \return    структура с датой, временем и часовым поясом
     ***********************************************************************************/
    TIMEDATATIMEZONE getTimezoneFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с результатом калибровки в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с датой, временем и часовым поясом
     * \return    структура с результатом калибровки
     ***********************************************************************************/
    CALIBRATION      getCalibrationFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с результатами тестирования в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с датой, временем и часовым поясом
     * \return    структура с результатами тестирования
     ***********************************************************************************/
    TESTSRESULT      getTestResultFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с данными плавающего решения в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с датой, временем и часовым поясом
     * \return    структура  с данными плавающего решения
     ***********************************************************************************/
    FLOATSOLUTION    getFloatSolutionFromData(QByteArray data);
    /*!
     * \brief     Конвертирует массив байт с данными фиксированного решения в структуру (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт с датой, временем и часовым поясом
     * \return    структура  с данными фиксированного решения
     ***********************************************************************************/
    FIXSOLUTION      getFixSolutionFromData(QByteArray data);
    /*!
     * \brief     Конвертирует первые 8-мь байт массива в double (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт
     * \return    число типа double
     ***********************************************************************************/
    double doubleFromByteArray(QByteArray data);
    /*!
     * \brief     Конвертирует первые 4-ре байта массива в float (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт
     * \return    число типа float
     ***********************************************************************************/
    float  floatFromByteArray(QByteArray data);
    /*!
     * \brief     Конвертирует первые 2-ва  байта массива в short (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт
     * \return    число типа short
     ***********************************************************************************/
    short  shortFromByteArray(QByteArray data);
    /*!
     * \brief     Конвертирует первые 4-ре  байта массива в unsigned long (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] массив байт
     * \return    число типа unsigned long
     ***********************************************************************************/
    unsigned long  uLongFromByteArray(QByteArray data);
    /*!
     * \brief     Конвертирует первые 10-ть  байт массива из формата FP80 в double (ЗАКРЫТЫЙ МЕТОД)
     * \remark    используется для преобразования времени из пакета 88
     * \param[in] массив байт
     * \return    число типа double
     ***********************************************************************************/
    double convLDtimeToDouble(QByteArray data);
    /*!
     * \brief     Получение значения недели с учётом fixweek (ЗАКРЫТЫЙ МЕТОД)
     * \remark    в рамках доработки rollover
     * \param[in] номер недели полученный из сообщения вектора состояния
     * \return    значение недели с учётом fixweek
     ***********************************************************************************/
    unsigned short getFixWeek(unsigned short week);
    /*!
     * \brief     Получение времени по значению полученному с помощью \ref convLDtimeToDouble (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] значение времени полученное с помощью \ref convLDtimeToDouble
     * \return    структура с данными времени
     ***********************************************************************************/
    TIME   getTime(double ldtime);
    /*!
     * \brief     Получение даты по значению полученному с помощью \ref convLDtimeToDouble (ЗАКРЫТЫЙ МЕТОД)
     * \param[in] значение даты полученное с помощью \ref convLDtimeToDouble
     * \return    структура с данными даты
     ***********************************************************************************/
    DATE   getDate(double ldtime, short gpsWeekNum);
};

#endif // BCP2_H
