#pragma once


#include "domain/ICpConsumers.h"


class ParolModule
{
public:
    virtual ~ParolModule() = default;

    void addConsumer(IParolConsumer * newConsumer)
    {
        m_consumers.push_back(newConsumer);
    }


protected:
    std::vector<IParolConsumer *> m_consumers;
};


class AtcrbsSModule
{
public:
    virtual ~AtcrbsSModule() = default;

    void addConsumer(IRbs_SConsumer * newConsumer)
    {
        m_consumers.push_back(newConsumer);
    }


protected:
    std::vector<IRbs_SConsumer *> m_consumers;
};


class Pdp
{
public:
    virtual ~Pdp() = default;

    ParolModule * moduleParol() const noexcept
    {
        return parolModule;
    }

    AtcrbsSModule * moduleAtcrbs() const noexcept
    {
        return atcrbsSModule;
    }


protected:
    AtcrbsSModule * atcrbsSModule = nullptr;
    ParolModule * parolModule = nullptr;
};
