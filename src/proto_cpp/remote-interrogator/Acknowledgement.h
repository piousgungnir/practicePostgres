#ifndef GUARD_89c0e860_7c59_480e_89f8_171433219130
#define GUARD_89c0e860_7c59_480e_89f8_171433219130


#include <GenericPrototypes/GenericPrototypes.h>

#include "Interrogation.h"


namespace remote_interrogator
{


struct Acknowledgement : public Codogramm
{
    enum { MessageTypeCode = 2020 };
    
    Acknowledgement()
    {
        appendToOrder( { &azimuth } );
        appendToOrder( { &size } );
        appendToOrder( { &interrogations } );
    }
    
    Acknowledgement(const Acknowledgement& orig)
        : Acknowledgement()
    {
        azimuth = orig.azimuth;
        size = orig.size;
        interrogations = orig.interrogations;
    }
    
    Acknowledgement & operator = (const Acknowledgement &) = default;
    
    Field<uint16_t> azimuth;
    Field<uint16_t> size;
    PeriodicBitField<Interrogation> interrogations = { size };  
};


} // namespace remote_interrogator


#endif // GUARD_89c0e860_7c59_480e_89f8_171433219130
