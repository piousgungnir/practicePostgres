#ifndef GUARD_514f188b_a87c_4d73_b231_0a2217cd30d2
#define GUARD_514f188b_a87c_4d73_b231_0a2217cd30d2


#include <GenericPrototypes/GenericPrototypes.h>



namespace remote_interrogator
{


struct Interrogation : public Codogramm
{
    struct InterrogationData : public Codogramm
    {
        InterrogationData()
        {
            appendToOrder( { &bytes } );
            appendToOrder( { &isLong } );
            appendToOrder( { &alignOn_32bit } );
        }
        
        InterrogationData(const InterrogationData& orig)
            : InterrogationData()
        {
            bytes = orig.bytes;
            isLong = orig.isLong;
        }
        
        InterrogationData & operator = (const InterrogationData &) = default;
        
        PeriodicBitField<uint32_t> bytes = { 4 };                   
        BitField<uint8_t, 1> isLong;
    };
    
    Interrogation()
    {
        appendToOrder( { &mode } );
        appendToOrder( { &pzk3 } );
        appendToOrder( { &alignOn_32bit } );
        appendToOrder( { &data } );
    }
    
    Interrogation(const Interrogation& orig)
        : Interrogation()
    {
        mode = orig.mode;
        pzk3 = orig.pzk3;
        data = orig.data;
    }
    
    Interrogation & operator = (const Interrogation &) = default;
    
    Field<uint8_t> mode;
    Field<uint8_t> pzk3;
    Field<InterrogationData> data;
};


} // namespace remote_interrogator


#endif // GUARD_514f188b_a87c_4d73_b231_0a2217cd30d2
