#ifndef GUARD_35493e34_69d8_40d9_be99_f4ec28b73580
#define GUARD_35493e34_69d8_40d9_be99_f4ec28b73580


#include <GenericPrototypes/GenericPrototypes.h>

#include "Answer.h"


namespace pdp
{


struct AtcrbsCoordinatePoint : public Codogramm
{
    enum { MessageTypeCode = 113 };
    
    enum SystemMode
    {
        SystemMode_Undefined = 0,
        SystemMode_RBS = 1,
        SystemMode_NRZ = 2,
        SystemMode_ParolAlarm = 3,
        SystemMode_ModeS = 4,
    };
    
    struct RbsCodes : public Codogramm
    {
        RbsCodes()
        {
            appendToOrder( { &code } );
            appendToOrder( { &confidenceCode } );
        }
        
        RbsCodes(const RbsCodes& orig)
            : RbsCodes()
        {
            code = orig.code;
            confidenceCode = orig.confidenceCode;
        }
        
        RbsCodes & operator = (const RbsCodes &) = default;
        
        Field<int32_t> code;
        Field<int32_t> confidenceCode;
    };
    
    struct Parol : public Codogramm
    {
        Parol()
        {
            appendToOrder( { &heigth } );
            appendToOrder( { &bortNumber } );
            appendToOrder( { &fuelCapacity } );
            appendToOrder( { &disaster } );
            appendToOrder( { &ur } );
            appendToOrder( { &di_4 } );
            appendToOrder( { &di_6 } );
            appendToOrder( { &isFriendCommonRecognition } );
            appendToOrder( { &isFriendGuaranteedRecognition } );
            appendToOrder( { &isFriendIndividualRecognition } );
        }
        
        Parol(const Parol& orig)
            : Parol()
        {
            heigth = orig.heigth;
            bortNumber = orig.bortNumber;
            fuelCapacity = orig.fuelCapacity;
            disaster = orig.disaster;
            ur = orig.ur;
            di_4 = orig.di_4;
            di_6 = orig.di_6;
            isFriendCommonRecognition = orig.isFriendCommonRecognition;
            isFriendGuaranteedRecognition = orig.isFriendGuaranteedRecognition;
            isFriendIndividualRecognition = orig.isFriendIndividualRecognition;
        }
        
        Parol & operator = (const Parol &) = default;
        
        Field<uint32_t> heigth;
        Field<uint32_t> bortNumber;
        Field<uint8_t> fuelCapacity;
        Field<uint8_t> disaster;
        Field<uint8_t> ur;
        Field<uint8_t> di_4;
        Field<uint8_t> di_6;
        BitField<uint8_t, 1> isFriendCommonRecognition;
        BitField<uint8_t, 1> isFriendGuaranteedRecognition;
        BitField<uint8_t, 1> isFriendIndividualRecognition;
    };
    
    struct MbData : public Codogramm
    {
        MbData()
        {
            appendToOrder( { &mb } );
            appendToOrder( { &bds1 } );
            appendToOrder( { &bds2 } );
            appendToOrder( { &timeStamp } );
        }
        
        MbData(const MbData& orig)
            : MbData()
        {
            mb = orig.mb;
            bds1 = orig.bds1;
            bds2 = orig.bds2;
            timeStamp = orig.timeStamp;
        }
        
        MbData & operator = (const MbData &) = default;
        
        Field<uint64_t> mb;
        Field<uint8_t> bds1;
        Field<uint8_t> bds2;
        Field<uint64_t> timeStamp;
    };
    
    struct BdsData : public Codogramm
    {
        BdsData()
        {
            appendToOrder( { &actualBdsSize } );
            appendToOrder( { &actualBds } );
            appendToOrder( { &outdatedBdsSize } );
            appendToOrder( { &outdatedBds } );
            appendToOrder( { &supportedBdsSize } );
            appendToOrder( { &supportedBds } );
            appendToOrder( { &usefullBdsSize } );
            appendToOrder( { &usefullDataBds } );
            appendToOrder( { &unrequestedBdsSize } );
            appendToOrder( { &unrequestedBds } );
            appendToOrder( { &nextRequestsSize } );
            appendToOrder( { &nextRequests } );
            appendToOrder( { &recentlyUpdatedBdsSize } );
            appendToOrder( { &recentlyUpdatedBds } );
        }
        
        BdsData(const BdsData& orig)
            : BdsData()
        {
            actualBdsSize = orig.actualBdsSize;
            actualBds = orig.actualBds;
            outdatedBdsSize = orig.outdatedBdsSize;
            outdatedBds = orig.outdatedBds;
            supportedBdsSize = orig.supportedBdsSize;
            supportedBds = orig.supportedBds;
            usefullBdsSize = orig.usefullBdsSize;
            usefullDataBds = orig.usefullDataBds;
            unrequestedBdsSize = orig.unrequestedBdsSize;
            unrequestedBds = orig.unrequestedBds;
            nextRequestsSize = orig.nextRequestsSize;
            nextRequests = orig.nextRequests;
            recentlyUpdatedBdsSize = orig.recentlyUpdatedBdsSize;
            recentlyUpdatedBds = orig.recentlyUpdatedBds;
        }
        
