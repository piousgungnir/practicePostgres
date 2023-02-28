#pragma once


#include "Pdp.h"
#include <pdp/EsgrloCoordinatePoint.h>
#include <pdp/EsgrloCoordinatePoint_v2.h>
#include <pdp/AlarmBearing.h>

class RemoteParolModule : public ParolModule
{
public:
    RemoteParolModule();

    void add(pdp::EsgrloCoordinatePoint & msg);
    void add(pdp::EsgrloCoordinatePoint_v2 & msg);

    void add(const pdp::AlarmBearing & msg);
};

