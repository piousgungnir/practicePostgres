#ifndef GUARD_38b002dc_0d1d_4c8f_a960_36dc22c4458b
#define GUARD_38b002dc_0d1d_4c8f_a960_36dc22c4458b


#include <GenericPrototypes/GenericPrototypes.h>



namespace sdp
{


struct Header : public Codogramm
{
    Header()
    {
        appendToOrder( { &type } );
        appendToOrder( { &size } );
    }
    
    Header(const Header& orig)
        : Header()
    {
        type = orig.type;
        size = orig.size;
    }
    
    Header & operator = (const Header &) = default;
    
    Field<uint8_t> type;
    Field<uint8_t> size;
};


} // namespace sdp


#endif // GUARD_38b002dc_0d1d_4c8f_a960_36dc22c4458b
