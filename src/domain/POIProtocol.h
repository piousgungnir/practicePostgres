#ifndef POIPROTOCOL_H
#define POIPROTOCOL_H

#include <algorithm>

#include "modes_v3.h"


namespace POI
{

/* Пеленг ХИП "Пароль" или Mk XA/XII и RBS (if HIP = 1) */
struct _Peleng_HIP
{
    uint32_t skw : 32; /* кол-во импульсов у пеленгов ХИП (>62) для определения скважности */
};
//-----------------------------------------------------------------------------

/* Пеленг АШП "Пароль" или Mk XA/XII и RBS (if ASP = 1) */
struct _Peleng_ASHP
{
    uint32_t value : 32; /* 0xAAAA5555 */
};
//-----------------------------------------------------------------------------

/* Тревога "Пароль" (if HIP=ASP=oP1=...zM2=zM1=0) */
#define EXTENDED_INFO__PAROL_ALERT 0x5555AAAA
struct _Parol_Alert
{
    uint32_t value : 32; /* 0x5555AAAA */
};

/* Режим 1 "Пароль" (if oP1=zP1= 1) */
struct _Parol_Mode1
{
    uint32_t : 22;
    uint32_t nBd : 1;   /* "1" - недостоверность признака Bd */
    uint32_t Gr : 1;    /* "1" - признак вхожд-я хотя бы одного из импульсов координ-го кода в состав координ-го кода др ответа */
    uint32_t KodOO : 6; /* " позиц-й код "гребенки" в реж-х 1,3-6 "Пароль" (мл. разр. соотв-ет ПОК=1, а ст. - ПОК=6)*/
    uint32_t PBL : 1;   /* "1" - признак обнаружения  ОС в "серой" зоне (Aсум < Араз + подставка) */
    uint32_t Bd : 1;    /* "1" - признак "Бедствие" (координатное) в режимах 1,2,4,6 "Пароль" */
};

/* Режим 2 "Пароль" (if oP2=zP2= 1) */
struct _Parol_Mode2
{
    uint32_t KodGO : 16; /* позиц-й код "гребенки" в реж-е 2 "Пароль" (мл. разр. соотв-ет ПОК=0, а ст. - ПОК=15)*/
    uint32_t PokGO : 4;  /* ПОК ГО в режиме 2 "Пароль"(0-15) */
    uint32_t : 2;
    uint32_t nBd : 1; /* "1" - недостоверность признака Bd */
    uint32_t Gr : 1;  /* "1" - признак вхожд-я хотя бы одного из импульсов координ-го кода в состав координ-го кода др ответа */
    uint32_t : 1;
    uint32_t PBL : 1; /* "1" - признак обнаружения  ОС в "серой" зоне (Aсум < Араз + подставка) */
    uint32_t Bd : 1;  /* "1" - признак "Бедствие" (координатное) в режимах 1,2,4,6 "Пароль" */
};

struct _Parol_Mode3
{
    uint32_t KodIO : 7;
    uint32_t Gr3 : 1;
    uint32_t nKodIO : 7;
    uint32_t : 1;
    uint32_t ZK3 : 4;
    uint32_t : 3;
    uint32_t Gr : 1;
    uint32_t KodOO : 6;
    uint32_t PBL : 1;
    uint32_t : 1;
};

struct _Parol_Mode4
{
    uint32_t IN_0 : 4;
    uint32_t IN_1 : 4;
    uint32_t IN_2 : 4;
    uint32_t IN_3 : 4;
    uint32_t IN_4 : 4;
    uint32_t DI : 2;
    uint32_t nBd : 1;
    uint32_t Gr : 1;
    uint32_t KodOO : 6;
    uint32_t PBL : 1;
    uint32_t Bd : 1;
};

struct _Parol_Mode6
{
    uint32_t KodH : 14;
    uint32_t Ur : 1;
    uint32_t Bdi : 1;
    uint32_t ZT : 4;
    uint32_t DI : 2;
    uint32_t nBd : 1;
    uint32_t Gr : 1;
    uint32_t KodOO : 6;
    uint32_t PBL : 1;
    uint32_t Bd : 1;
};

// Коды достоверности признаковой информации в режимах "Пароль" 4 и 6. (поле DI)
enum
{
    DICode_Apsent = 0, // Отсутствует
    DICode_Corrected,  // Исправлена
    DICode_Uncertain,  // Недостоверна
    DICode_Reliable    // Достоверна
};

//-----------------------------------------------------------------------------

union _RBS_InfoCode
{
    struct
    {
        uint32_t D1 : 1;
        uint32_t D2 : 1;
        uint32_t D4 : 1;
        uint32_t C1 : 1;
        uint32_t C2 : 1;
        uint32_t C4 : 1;
        uint32_t B1 : 1;
        uint32_t B2 : 1;
        uint32_t B4 : 1;
        uint32_t A1 : 1;
        uint32_t A2 : 1;
        uint32_t A4 : 1;
        uint32_t X : 1;
    };
    uint32_t code;
};
using RBS_InfoCode = union _RBS_InfoCode;

struct _RBS_ModeOther
{
    uint32_t Code : 13;
    uint32_t SPI : 1;
    uint32_t : 2;
    uint32_t nCode : 13;
    uint32_t nSPI : 1;
    uint32_t PBL : 1;
    uint32_t AW : 1;
};

struct _RBS_Mode4
{
    uint32_t KodM4 : 15;
    uint32_t POKM4 : 4;
    uint32_t : 3;
    uint32_t Gr : 1;
    uint32_t : 6;
    uint32_t PBL : 1;
    uint32_t : 1;
};

union _S_Data
{
    uint32_t data[4];
    struct
    {
        uint32_t : 32;
        uint32_t : 32;
        uint32_t : 32;
        uint32_t : 16;
        uint32_t BDS2 : 4;
        uint32_t BDS1 : 4;
        uint32_t sicAllCall : 2;
        uint32_t PC_type : 2;
        uint32_t PC_EC : 2;
        uint32_t : 2;
    };
};

// FIXME (oprikov_el): костыльно добавлены данных для S режима путем выравнивания до 4 слов
union ExtendedInfo
{
    struct // simple
    {
        union
        {
            struct _Peleng_HIP peleng_HIP;
            struct _Peleng_ASHP peleng_ASHP;

