#ifndef GUARD_6bc4580b_7356_44e9_bca6_ed0a2c5c2705
#define GUARD_6bc4580b_7356_44e9_bca6_ed0a2c5c2705


#include <GenericPrototypes/GenericPrototypes.h>



namespace pdp
{


struct Answer_v2 : public Codogramm
{
    enum { MessageTypeCode = 152 };
    
    struct ExtendedInfo : public Codogramm
    {
        ExtendedInfo()
        {
            appendToOrder( { &otherModeData } );
            appendToOrder( { &bds1 } );
            appendToOrder( { &bds2 } );
            appendToOrder( { &modesDataSize } );
            appendToOrder( { &modeS } );
        }
        
        ExtendedInfo(const ExtendedInfo& orig)
            : ExtendedInfo()
        {
            otherModeData = orig.otherModeData;
            bds1 = orig.bds1;
            bds2 = orig.bds2;
            modesDataSize = orig.modesDataSize;
            modeS = orig.modeS;
        }
        
        ExtendedInfo & operator = (const ExtendedInfo &) = default;
        
        Field<uint32_t> otherModeData;
        BitField<uint8_t, 4> bds1;
        BitField<uint8_t, 4> bds2;
        Field<uint8_t> modesDataSize;
        PeriodicBitField<uint32_t> modeS = { modesDataSize };       
    };
    
    struct ResponseSignalInfo : public Codogramm
    {
        ResponseSignalInfo()
        {
            appendToOrder( { &Dos } );
            appendToOrder( { &dDS } );
            appendToOrder( { &ASP } );
            appendToOrder( { &HIP } );
            appendToOrder( { &mode } );
            appendToOrder( { &exInfo } );
        }
        
        ResponseSignalInfo(const ResponseSignalInfo& orig)
            : ResponseSignalInfo()
        {
            Dos = orig.Dos;
            dDS = orig.dDS;
            ASP = orig.ASP;
            HIP = orig.HIP;
            mode = orig.mode;
            exInfo = orig.exInfo;
        }
        
        ResponseSignalInfo & operator = (const ResponseSignalInfo &) = default;
        
        BitField<uint32_t, 20> Dos;
        BitField<uint16_t, 10> dDS;
        BitField<uint8_t, 1> ASP;
        BitField<uint8_t, 1> HIP;
        Field<uint8_t> mode;
        Field<ExtendedInfo> exInfo;
    };
    
    struct Amplitude : public Codogramm
    {
        Amplitude()
        {
            appendToOrder( { &sum } );
            appendToOrder( { &dif } );
            appendToOrder( { &pbl } );
        }
        
        Amplitude(const Amplitude& orig)
            : Amplitude()
        {
            sum = orig.sum;
            dif = orig.dif;
            pbl = orig.pbl;
        }
        
        Amplitude & operator = (const Amplitude &) = default;
        
        Field<uint32_t> sum;
        Field<uint32_t> dif;
        Field<uint32_t> pbl;
    };
    
    struct PelengatedInfo : public Codogramm
    {
        PelengatedInfo()
        {
            appendToOrder( { &distance } );
            appendToOrder( { &azimuth } );
            appendToOrder( { &phase } );
            appendToOrder( { &amplitude } );
        }
        
        PelengatedInfo(const PelengatedInfo& orig)
            : PelengatedInfo()
        {
            distance = orig.distance;
            azimuth = orig.azimuth;
            phase = orig.phase;
            amplitude = orig.amplitude;
        }
        
        PelengatedInfo & operator = (const PelengatedInfo &) = default;
        
        Field<double> distance;
        Field<double> azimuth;
        Field<double> phase;
        Field<Amplitude> amplitude;
    };
    
    Answer_v2()
    {
        appendToOrder( { &radio } );
        appendToOrder( { &monopulseAzimuth } );
        appendToOrder( { &monopulseCorrection } );
        appendToOrder( { &phase } );
        appendToOrder( { &distance } );
        appendToOrder( { &info } );
        appendToOrder( { &timestamp } );
    }
    
    Answer_v2(const Answer_v2& orig)
        : Answer_v2()
    {
        radio = orig.radio;
        monopulseAzimuth = orig.monopulseAzimuth;
        monopulseCorrection = orig.monopulseCorrection;
        phase = orig.phase;
        distance = orig.distance;
        info = orig.info;
        timestamp = orig.timestamp;
    }
    
    Answer_v2 & operator = (const Answer_v2 &) = default;
    
    Field<PelengatedInfo> radio;
    Field<double> monopulseAzimuth;
    Field<double> monopulseCorrection;
    Field<double> phase;
    Field<double> distance;
    Field<ResponseSignalInfo> info;
    Field<uint64_t> timestamp;
};


} // namespace pdp


#endif // GUARD_6bc4580b_7356_44e9_bca6_ed0a2c5c2705
