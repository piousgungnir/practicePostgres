#pragma once

#include <chrono>
#include <memory>

#include <pdp/Answer.h>
#include <pdp/Answer_v2.h>

#include "RemotePdp.h"


template<typename AnswerType>
std::shared_ptr<impl::Answer> makeAnswer(AnswerType const & answer,
                                         std::chrono::system_clock::time_point ts = std::chrono::system_clock::now())
{
    static_assert(std::is_base_of<pdp::Answer, AnswerType>::value || std::is_base_of<pdp::Answer_v2, AnswerType>::value,
                  "AnswerType must be pdp::Answer or pdp::Answer_v2");

    impl::PelengatedInfo radio;
    radio.distance = answer.radio.distance;
    radio.azimuth = answer.radio.azimuth;
    radio.phase = answer.radio.phase;
    radio.amplitude.sum = answer.radio.amplitude.sum;
    radio.amplitude.dif = answer.radio.amplitude.dif;
    radio.amplitude.pbl = answer.radio.amplitude.pbl;
    POI::ResponseSignalInfo info;
    info.Dos = answer.info.Dos;
    info.dDS = answer.info.dDS;
    info.ASP = answer.info.ASP;
    info.HIP = answer.info.HIP;
    info.mode = static_cast<interrogator_v3::Mode>(answer.info.mode.value());
    info.exInfo._ser_value = answer.info.exInfo.otherModeData;

    auto exInfo = answer.info.exInfo;
    for (uint modesIndex = 0; modesIndex < 4; ++modesIndex)
        info.exInfo.modeS.data[modesIndex] = exInfo.modeS[modesIndex];

    auto baseAnswer = std::make_shared<impl::Answer>(impl::RawAnswer{ts, radio, info});

    baseAnswer->monopulseAzimuth = answer.monopulseAzimuth;
    baseAnswer->monopulseCorrection = answer.monopulseCorrection;
    baseAnswer->phase = answer.phase;
    baseAnswer->distance = answer.distance;

    return baseAnswer;
}
