#include "Azimuth.h"


Azimuth& Azimuth::operator *=(double n)
{
    if (not std::signbit(n))
    {
        auto fp_type = std::fpclassify(n);
        if (fp_type == FP_NORMAL or fp_type == FP_ZERO)
        {
            auto value = static_cast<uint32_t>(std::lround(azimuth * n));
            azimuth = azimuth::normalizeAzimuth<uint32_t, UnderlyingType>(value, FullCircleCode);
        }
        else
        {
            throw std::invalid_argument("Azimuth: multiply operand not normal value, it is nan or infinite");
        }
    }
    else
    {
        throw std::invalid_argument("Azimuth: multiply operand not be less than 0");
    }
    return *this;
}


Azimuth &Azimuth::operator /=(double n)
{
    if (not std::signbit(n))
    {
        auto fp_type = std::fpclassify(n);
        if (fp_type == FP_NORMAL)
        {
            auto value = static_cast<uint32_t>(std::lround(azimuth / n));
            azimuth = azimuth::normalizeAzimuth<uint32_t, UnderlyingType>(value, FullCircleCode);
        }
        else
        {
            throw std::invalid_argument("Azimuth: divider not normal value, it is nan, zero or infinite");
        }
    }
    else
    {
        throw std::invalid_argument("Azimuth: divider not be less than 0");
    }
    return *this;
}


Azimuth operator * (const Azimuth& az, double n)
{
    Azimuth azimuth = az;
    azimuth *= n;
    return azimuth;
}


Azimuth operator / (const Azimuth& az, double n)
{
    Azimuth azimuth = az;
    azimuth /= n;
    return azimuth;
}
