#ifndef GUARD_e6001a90_36c7_437c_b68f_31069c253b39
#define GUARD_e6001a90_36c7_437c_b68f_31069c253b39


#include <GenericPrototypes/GenericPrototypes.h>

#include "Interrogation.h"


namespace remote_interrogator
{


struct Acknowledgement_v2 : public Codogramm
{
    enum { MessageTypeCode = 2026 };
    
    Acknowledgement_v2()
    {
        appendToOrder( { &azimuth } );
        appendToOrder( { &size } );
        appendToOrder( { &timestamp } );
        appendToOrder( { &interrogations } );
    }
    
    Acknowledgement_v2(const Acknowledgement_v2& orig)
        : Acknowledgement_v2()
    {
        azimuth = orig.azimuth;
        size = orig.size;
        timestamp = orig.timestamp;
        interrogations = orig.interrogations;
    }
    
    Acknowledgement_v2 & operator = (const Acknowledgement_v2 &) = default;
    
    Field<uint16_t> azimuth;
    Field<uint16_t> size;
    Field<uint64_t> timestamp;
    PeriodicBitField<Interrogation> interrogations = { size };  
};


} // namespace remote_interrogator


#endif // GUARD_e6001a90_36c7_437c_b68f_31069c253b39
