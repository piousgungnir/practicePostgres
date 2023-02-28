#include "PdpClient.h"

#include <cassert>
#include <type_traits>

#include "cctype"

#include <pdp/AlarmBearing.h>
#include <pdp/AtcrbsCoordinatePoint.h>
#include <pdp/AtcrbsCoordinatePoint_v2.h>
#include <pdp/EsgrloCoordinatePoint.h>
#include <pdp/EsgrloCoordinatePoint_v2.h>
//#include <pdp/MainReceiver.h>
//#include <remote-interrogator/FieldMonitorCmdRequest.h>
//#include <remote-interrogator/PacketRequest.h>

//#include "misc/InstanceGenerator.h"

#include "MakeAnswer.h"

constexpr std::array<int32_t, 9> PdpClient::handledTypes;

PdpClient::PdpClient(RemotePdp * pdp, IRadioInfoConsumer * radioConsumer, ISdpRequestsContainer * sdpRequestsContainer)
    : pdp(pdp)
    , sdpRequestsContainer(sdpRequestsContainer)
    , radioConsumer(radioConsumer)
{
    assert(pdp != nullptr);
    rbs = dynamic_cast<RemoteAtcrbsSModule *>(pdp->moduleAtcrbs());
    // assert(rbs != nullptr);
    parol = dynamic_cast<RemoteParolModule *>(pdp->moduleParol());
    // assert(parol != nullptr);
    // assert(radioConsumer != nullptr);
}

/*
void PdpClient::addFieldMonitorCmd(uint32_t id, uint32_t aa, const stdssr::ModeS::sse::FieldMonitorCmd & cmd)
{
    pdp::FieldMonitorCmdRequest msg;
    msg.id = id;
    msg.AA = aa;
    msg.cmd.pc = cmd.pc;
    msg.cmd.bds = cmd.bds;
    msg.cmd.ma = cmd.ma;

    send(msg);
}

void PdpClient::addPacket(uint32_t id, uint32_t aa, const stdssr::ModeS::sse::Packet & packet)
{
    pdp::PacketRequest msg;
    msg.id = id;
    msg.AA = aa;
    msg.pack.ma = packet.message;

    send(msg);
}
*/

void PdpClient::onData(int32_t typecode, const std::vector<uint8_t> & data)
{
    bool canHandled = std::any_of(handledTypes.begin(), handledTypes.end(), [typecode](auto v)
                                  { return v == typecode; });
    if (not canHandled)
        return;

    std::vector<uint8_t> memory(data);
    BitStream stream(memory);
    switch (typecode)
    {
    case pdp::Answer::MessageTypeCode:
    {
        pdp::Answer msg;
        stream >> msg;
        if (radioConsumer)
            radioConsumer->setRadioInfo(makeAnswer(msg));
        break;
    }
    case pdp::Answer_v2::MessageTypeCode:
    {
        pdp::Answer_v2 msg;
        stream >> msg;
        if (radioConsumer)
            radioConsumer->setRadioInfo(makeAnswer(msg,
                                                   std::chrono::system_clock::time_point(std::chrono::nanoseconds(msg.timestamp))));
        break;
    }
    case pdp::AtcrbsCoordinatePoint::MessageTypeCode:
    {
        pdp::AtcrbsCoordinatePoint msg;
        stream >> msg;
        if (rbs)
            rbs->add(msg);
        break;
    }
    case pdp::AtcrbsCoordinatePoint_v2::MessageTypeCode:
    {
        pdp::AtcrbsCoordinatePoint_v2 msg;
        stream >> msg;
        if (rbs)
            rbs->add(msg);
        break;
    }
    case pdp::EsgrloCoordinatePoint::MessageTypeCode:
    {
        pdp::EsgrloCoordinatePoint msg;
        stream >> msg;
        if (parol)
            parol->add(msg);
        break;
    }
    case pdp::EsgrloCoordinatePoint_v2::MessageTypeCode:
    {
        pdp::EsgrloCoordinatePoint_v2 msg;
        stream >> msg;
        if (parol)
            parol->add(msg);
        break;
    }
    case pdp::AlarmBearing::MessageTypeCode:
    {
        pdp::AlarmBearing msg;
        stream >> msg;
        if (parol)
            parol->add(msg);
        break;
    }
    case sdp::ExternalTargetDesignation::MessageTypeCode:
    {
        sdp::ExternalTargetDesignation msg;
        stream >> msg;

        if (sdpRequestsContainer)
            sdpRequestsContainer->push(msg);

        break;
    }
    default:
    {
        //        qDebug() << "not valid type" << type << "in" << __PRETTY_FUNCTION__;
    }
    }
}

template<typename T>
void PdpClient::send(T message)
{
    std::vector<uint8_t> memory;
    BitStream stream(memory);
    stream << message;
    ipc::Process::send(T::MessageTypeCode, memory.data(), memory.size());
}
