#pragma once

#include "IPCPoint.h"

using Url = std::string;

class NanomsgPipelineServer : public ipc::Communication
{

public:
    explicit NanomsgPipelineServer(const Url & publisherUrl);
    ~NanomsgPipelineServer();

    // Communication interface
    void send(int32_t typecode, const uint8_t * data, std::size_t size) override;

private:
    Url publisherUrl_ = "inproc://block_publisher";
    int pub_;
};
