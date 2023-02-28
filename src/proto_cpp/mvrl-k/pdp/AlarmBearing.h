#ifndef GUARD_df59b8d5_f2c9_429d_8c18_9cdacdbbdead
#define GUARD_df59b8d5_f2c9_429d_8c18_9cdacdbbdead


#include <GenericPrototypes/GenericPrototypes.h>



namespace pdp
{


struct AlarmBearing : public Codogramm
{
    enum { MessageTypeCode = 111 };
    
    AlarmBearing()
    {
        appendToOrder( { &azimuth } );
        appendToOrder( { &MA } );
    }
    
    AlarmBearing(const AlarmBearing& orig)
        : AlarmBearing()
    {
        azimuth = orig.azimuth;
        MA = orig.MA;
    }
    
    AlarmBearing & operator = (const AlarmBearing &) = default;
    
    Field<double> azimuth;
    Field<uint32_t> MA;
};


} // namespace pdp


#endif // GUARD_df59b8d5_f2c9_429d_8c18_9cdacdbbdead
