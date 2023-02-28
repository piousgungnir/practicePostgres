#include "RemoteAtcrbsSModule.h"

#include "PdpClient.h"

#include "MakeAnswer.h"

template<typename T>
impl::CoordinatePoint makeBaseCoordinatePoint(T & msg)
{
    static_assert(
        std::is_base_of<pdp::AtcrbsCoordinatePoint, T>::value || std::is_base_of<pdp::AtcrbsCoordinatePoint_v2, T>::value,
        "T must be pdp::AtcrbsCoordinatePoint or pdp::AtcrbsCoordinatePoint_v2");

    impl::CoordinatePoint baseCp;
    baseCp.rejectedByTagc = msg.rejectedByVaru;
    baseCp.range = msg.range;
    baseCp.azimuth = msg.azimuth;
    baseCp.amplitude = msg.amplitude;
    baseCp.xImpulse = msg.xImpulse;
    baseCp.mode = SystemMode(static_cast<int>(msg.mode));
    baseCp.unitCode = static_cast<int>(msg.unitCode);
    baseCp.bortNumber = static_cast<int>(msg.bortNumber);
    baseCp.altitude = static_cast<int>(msg.altitude);
    baseCp.spi = msg.spi;
    baseCp.militaryAlarm = msg.militaryAlarm;

    baseCp.mark._1.code = msg.mode_1.code;
    baseCp.mark._1.confidenceCode = msg.mode_1.confidenceCode;
    baseCp.mark._2.code = msg.mode_2.code;
    baseCp.mark._2.confidenceCode = msg.mode_2.confidenceCode;
    baseCp.mark._3A.code = msg.mode_3A.code;
    baseCp.mark._3A.confidenceCode = msg.mode_3A.confidenceCode;
    baseCp.mark._C.code = msg.mode_C.code;
    baseCp.mark._C.confidenceCode = msg.mode_C.confidenceCode;

    // mode S
    baseCp.modeS.AA = msg.modes.AA;
    baseCp.modeS.identity = static_cast<int>(msg.modes.identity);
    baseCp.modeS.altitude = static_cast<int>(msg.modes.altitude);
    baseCp.isRollCall = msg.isRollCall;

    //bds

    auto toMbData = [](const auto & msg)
    {
        std::vector<impl::CoordinatePoint::MBData> mbData;

        for (size_t i = 0; i < msg.size(); i++)
        {
            auto bds = msg.at(i);
            impl::CoordinatePoint::MBData mb;
            mb.BDS1 = bds.bds1;
            mb.BDS2 = bds.bds2;
            mb.mb = bds.mb;
            auto timeMs = std::chrono::system_clock::time_point{std::chrono::milliseconds{bds.timeStamp}};
            mb.timestamp = timeMs;
            mbData.push_back(mb);
        }
        return mbData;
    };

    baseCp.modeS.mbdata = toMbData(msg.modes.Bds.actualBds);
    baseCp.modeS.outdatedBds = toMbData(msg.modes.Bds.outdatedBds);

    baseCp.modeS.supportedBds = msg.modes.Bds.supportedBds;
    baseCp.modeS.usefullDataBds = msg.modes.Bds.usefullDataBds;
    baseCp.modeS.nextRequests = msg.modes.Bds.nextRequests;
    baseCp.modeS.recentlyUpdatedBds = msg.modes.Bds.recentlyUpdatedBds;
    baseCp.modeS.unrequestedBds = msg.modes.Bds.unrequestedBds;

    return baseCp;
}

void RemoteAtcrbsSModule::add(pdp::AtcrbsCoordinatePoint & msg)
{
    auto cp = makeBaseCoordinatePoint(msg);

    for (uint32_t i = 0; i < msg.packsNum; ++i)
        cp.pack.push_back(makeAnswer(msg.pack[i]));

    for (auto && consumer : m_consumers)
        consumer->addCp(cp);
}

void RemoteAtcrbsSModule::add(pdp::AtcrbsCoordinatePoint_v2 & msg)
{
    auto cp = makeBaseCoordinatePoint(msg);

    for (uint32_t i = 0; i < msg.packsNum; ++i)
        cp.pack.push_back(makeAnswer(msg.pack[i],
                                     std::chrono::system_clock::time_point(std::chrono::nanoseconds(msg.pack[i].timestamp))));

    for (auto && consumer : m_consumers)
        consumer->addCp(cp);
}
