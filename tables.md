## table "CoordinatePoint"
- ## +id
- ## +time
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

## table "Answer_RBS_ModeOther"
то, что в скобках - название структуры, которая содержит поля.

На сколько я понял, ответы будут отлчитаться только блоком *ExtendedInfo*.Поэтому можно сделать 1 таблицу "Answer", а блок *ExtendedInfo* вынести в дополнительнвые таблицы по типу ответа. 

Но тогда понадобиться дополнительный id для каждого ответа ??? 

Нужно решить, разные таблицы по типу вопроса, или вынести блок *ExtendedInfo* в отдельные таблицы, отставивь 1 общую таблицу "Answer"

Мне больше нравиться вариант с 2 таблицами по тимам ответов, если данные из них не участвуют в общих выборках

!!! - дубликаты

- ## +id CoordinatePoint
- Timestamp timestamp
- (PelengatedInfo)
    - !!!double distance
    - double azimuth
    - !!!double phase
    - uint32_t sum
    - uint32_t dif
    - !!!uint32_t pbl
- (POI::ResponseSignalInfo)
    - -uint32_t dDS : 10
    - -uint32_t ASP : 1
    - -uint32_t HIP : 1
    - interrogator_v3::Mode mode (от 0 до 4)
    - (ExtendedInfo)
        - (_RBS_ModeOther)
            - uint32_t Code : 13;
            - uint32_t SPI : 1;
            - uint32_t : 2; ??? что значит анонимное битовое поле? Выравнивание? Оно не нужно?
            - uint32_t nCode : 13;
            - uint32_t nSPI : 1;
            - !!!uint32_t PBL : 1;
            - uint32_t AW : 1;
            - +uint32_t _reserved[4 - 1] ???
        - uint32_t values[TotalWords + ExtendedWords] ???
- double monopulseAzimuth = 0
- double monopulseCorrection = 0
- !!!double phase = 0
- !!!double distance = 0
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
    - -uint32_t Dos : 20
    - -uint32_t dDS : 10
    - -uint32_t ASP : 1
    - -uint32_t HIP : 1
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

``` 
create table "CoordinatePoint"
(
    "idCoordinatePoint"    BIGINT           not null
        constraint "CoordinatePoint_pk"
            primary key,
    "timeCoordinatePoint"  timestamp        not null,
    "range "               double precision not null,
    "azimuth "             double precision not null,
    "amplitude "           numeric          not null,
    mode                   integer          not null,
    "missionCode"          integer,
    "unitCode "            integer,
    "bortNumber"           integer,
    altitude               integer,
    spi                    bool,
    "xImpulse"             bool,
    "militaryAlarm"        bool,
    "isRollCall"           bool,
    "isControlResponder"   bool,
    "missionCodeIsGarbled" bool,
    "unitCodeIsGarbled"    bool,
    "bortNumberIsGarbled"  bool,
    "heightIsGarbled"      bool
);

comment on column "CoordinatePoint"."timeCoordinatePoint" is 'может быть проблема с часовым поисом';

comment on column "CoordinatePoint"."amplitude " is 'не придумал как реализовать uint16_t, numeric универсальный тип (но ~медленный при расчетах в базе) ';

comment on column "CoordinatePoint".mode is 'можно добавить ограничение, от 0 до 4';
```
!!! отмечены дубликаты
```
create table "Answer_RBS_ModeOther"
(
    "idCoordinatePoint"   bigint    not null
        constraint "Answer_RBS_ModeOther_CoordinatePoint_idCoordinatePoint_fk"
            references "CoordinatePoint",
    "timeAnswerRBS"       timestamp not null,
    !!!distance              double precision,
    azimuth               double precision,
    !!!phase                 double precision,
    sum                   numeric,
    dif                   numeric,
    !!!pbl                   numeric,
    mode                  integer,
    "Code"                bit varying(13),
    SPI                   bit varying(1),
    "nCode"               bit varying(13),
    "nSPI"                bit varying(1),
    !!!PBL                   bit varying(1),
    AW                    bit varying(1),
    "monopulseAzimuth"    double precision,
    "monopulseCorrection" double precision,
    !!!phase                 double precision,
    !!!distance              double precision,
    "azimuthFromDistance" double precision
);

comment on column "Answer_RBS_ModeOther"."timeAnswerRBS" is 'обратить внимание на часовой пояс, плюс нужно ли это поле';

comment on column "Answer_RBS_ModeOther".mode is 'возможно ограничение от 0 до 4';

comment on column "Answer_RBS_ModeOther"."Code" is 'битовое поле может быть меньшей длинны (varying)';

comment on column "Answer_RBS_ModeOther".SPI is 'следующим идет анонимное битовое поле ?';

comment on column "Answer_RBS_ModeOther".AW is 'поле _reserved и values[TotalWords + ExtendedWords] пропущено';

```