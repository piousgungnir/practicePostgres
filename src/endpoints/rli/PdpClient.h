#ifndef PDPCLIENT_H
#define PDPCLIENT_H

#include <pdp/Answer.h>
#include <pdp/Answer_v2.h>
#include <sdp/ExternalTargetDesignation.h>

//#include <stdssr/ModeS/sse/SpecificServicesEntity.h>
#include <network/IPCPoint.h>

#include "RemotePdp.h"
#include "domain/IRadioInfoConsumer.h"
#include "domain/ISdpRequestsContainer.h"

#include <cctype>
#include <array>

class PdpClient
    : /*public stdssr::ModeS::sse::IFieldMonitorController
    , public stdssr::ModeS::sse::IPacketsSender
    ,*/
      public ipc::Process
{
public:
    explicit PdpClient(RemotePdp * pdp,
                       IRadioInfoConsumer * radioConsumer,
                       ISdpRequestsContainer * sdpRequestsContainer);

    // IFieldMonitorController interface
    // void addFieldMonitorCmd(uint32_t id, uint32_t aa, const stdssr::ModeS::sse::FieldMonitorCmd & cmd) override;

    // IPacketsSender interface
    // void addPacket(uint32_t id, uint32_t aa, const stdssr::ModeS::sse::Packet & packet) override;

    // Process interface
    void onData(int32_t typecode, const std::vector<uint8_t> & data) override;

private:
    template<typename T>
    void send(T message);

public:
    static constexpr std::array<int32_t, 9> handledTypes = {
        pdp::Answer::MessageTypeCode,
        pdp::Answer_v2::MessageTypeCode,
        pdp::AtcrbsCoordinatePoint_v2::MessageTypeCode,
        pdp::EsgrloCoordinatePoint_v2::MessageTypeCode,
        pdp::AtcrbsCoordinatePoint::MessageTypeCode,
        pdp::EsgrloCoordinatePoint::MessageTypeCode,
        pdp::AlarmBearing::MessageTypeCode,
        sdp::ExternalTargetDesignation::MessageTypeCode,
    };

private:
    RemotePdp * pdp = nullptr;
    RemoteAtcrbsSModule * rbs = nullptr;
    RemoteParolModule * parol = nullptr;
    ISdpRequestsContainer * sdpRequestsContainer = nullptr;
    IRadioInfoConsumer * radioConsumer = nullptr;
};


#endif // PDPCLIENT_H
