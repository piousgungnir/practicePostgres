#pragma once

#include <sdp/ExternalTargetDesignation.h>

class ISdpRequestsContainer
{
public:
    virtual ~ISdpRequestsContainer() = default;

    virtual void push(sdp::ExternalTargetDesignation const&) = 0;
};


