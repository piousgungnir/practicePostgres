## table "CoordinatePoint"
- ## +id
- bool rejectedByTagc
- double range
- double azimuth
- uint16_t amplitude
---
- SystemMode mode (int 0 - 4)
---
- int missionCode = NotValidValue
- int unitCode = NotValidValue
- int bortNumber = NotValidValue
- int altitude = NotValidValue
- bool spi = false
- bool xImpulse = false
- bool militaryAlarm = false
---
- struct Mark ???
---
- struct ModeS ???
---
- bool isRollCall = false
- bool isControlResponder = false
---
- pack_t pack Это вектор ответов, они вынесены в отдельные таблицы под id CoordinatePoint
---
- bool missionCodeIsGarbled = false
- bool unitCodeIsGarbled = false
- bool bortNumberIsGarbled = false
- bool heightIsGarbled = false
- 
## table "Answer_RBS_ModeOther"
то, что в скобках - название структуры, которая содержит поля.

На сколько я понял, ответы будут отлчитаться только блоком *ExtendedInfo*.Поэтому можно сделать 1 таблицу "Answer", а блок *ExtendedInfo* вынести в дополнительнвые таблицы по типу ответа. 

Но тогда понадобиться дополнительный id для каждого ответа ??? 

Нужно решить, разные таблицы по типу вопроса, или вынести блок *ExtendedInfo* в отдельные таблицы, отставивь 1 общую таблицу "Answer"

Мне больше нравиться вариант с 2 таблицами по тимам ответов, если данные из них не участвуют в общих выборках

- ## +id CoordinatePoint
- Timestamp timestamp
- (PelengatedInfo)
    - double distance
    - double azimuth
    - double phase
    - uint32_t sum
    - uint32_t dif
    - uint32_t pbl
- (POI::ResponseSignalInfo) тут не совсем понял как работает структура без названия(
    - uint32_t Dos : 20
    - uint32_t dDS : 10
    - uint32_t ASP : 1
    - uint32_t HIP : 1
    - interrogator_v3::Mode mode (от 0 до 4)
    - (ExtendedInfo)
        - (_RBS_ModeOther)
            - uint32_t Code : 13;
            - uint32_t SPI : 1;
            - uint32_t : 2;
            - uint32_t nCode : 13;
            - uint32_t nSPI : 1;
            - uint32_t PBL : 1;
            - uint32_t AW : 1;
            - +uint32_t _reserved[4 - 1] ???
        - uint32_t values[TotalWords + ExtendedWords] ???
- double monopulseAzimuth = 0
- double monopulseCorrection = 0
- double phase = 0
- double distance = 0
- double azimuthFromDistance = 0

## table "Answer_S_Data"
то, что в скобках - название структуры, которая содержит поля.

На сколько я понял, ответы будут отлчитаться только блоком *ExtendedInfo*.Поэтому можно сделать 1 таблицу "Answer", а блок *ExtendedInfo* вынести в дополнительнвые таблицы по типу ответа. 

Но тогда понадобиться дополнительный id для каждого ответа ??? 

Нужно решить, разные таблицы по типу вопроса, или вынести блок *ExtendedInfo* в отдельные таблицы, отставивь 1 общую таблицу "Answer"

Мне больше нравиться вариант с 2 таблицами по тимам ответов, если данные из них не участвуют в общих выборках

- ## +id CoordinatePoint
- Timestamp timestamp
- (PelengatedInfo)
    - double distance
    - double azimuth
    - double phase
    - uint32_t sum
    - uint32_t dif
    - uint32_t pbl
- (POI::ResponseSignalInfo)
    - uint32_t Dos : 20
    - uint32_t dDS : 10
    - uint32_t ASP : 1
    - uint32_t HIP : 1
    - interrogator_v3::Mode mode (от 0 до 4)
    - (ExtendedInfo)
        - (_S_Data)
            - uint32_t data[4];
            - uint32_t : 32;
            - uint32_t : 32;
            - uint32_t : 32;
            - uint32_t : 16;
            - uint32_t BDS2 : 4;
            - uint32_t BDS1 : 4;
            - uint32_t sicAllCall : 2;
            - uint32_t PC_type : 2;
            - uint32_t PC_EC : 2;
            - uint32_t : 2;
        - uint32_t values[TotalWords + ExtendedWords] ???
- double monopulseAzimuth = 0
- double monopulseCorrection = 0
- double phase = 0
- double distance = 0
- double azimuthFromDistance = 0