            struct _Parol_Alert parol_modeAlert;
            struct _Parol_Mode1 parol_mode1;
            struct _Parol_Mode2 parol_mode2;
            struct _Parol_Mode3 parol_mode3;
            struct _Parol_Mode4 parol_mode4;
            struct _Parol_Mode6 parol_mode6;

            struct _RBS_ModeOther rbs_modeOther; //+
            struct _RBS_Mode4 rbs_mode4;
            uint32_t _ser_value;
        };
        uint32_t _reserved[4 - 1];
    };

    _S_Data modeS; //+
};


////////////////////////////ResponseSignalInfo//////////////////////////////
union ResponseSignalInfo
{
    enum
    {
        TotalWords = 4
    };
    enum
    {
        ExtendedWords = 3
    }; // FIXME (orpikov_el): костыль по выравниванию

    ResponseSignalInfo()
    {
        std::fill(std::begin(values), std::end(values), 0);
    }

    /* Для каждого ответного сигнала */
    struct //?
    {
        uint32_t Dos : 20; /* дальность ОС (кроме пеленгов АШП/ХИП)*/
        uint32_t dDS : 10; /* Уменьшение значения Dos при расчете поправки на азимут (только для ОС режима S)*/
        uint32_t ASP : 1;  /* признак пеленга АШП */
        uint32_t HIP : 1;  /* признак пеленга ХИП */

        interrogator_v3::Mode mode = interrogator_v3::Mode::None;

        ExtendedInfo exInfo;
    };
    uint32_t values[TotalWords + ExtendedWords];
};

} // namespace POI


#endif /* POIPROTOCOL_H */
