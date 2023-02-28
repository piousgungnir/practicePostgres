#ifndef GUARD_e6f4649f_f9ea_4ee1_a43b_e33278525d0e
#define GUARD_e6f4649f_f9ea_4ee1_a43b_e33278525d0e


#include <GenericPrototypes/GenericPrototypes.h>



namespace sdp
{


struct ChangeTrace : public Codogramm
{
    enum { MessageTypeCode = 1302 };
    
    enum Message
    {
        Message_WordSize = 3,
    };
    
    enum MessageExternal
    {
        MessageExternal_TypeCode = 2,
    };
    
    ChangeTrace()
    {
        appendToOrder( { &PreviosNumTrace } );
        appendToOrder( { &NewNumTrace } );
    }
    
    ChangeTrace(const ChangeTrace& orig)
        : ChangeTrace()
    {
        PreviosNumTrace = orig.PreviosNumTrace;
        NewNumTrace = orig.NewNumTrace;
    }
    
    ChangeTrace & operator = (const ChangeTrace &) = default;
    
    Field<uint16_t> PreviosNumTrace;                            	/// Предыдущий номер трассы
    Field<uint16_t> NewNumTrace;                                	/// Новый номер трассы
};


} // namespace sdp


#endif // GUARD_e6f4649f_f9ea_4ee1_a43b_e33278525d0e
