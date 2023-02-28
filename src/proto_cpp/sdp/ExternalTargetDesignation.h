#ifndef GUARD_4c3af921_f1a5_4158_9561_c1dcac9bbe84
#define GUARD_4c3af921_f1a5_4158_9561_c1dcac9bbe84


#include <GenericPrototypes/GenericPrototypes.h>

#include "Trace.h"


namespace sdp
{


struct ExternalTargetDesignation : public Codogramm
{
    enum { MessageTypeCode = 1301 };
    
    enum Message
    {
        Message_WordSize = 8,
    };
    
    enum MessageExternal
    {
        MessageExternal_TypeCode = 1,
    };
    
    ExternalTargetDesignation()
    {
        appendToOrder( { &trace } );
        appendToOrder( { &distance } );
        appendToOrder( { &azimuth } );
        appendToOrder( { &sectorBegin } );
        appendToOrder( { &sectorEnd } );
    }
    
    ExternalTargetDesignation(const ExternalTargetDesignation& orig)
        : ExternalTargetDesignation()
    {
        trace = orig.trace;
        distance = orig.distance;
        azimuth = orig.azimuth;
        sectorBegin = orig.sectorBegin;
        sectorEnd = orig.sectorEnd;
    }
    
    ExternalTargetDesignation & operator = (const ExternalTargetDesignation &) = default;
    
    Field<Trace> trace;
    Field<uint16_t> distance = lsb{ 12.5 };                     	/// [ м ] Ожидаемая дальность цели
    Field<uint16_t> azimuth = lsb{ 360.0 / 65536 };             	/// [ град. ] Ожидаемый азимут цели
    Field<uint16_t> sectorBegin = lsb{ 360.0 / 65536 };         	/// [ град. ] Азимут начала сектора
    Field<uint16_t> sectorEnd = lsb{ 360.0 / 65536 };           	/// [ град. ] Азимут конца сектора
};


} // namespace sdp


#endif // GUARD_4c3af921_f1a5_4158_9561_c1dcac9bbe84
