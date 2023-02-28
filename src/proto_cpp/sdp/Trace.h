#ifndef GUARD_544c2086_de4b_4253_9adc_4d177bac6bbc
#define GUARD_544c2086_de4b_4253_9adc_4d177bac6bbc


#include <GenericPrototypes/GenericPrototypes.h>



namespace sdp
{


struct Trace : public Codogramm
{
    enum { MessageTypeCode = 1300 };
    
    enum Message
    {
        Message_WordSize = 4,
    };
    
    enum MessageExternal
    {
        MessageExternal_TypeCode = 1,
    };
    
    Trace()
    {
        appendToOrder( { &AA } );
        appendToOrder( { &C } );
        appendToOrder( { &alignOn_16bit } );
        appendToOrder( { &number } );
    }
    
    Trace(const Trace& orig)
        : Trace()
    {
        AA = orig.AA;
        C = orig.C;
        number = orig.number;
    }
    
    Trace & operator = (const Trace &) = default;
    
    BitField<uint32_t, 24> AA;                                  	/// Объявленный адрес ВС в режиме S
    BitField<uint8_t, 1> C;                                     	/// Признак сброса трассы
    Field<uint16_t> number;                                     	/// Номер трассы
};


} // namespace sdp


#endif // GUARD_544c2086_de4b_4253_9adc_4d177bac6bbc
