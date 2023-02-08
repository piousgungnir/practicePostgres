#pragma once

#include "POIProtocol.h"
#include "Timestamp.h"

namespace impl
{

/**
 * @brief The PelengatedInfo struct
 *
 * Информация полученная пеленгационным способом
 */
struct PelengatedInfo
{
    double distance;                                /// [м] дальность обнаруженного ответа
    double azimuth;                                 /// [рад] азимут обнаруженного ответа
    double phase;                                   /// [рад] разность фаз между суммарным и разностным каналами

    struct
    {
        uint32_t sum;
        uint32_t dif;
        uint32_t pbl;
    } amplitude;
};

struct RawAnswer
{
    Timestamp timestamp;
    PelengatedInfo radio;
    POI::ResponseSignalInfo info;
};

struct Answer
{
    explicit Answer(const RawAnswer& rawAnswer) noexcept
    : raw(rawAnswer)
    {
        //monopulseAzimuth = monopulseCorrection = phase = distance = azimuthFromDistance = 0;
    }
    const RawAnswer raw;
    double monopulseAzimuth = 0; /// [рад]
    double monopulseCorrection = 0;
    double phase = 0;
    double distance = 0; /// [м]
    double azimuthFromDistance = 0;
};

} // namespace impl

