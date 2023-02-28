#ifndef GUARD_2a96b00d_c388_4161_9da5_75e182340189
#define GUARD_2a96b00d_c388_4161_9da5_75e182340189


#include <GenericPrototypes/GenericPrototypes.h>

#include "Answer_v2.h"


namespace pdp
{


struct EsgrloCoordinatePoint_v2 : public Codogramm
{
    enum { MessageTypeCode = 154 };
    
    EsgrloCoordinatePoint_v2()
    {
        appendToOrder( { &rejectedByVaru } );
        appendToOrder( { &range } );
        appendToOrder( { &azimuth } );
        appendToOrder( { &amplitude } );
        appendToOrder( { &height } );
        appendToOrder( { &bortNumber } );
        appendToOrder( { &fuelCapacity } );
        appendToOrder( { &disaster } );
        appendToOrder( { &infoDisaster } );
        appendToOrder( { &ur } );
        appendToOrder( { &di_4 } );
        appendToOrder( { &di_6 } );
        appendToOrder( { &isFriendCommonRecognition } );
        appendToOrder( { &isFriendGuaranteedRecognition } );
        appendToOrder( { &isFriendIndividualRecognition } );
        appendToOrder( { &M13 } );
        appendToOrder( { &M46 } );
        appendToOrder( { &packsNum } );
        appendToOrder( { &pack } );
    }
    
    EsgrloCoordinatePoint_v2(const EsgrloCoordinatePoint_v2& orig)
        : EsgrloCoordinatePoint_v2()
    {
        rejectedByVaru = orig.rejectedByVaru;
        range = orig.range;
        azimuth = orig.azimuth;
        amplitude = orig.amplitude;
        height = orig.height;
        bortNumber = orig.bortNumber;
        fuelCapacity = orig.fuelCapacity;
        disaster = orig.disaster;
        infoDisaster = orig.infoDisaster;
        ur = orig.ur;
        di_4 = orig.di_4;
        di_6 = orig.di_6;
        isFriendCommonRecognition = orig.isFriendCommonRecognition;
        isFriendGuaranteedRecognition = orig.isFriendGuaranteedRecognition;
        isFriendIndividualRecognition = orig.isFriendIndividualRecognition;
        M13 = orig.M13;
        M46 = orig.M46;
        packsNum = orig.packsNum;
        pack = orig.pack;
    }
    
    EsgrloCoordinatePoint_v2 & operator = (const EsgrloCoordinatePoint_v2 &) = default;
    
    BitField<uint8_t, 1> rejectedByVaru;
    Field<double> range;
    Field<double> azimuth;
    Field<uint16_t> amplitude;
    Field<uint32_t> height;
    Field<uint32_t> bortNumber;
    Field<uint8_t> fuelCapacity;
    Field<uint8_t> disaster;
    Field<uint8_t> infoDisaster;
    Field<uint8_t> ur;
    Field<uint8_t> di_4;
    Field<uint8_t> di_6;
    BitField<uint8_t, 1> isFriendCommonRecognition;
    BitField<uint8_t, 1> isFriendGuaranteedRecognition;
    BitField<uint8_t, 1> isFriendIndividualRecognition;
    Field<uint32_t> M13;
    Field<uint32_t> M46;
    Field<uint16_t> packsNum;
    PeriodicBitField<Answer_v2> pack = { packsNum };            
};


} // namespace pdp


#endif // GUARD_2a96b00d_c388_4161_9da5_75e182340189
