#pragma once


// Класс Azimuth хранит в себе в 16-битном безнаковом представлении код азимута
// 16 битный код способен представить азимут с точностью 0,005493164 градуса = 0,329589844 минут = 19,775390625 секунд
// Класс предоставляет методы для построения себя из градусов, радианов и беззнакового кода с произвольным битовым
// разрешением в пределах от 1 до 64 бит.
// При простоении из радиан или градусов производится проверка переданного аргумента и может быть брошено
// исключение std::invalid_argument в случаях когда аргумент nan или infinite. С переданных аргументов
// снимается возможная переодичность и значения переносятся в положительную область.
// Класс предоставляет арифметические операции над азимутом. Операции деления и умножения определены для
// double, реализуя семантику увеличение/уменьшение в разы, а не деление Азимутов друг на друга.
// Эти операции могут бросать исключения std::invalid_argument в случаях когда переданный аргумент nan или infinite
// Класс предоставляет возможности перевода внутреннего представления в градусы, радианы и азимутальный код
// с битовой разрядностью от 1 до 64
// Метод inRange позволяет опрделить входит ли текущий азимут в переданный диапозон с включением границ
// Определены только == и != операторы сравнения, остальные не имеют смысла ввиду цикличности азимута
//
// Предоставляются пользовательские литералы определенные в namespace azimuth::literals для построения Azimuth


#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <type_traits>


namespace azimuth
{


constexpr double  FullCircleRadians = 2 * M_PI;
constexpr double  FullCircleDegrees = 360;


template <typename T, typename R>
constexpr R normalizeAzimuth(T value, T fullCircle) noexcept
{
    static_assert (std::is_arithmetic<T>(), "T must be arithmetic type");
    static_assert (std::is_arithmetic<R>(), "R must be arithmetic type");

    while(value < 0)
        value += fullCircle;
    while(value >= fullCircle)
        value -= fullCircle;
    return static_cast<R>(value);
}


} // namespace azimuth


class Azimuth
{
    using UnderlyingType = uint16_t;

    constexpr static int32_t FullCircleCode    = (1 << 16);
    constexpr static double  FromRadiansRatio  = FullCircleCode / azimuth::FullCircleRadians;
    constexpr static double  FromDegreesRatio  = FullCircleCode / azimuth::FullCircleDegrees;

    friend constexpr bool operator == (const Azimuth& left, const Azimuth& right) noexcept;
    friend constexpr bool operator != (const Azimuth& left, const Azimuth& right) noexcept;

    friend constexpr Azimuth operator + (const Azimuth& left, const Azimuth& right) noexcept;
    friend constexpr Azimuth operator - (const Azimuth& left, const Azimuth& right) noexcept;
    friend Azimuth operator * (const Azimuth& az, double n);
    friend Azimuth operator / (const Azimuth& az, double n);

public:
    constexpr static int32_t minAzimuthValue = 0;
    constexpr static int32_t maxAzimuthValue = FullCircleCode - 1;

public:
    static constexpr Azimuth fromDegrees(double degrees);
    static constexpr Azimuth fromRadians(double radians);

    template<uint8_t BitResolution>
    constexpr static Azimuth fromAzimuthCode(uint64_t code) noexcept;

    constexpr Azimuth() = default;
    constexpr explicit Azimuth(uint16_t azimuthCode) noexcept;

    constexpr uint16_t azimuthCode() const noexcept;
    constexpr double toDegrees() const noexcept;
    constexpr double toRadians() const noexcept;

    template<uint8_t BitResolution, typename R = uint64_t>
    constexpr R toAzimuthCode() const noexcept;
    
    constexpr bool inRange(const Azimuth& left, const Azimuth& rigth, bool includeLeft=true, bool includeRigth=true) const noexcept;
    
    constexpr Azimuth& operator += (const Azimuth& other) noexcept;
    constexpr Azimuth& operator -= (const Azimuth& other) noexcept;
    Azimuth& operator *= (double n);
    Azimuth& operator /= (double n);

private:
    UnderlyingType azimuth = 0;
};


#if defined(__clang__)

#elif  defined(__GNUC__) && ((__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 60100)
    #error "unsupported GCC version - see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67371"
#endif
constexpr Azimuth Azimuth::fromDegrees(double degrees)
{
    Azimuth azimuth;
    auto fp_type = std::fpclassify(degrees);
    if (fp_type == FP_NORMAL or fp_type == FP_ZERO)
    {
        degrees = azimuth::normalizeAzimuth<double, double>(degrees, azimuth::FullCircleDegrees);
        azimuth.azimuth = static_cast<UnderlyingType>(degrees * FromDegreesRatio + 0.5);
    }
    else
    {
        throw std::invalid_argument("Azimuth: degrees abnormal value, nan or infinite");
    }
    return azimuth;
}


#if defined(__clang__)

#elif  defined(__GNUC__) && ((__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 60100)
    #error "unsupported GCC version - see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67371"
