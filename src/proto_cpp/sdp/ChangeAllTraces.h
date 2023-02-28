#ifndef GUARD_af92ea24_8ad5_4f81_9fa2_521be32ee2b4
#define GUARD_af92ea24_8ad5_4f81_9fa2_521be32ee2b4


#include <GenericPrototypes/GenericPrototypes.h>



namespace sdp
{


struct ChangeAllTraces : public Codogramm
{
    enum { MessageTypeCode = 1303 };
    
    enum Message
    {
        Message_WordSize = 1,
    };
    
    enum MessageExternal
    {
        MessageExternal_TypeCode = 3,
    };
    
    ChangeAllTraces()
    {
    }
    
    ChangeAllTraces(const ChangeAllTraces& orig)
        : ChangeAllTraces()
    {
    }
    
    ChangeAllTraces & operator = (const ChangeAllTraces &) = default;
    
};


} // namespace sdp


#endif // GUARD_af92ea24_8ad5_4f81_9fa2_521be32ee2b4
