#ifndef MODES_V3_H
#define MODES_V3_H

#include <cstdint>

namespace interrogator_v3
{

enum class Mode : uint8_t
{
    ParolII             = 0x02,
    None                = 0x04,
    ParolI              = 0x10,
    ParolIII,                   //0x11
    ParolIV             = 0x1D,
    ParolVI,                    //0x1E

    Mode1               = 0x41,
    Mode2,
    Mode3A,
    ModeC               = 0x45,

    // Короткий P4
    Intermode1          = 0x49,
    Intermode2,
    IntermodeA,
    IntermodeC          = 0x4D,

    ModeS               = 0x50,
    // Комбинированый запрос Межрежимный только RBS и Режим S
    // TODO (oprikov_el): выпилить коды комбинированых режимов из публичной части запросчика.
    // Код комбинированного режима должен формироваться в реализации запросчика исходя из сочетания двух одиночных запросов
    Intermode1_ModeS,
    Intermode2_ModeS,
    IntermodeA_ModeS,
    IntermodeC_ModeS    = 0x55,

    // Длинный P4
    Intermode1_S        = 0x59,
    Intermode2_S,
    IntermodeA_S,
    IntermodeC_S        = 0x5D,

    Mode4               = 0x60,
    // Комбинированый запрос Режим 4 и Mark-XII
    Mode4_Mode1,
    Mode4_Mode2,
    Mode4_Mode3A,
    Mode4_ModeC         = 0x65

};


inline bool isP3(const Mode &mode) noexcept
{
    return mode == Mode::ParolIII;
}
inline bool isParol(const Mode &mode) noexcept
{
    return mode == Mode::ParolI or
           mode == Mode::ParolII or
           mode == Mode::ParolIII or
           mode == Mode::ParolIV or
           mode == Mode::ParolVI;
}
inline bool isMark(const Mode &mode) noexcept
{
    return mode == Mode::Mode1 or
           mode == Mode::Mode2 or
           mode == Mode::Mode4;
}
inline bool isRbs(const Mode &mode) noexcept
{
    return mode == Mode::Mode1 or
            mode == Mode::Mode2 or
            mode == Mode::Mode3A or
            mode == Mode::ModeC or
            mode == Mode::Mode4 or
            mode == Mode::Intermode1 or
            mode == Mode::Intermode1_S or
            mode == Mode::Intermode2 or
            mode == Mode::Intermode2_S or
            mode == Mode::IntermodeA or
            mode == Mode::IntermodeA_S or
            mode == Mode::IntermodeC or
            mode == Mode::IntermodeC_S;
}
inline bool canCombined(const Mode &mode) noexcept
{
    return mode == Mode::Mode1 or
           mode == Mode::Mode2 or
           mode == Mode::Mode3A or
           mode == Mode::ModeC;
}
inline bool isS(const Mode &mode) noexcept
{
    return mode == Mode::ModeS or
           mode == Mode::Intermode1_ModeS or
           mode == Mode::Intermode2_ModeS or
           mode == Mode::IntermodeA_ModeS or
           mode == Mode::IntermodeC_ModeS;
}

inline Mode makeCombined(const Mode &mode) noexcept
{
    Mode combinedMode;
    switch (mode) {
    case Mode::Mode1:
        combinedMode = Mode::Intermode1_ModeS;
        break;
    case Mode::Mode2:
        combinedMode = Mode::Intermode2_ModeS;
        break;
    case Mode::Mode3A:
        combinedMode = Mode::IntermodeA_ModeS;
        break;
    case Mode::ModeC:
        combinedMode = Mode::IntermodeC_ModeS;
        break;
    default:
        combinedMode = Mode::None;
    }
    return combinedMode;
}

inline constexpr bool isCombined(const Mode & mode) noexcept
{
    return mode == Mode::Intermode1_ModeS
        or mode == Mode::Intermode2_ModeS
        or mode == Mode::IntermodeA_ModeS
        or mode == Mode::IntermodeC_ModeS

        or mode == Mode::Mode4_Mode1
        or mode == Mode::Mode4_Mode2
        or mode == Mode::Mode4_Mode3A
        or mode == Mode::Mode4_ModeC;
}

inline constexpr Mode extractRbsMode(const Mode & mode) noexcept
{
    if      (mode == Mode::Intermode1_ModeS) return Mode::Intermode1;
    else if (mode == Mode::Intermode2_ModeS) return Mode::Intermode2;
    else if (mode == Mode::IntermodeA_ModeS) return Mode::IntermodeA;
    else if (mode == Mode::IntermodeC_ModeS) return Mode::IntermodeC;

    else if (mode == Mode::Mode4_Mode1) return Mode::Mode1;
    else if (mode == Mode::Mode4_Mode2) return Mode::Mode2;
    else if (mode == Mode::Mode4_Mode3A) return Mode::Mode3A;
    else if (mode == Mode::Mode4_ModeC) return Mode::ModeC;
    return mode;
}

} // end of namespace interrogator_v3

#endif // MODES_V3_H