        BdsData & operator = (const BdsData &) = default;
        
        Field<uint8_t> actualBdsSize;
        PeriodicBitField<MbData> actualBds = { actualBdsSize };     
        Field<uint8_t> outdatedBdsSize;
        PeriodicBitField<MbData> outdatedBds = { outdatedBdsSize }; 
        Field<uint8_t> supportedBdsSize;
        PeriodicBitField<uint8_t> supportedBds = { supportedBdsSize };
        Field<uint8_t> usefullBdsSize;
        PeriodicBitField<uint8_t> usefullDataBds = { usefullBdsSize };
        Field<uint8_t> unrequestedBdsSize;
        PeriodicBitField<uint8_t> unrequestedBds = { unrequestedBdsSize };
        Field<uint8_t> nextRequestsSize;
        PeriodicBitField<uint8_t> nextRequests = { nextRequestsSize };
        Field<uint8_t> recentlyUpdatedBdsSize;
        PeriodicBitField<uint8_t> recentlyUpdatedBds = { recentlyUpdatedBdsSize };
    };
    
    struct ModeS : public Codogramm
    {
        ModeS()
        {
            appendToOrder( { &AA } );
            appendToOrder( { &identity } );
            appendToOrder( { &altitude } );
            appendToOrder( { &PrS } );
            appendToOrder( { &Bds } );
        }
        
        ModeS(const ModeS& orig)
            : ModeS()
        {
            AA = orig.AA;
            identity = orig.identity;
            altitude = orig.altitude;
            PrS = orig.PrS;
            Bds = orig.Bds;
        }
        
        ModeS & operator = (const ModeS &) = default;
        
        Field<uint32_t> AA;
        Field<uint32_t> identity;
        Field<uint32_t> altitude;
        BitField<uint8_t, 3> PrS;
        Field<BdsData> Bds;
    };
    
    AtcrbsCoordinatePoint()
    {
        appendToOrder( { &rejectedByVaru } );
        appendToOrder( { &range } );
        appendToOrder( { &azimuth } );
        appendToOrder( { &amplitude } );
        appendToOrder( { &xImpulse } );
        appendToOrder( { &mode } );
        appendToOrder( { &unitCode } );
        appendToOrder( { &bortNumber } );
        appendToOrder( { &altitude } );
        appendToOrder( { &spi } );
        appendToOrder( { &militaryAlarm } );
        appendToOrder( { &mode_1 } );
        appendToOrder( { &mode_2 } );
        appendToOrder( { &mode_3A } );
        appendToOrder( { &mode_C } );
        appendToOrder( { &parol } );
        appendToOrder( { &modes } );
        appendToOrder( { &isRollCall } );
        appendToOrder( { &M } );
        appendToOrder( { &packsNum } );
        appendToOrder( { &pack } );
    }
    
    AtcrbsCoordinatePoint(const AtcrbsCoordinatePoint& orig)
        : AtcrbsCoordinatePoint()
    {
        rejectedByVaru = orig.rejectedByVaru;
        range = orig.range;
        azimuth = orig.azimuth;
        amplitude = orig.amplitude;
        xImpulse = orig.xImpulse;
        mode = orig.mode;
        unitCode = orig.unitCode;
        bortNumber = orig.bortNumber;
        altitude = orig.altitude;
        spi = orig.spi;
        militaryAlarm = orig.militaryAlarm;
        mode_1 = orig.mode_1;
        mode_2 = orig.mode_2;
        mode_3A = orig.mode_3A;
        mode_C = orig.mode_C;
        parol = orig.parol;
        modes = orig.modes;
        isRollCall = orig.isRollCall;
        M = orig.M;
        packsNum = orig.packsNum;
        pack = orig.pack;
    }
    
    AtcrbsCoordinatePoint & operator = (const AtcrbsCoordinatePoint &) = default;
    
    BitField<uint8_t, 1> rejectedByVaru;
    Field<double> range;
    Field<double> azimuth;
    Field<uint16_t> amplitude;
    BitField<uint8_t, 1> xImpulse;
    BitField<uint8_t, 5> mode;
    Field<uint32_t> unitCode;
    Field<uint32_t> bortNumber;
    Field<uint32_t> altitude;
    BitField<uint8_t, 1> spi;
    BitField<uint8_t, 1> militaryAlarm;
    Field<RbsCodes> mode_1;
    Field<RbsCodes> mode_2;
    Field<RbsCodes> mode_3A;
    Field<RbsCodes> mode_C;
    Field<Parol> parol;
    Field<ModeS> modes;
    BitField<uint8_t, 1> isRollCall;
    Field<uint16_t> M;
    Field<uint16_t> packsNum;
    PeriodicBitField<Answer> pack = { packsNum };               
};


} // namespace pdp


#endif // GUARD_35493e34_69d8_40d9_be99_f4ec28b73580
