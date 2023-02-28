#include "RemoteParolModule.h"

#include "PdpClient.h"
#include "MakeAnswer.h"


RemoteParolModule::RemoteParolModule()
{

}

template <typename T>
esgrlo::CoordinatePoint makeBaseCoordinatePoint(T & msg)
{
    static_assert(
        std::is_base_of<pdp::EsgrloCoordinatePoint, T>::value || std::is_base_of<pdp::EsgrloCoordinatePoint_v2, T>::value,
        "T must be pdp::AtcrbsCoordinatePoint or pdp::AtcrbsCoordinatePoint_v2");

    esgrlo::CoordinatePoint baseCp;
    baseCp.rejectedByTagc = msg.rejectedByVaru;
    baseCp.range = msg.range;
    baseCp.azimuth = msg.azimuth;
    baseCp.amplitude = msg.amplitude;
    baseCp.heigth = static_cast<int>(msg.height);
    baseCp.bortNumber = static_cast<int>(msg.bortNumber);
    baseCp.fuelCapacity = msg.fuelCapacity;
    baseCp.disaster = msg.disaster;
    baseCp.infoDisaster = msg.infoDisaster;
    baseCp.ur = msg.ur;
    baseCp.di_4 = msg.di_4;
    baseCp.di_6 = msg.di_6;
    baseCp.isFriendCommonRecognition = msg.isFriendCommonRecognition;
    baseCp.isFriendGuaranteedRecognition = msg.isFriendGuaranteedRecognition;
    baseCp.isFriendIndividualRecognition = msg.isFriendIndividualRecognition;
    baseCp.M13 = static_cast<int>(msg.M13);
    baseCp.M46 = static_cast<int>(msg.M46);

    return baseCp;
}

void RemoteParolModule::add(pdp::EsgrloCoordinatePoint &msg)
{
    auto cp = makeBaseCoordinatePoint(msg);

    for (uint32_t i = 0; i < msg.packsNum; ++i)
        cp.pack.push_back(makeAnswer(msg.pack[i]));

    for (auto&& consumer : m_consumers)
        consumer->addCp(cp);
}

void RemoteParolModule::add(pdp::EsgrloCoordinatePoint_v2 &msg)
{
    auto cp = makeBaseCoordinatePoint(msg);

    for (uint32_t i = 0; i < msg.packsNum; ++i)
        cp.pack.push_back(makeAnswer(msg.pack[i],
                                     std::chrono::system_clock::time_point(std::chrono::nanoseconds(msg.pack[i].timestamp))));

    for (auto&& consumer : m_consumers)
        consumer->addCp(cp);
}


void RemoteParolModule::add(const pdp::AlarmBearing & msg)
{
    Azimuth azimuth = Azimuth::fromDegrees(msg.azimuth.value());

    for (auto&& consumer : m_consumers)
    {
        consumer->addAlarmBearing(azimuth);
    }
}
