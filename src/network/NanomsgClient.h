#pragma once

#include <atomic>
#include <thread>

#include "IPCPoint.h"

using Url = std::string;

class NanomsgPipelineClient : public ipc::Communication
{

public:
    explicit NanomsgPipelineClient(const Url & pipelineUrl);
    ~NanomsgPipelineClient() override;

    // Communication interface
    void send(int32_t typecode, const uint8_t * data, std::size_t size) override;

private:
    void subscribeAndListen_();

private:
    std::thread subscribeThread_;
    Url subscribeUrl_;
    int subscribeSocket_;

    std::atomic_bool isRunning_{true};
};
