#pragma once

#include <memory>

#include "Group.h"

class IRadioInfoConsumer
{
public:
    virtual ~IRadioInfoConsumer() = default;
    virtual void setRadioInfo(const Response &) = 0;
};