#endif
constexpr Azimuth Azimuth::fromRadians(double radians)
{
    Azimuth azimuth;
    auto fp_type = std::fpclassify(radians);
    if (fp_type == FP_NORMAL or fp_type == FP_ZERO)
    {
        radians = azimuth::normalizeAzimuth<double, double>(radians, azimuth::FullCircleRadians);
        azimuth.azimuth = static_cast<UnderlyingType>(radians * FromRadiansRatio + 0.5);
    }
    else
    {
        throw std::invalid_argument("Azimuth: radians abnormal value, nan or infinite");
    }
    return azimuth;
}


template<uint8_t BitResolution>
constexpr Azimuth Azimuth::fromAzimuthCode(uint64_t code) noexcept
{
    static_assert (0 < BitResolution and BitResolution <= 64, "Azimuth: BitResolution must be non zero value in range [1, 64]");

    Azimuth azimuth;
    constexpr int8_t scale = 16 - BitResolution;
    code = (scale > 0) ? code << scale : code >> (-scale);
    azimuth.azimuth = azimuth::normalizeAzimuth<int32_t, UnderlyingType>(code & 0xFFFF, FullCircleCode);
    return azimuth;
}

template<>
constexpr Azimuth Azimuth::fromAzimuthCode<16>(uint64_t code) noexcept
{
    Azimuth azimuth(azimuth::normalizeAzimuth<int32_t, UnderlyingType>(code & 0xFFFF, FullCircleCode));
    return azimuth;
}


constexpr Azimuth::Azimuth(uint16_t azimuthCode) noexcept
    : azimuth(azimuthCode)
{
}


constexpr uint16_t Azimuth::azimuthCode() const noexcept
{
    return azimuth;
}


constexpr double Azimuth::toDegrees() const noexcept
{
    return azimuth / FromDegreesRatio;
}


constexpr double Azimuth::toRadians() const noexcept
{
    return azimuth / FromRadiansRatio;
}


template<uint8_t BitResolution, typename R>
constexpr R Azimuth::toAzimuthCode() const noexcept
{
    static_assert (0 < BitResolution and BitResolution <= 64, "Azimuth: BitResolution must be non zero value in range [1, 64]");
    static_assert (std::is_integral<R>(), "Azimuth: return type R must be integral type");
    static_assert (BitResolution <= sizeof(R) * 8, "Azimuth: bit resolution more than type R can store");

    uint64_t code = azimuth;
    constexpr int8_t scale = 16 - BitResolution;
    code = (scale > 0) ? code >> scale : code << (-scale);
    return static_cast<R>(code);
}

template<>
constexpr uint16_t Azimuth::toAzimuthCode<16>() const noexcept
{
    return azimuth;
}


constexpr bool operator == (const Azimuth& left, const Azimuth& right) noexcept
{
    return left.azimuth == right.azimuth;
}


constexpr bool operator != (const Azimuth& left, const Azimuth& right) noexcept
{
    return left.azimuth != right.azimuth;
}


constexpr bool Azimuth::inRange(const Azimuth& left, const Azimuth& right, bool includeLeft, bool includeRigth) const noexcept
{
    auto rValue = static_cast<uint16_t>(right.azimuth - left.azimuth);
    auto cValue = static_cast<uint16_t>(azimuth - left.azimuth);

    bool inRange = (cValue <= rValue);
    bool inLeft = (azimuth == left.azimuth);
    bool inRight = (azimuth == right.azimuth);

    bool falseOnBorder = (not includeLeft and inLeft) or (not includeRigth and inRight);
    bool result = inRange and not falseOnBorder;

    return result;
}


constexpr Azimuth& Azimuth::operator +=(const Azimuth& other) noexcept
{
    azimuth += other.azimuth;
    return *this;
}


constexpr Azimuth &Azimuth::operator -=(const Azimuth &other) noexcept
{
    azimuth -= other.azimuth;
    return *this;
}


constexpr Azimuth operator + (const Azimuth& left, const Azimuth& right) noexcept
{
    return Azimuth(left.azimuth + right.azimuth);
}


constexpr Azimuth operator - (const Azimuth& left, const Azimuth& right) noexcept
{
    return Azimuth(left.azimuth - right.azimuth);
}


namespace azimuth
{


namespace literals
{


/**
 * @brief Пользовательский литерал. Преобразует число из градусов в Azimuth
 * @param value - значение в градусах
 * @return Azimuth
 */
constexpr Azimuth operator"" _deg(long double value)
{
    return Azimuth::fromDegrees(static_cast<double>(value));
}


constexpr Azimuth operator"" _deg(unsigned long long value)
{
    return Azimuth::fromDegrees(static_cast<double>(value));
}

/**
 * @brief Пользовательский литерал. Преобразует число из радианов в Azimuth
 * @param value - значение в радианах
 * @return Azimuth
 */
constexpr Azimuth operator"" _rad(long double value)
{
    return Azimuth::fromRadians(static_cast<double>(value));
}


constexpr Azimuth operator"" _rad(unsigned long long value)
{
    return Azimuth::fromRadians(static_cast<double>(value));
}


} // namespace literals


} // namespace azimuth


namespace azimuth
{

static const Azimuth minAzimuth = Azimuth::fromAzimuthCode<16>(Azimuth::minAzimuthValue);
static const Azimuth maxAzimuth = Azimuth::fromAzimuthCode<16>(Azimuth::maxAzimuthValue);

} // namespace azimuth
