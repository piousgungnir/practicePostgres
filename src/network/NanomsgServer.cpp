#include "NanomsgServer.h"

#include <iostream>

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/pubsub.h>

auto printErr = [](const std::string & str)
{ std::cerr << str.c_str() << ": " << nn_strerror(nn_errno()) << std::endl; };

NanomsgPipelineServer::NanomsgPipelineServer(const Url & publisherUrl)
{
    if ((pub_ = nn_socket(AF_SP, NN_PUB)) < 0)
        printErr("mock block_pub");
    if (nn_bind(pub_, publisherUrl_.c_str()) < 0)
        printErr("mock block_pub");
}

NanomsgPipelineServer::~NanomsgPipelineServer()
{
    nn_close(pub_);
    nn_shutdown(pub_, 0);
}

void NanomsgPipelineServer::send(int32_t typecode, const uint8_t * data, std::size_t size)
{
    std::vector<uint8_t> sendedData(4);
    int32_t * ptr32 = reinterpret_cast<int32_t *>(sendedData.data());
    *ptr32 = typecode;
    sendedData.insert(sendedData.end(), data, data + size);

    nn_send(pub_, sendedData.data(), sendedData.size(), 0);
}
