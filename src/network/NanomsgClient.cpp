#include "NanomsgClient.h"

#include <iostream>

#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/pubsub.h>

auto printErr = [](const std::string & str)
{ std::cerr << str.c_str() << ": " << nn_strerror(nn_errno()) << std::endl; };

NanomsgPipelineClient::NanomsgPipelineClient(const Url & pipelineUrl)
    : subscribeUrl_(pipelineUrl)
{
    subscribeThread_ = std::thread(&NanomsgPipelineClient::subscribeAndListen_, this);
}

NanomsgPipelineClient::~NanomsgPipelineClient()
{
    isRunning_.store(false);

    if (subscribeThread_.joinable())
        subscribeThread_.join();
}

void NanomsgPipelineClient::send(int32_t /*typecode*/, const uint8_t * /*data*/, std::size_t /*size*/)
{
}

void NanomsgPipelineClient::subscribeAndListen_()
{
    {
        if ((subscribeSocket_ = nn_socket(AF_SP, NN_SUB)) < 0)
        {
            printErr("nn_sub_socket");
            return;
        }
        if ((nn_connect(subscribeSocket_, subscribeUrl_.c_str())) < 0)
        {
            printErr("nn_sub_connect");
            return;
        }

        if (nn_setsockopt(subscribeSocket_, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0)
        {
            printErr("nn_sub_setsockopt");
            return;
        }

        int to = 100;
        if (nn_setsockopt(subscribeSocket_, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) < 0)
        {
            printErr("nn_sub_setsockopt nn_rcvtimeo");
            return;
        }
    }

    char * buf = nullptr;
    int bytes = -1;
    while (isRunning_.load())
    {
        bytes = nn_recv(subscribeSocket_, &buf, NN_MSG, 0);
        if (bytes < 4)
        {
            continue;
        }
        {
            int32_t typecode = *(reinterpret_cast<int32_t *>(buf));
            std::vector<uint8_t> data(buf + 4, buf + bytes);
            if (isRunning_.load())
            {
                ipc::Communication::onData(typecode, data);
            }
        }
        nn_freemsg(buf);
    }
    nn_close(subscribeSocket_);
    nn_shutdown(subscribeSocket_, 0);
}
