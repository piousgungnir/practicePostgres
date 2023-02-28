#pragma once


#include "Pdp.h"

#include "pdp/AtcrbsCoordinatePoint.h"
#include "pdp/AtcrbsCoordinatePoint_v2.h"


class RemoteAtcrbsSModule : public AtcrbsSModule
{
public:
    void add(pdp::AtcrbsCoordinatePoint & msg);
    void add(pdp::AtcrbsCoordinatePoint_v2 & msg);
};
