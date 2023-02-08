#ifndef IAZIMUTHCONSUMER_H
#define IAZIMUTHCONSUMER_H

#include "Azimuth.h"
class IAzimuthConsumer
{
public:
    virtual ~IAzimuthConsumer() = default;
    virtual void setAzimuth(const Azimuth &azimuth) = 0;
};
#endif // IAZIMUTHCONSUMER_H
