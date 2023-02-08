#pragma once

#include "azimuth/Azimuth.h"

#include "CoordinatePoint.h"

struct asinfo_t;

class IParolConsumer
{
public:
    virtual void addAlarmBearing(const Azimuth &) = 0;
    virtual void addCp(const esgrlo::CoordinatePoint &) = 0;
    virtual ~IParolConsumer() = default;
};

class IRbs_SConsumer
{
public:
    virtual void addCp(const impl::CoordinatePoint &) = 0;
    virtual void addAsInfo(asinfo_t &) = 0;
    virtual ~IRbs_SConsumer() = default;
};

template<class Consumer>
class ICpDistributor
{
public:
    void addConsumer(Consumer *consumer)
    {
        m_consumers.push_back(consumer);
    }
    // TODO: test it!
    void removeConsumer(Consumer *deletedConsumer)
    {
        for (auto it = m_consumers.begin(); it != m_consumers.end(); it = std::next(it))
            if (*it == deletedConsumer)
            {
                m_consumers.erase(it);
                return;
            }
    }

    // void notifyConsumers(some args);

protected:
    std::vector<Consumer *> m_consumers;
};
