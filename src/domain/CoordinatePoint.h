#pragma once


#include <vector>

#include "Answer.h"
#include "POIProtocol.h"

#include "Group.h"


enum
{
    DayIn100nsIntervals = 864000000000
};


enum SystemMode
{
    Undefened = 0,
    RBS = 1,
    NRZ,
    ParolAlarm,
    ModeS
};


using pack_t = Group;
using deltaAzimuth_t = std::vector<double>;

namespace impl
{


struct CoordinatePoint
{
    enum
    {
        NotValidValue = 1000000
    };

    struct MBData
    {
        uint64_t mb;
        uint8_t BDS1;
        uint8_t BDS2;
        Timestamp timestamp;
    };

    CoordinatePoint()
    {
    }

    long idCoordinatePoint;
    Timestamp timestamp;
    bool rejectedByTagc = false;
    double range{0}; // дистанция, на которой находится координатная точка
    double azimuth{0};
    uint16_t amplitude{0}; // амплитуда координатной точки (мощность сигнала) в дБ

    SystemMode mode;

    int missionCode = NotValidValue; // aircraft type or mission в ружиме 1
    int unitCode = NotValidValue;    // unit code or tail number в режиме 2
    int bortNumber = NotValidValue;  // бортовой номер в режиме 3/А, 10-тичный эквивалент
    // TODO перейти на использование stdssr::rbs::ModeC::Altitude
    int altitude = NotValidValue; // высота РБС(режим С) в м
    bool spi = false;
    bool xImpulse = false;
    bool militaryAlarm = false;

    struct
    {
        struct Codes
        {
            int code;
            int confidenceCode;
        };
        Codes _1;
        Codes _2;
        Codes _3A;
        Codes _C;
    } mark;

    struct ModeS
    {
        uint32_t AA = 0;              // номер борта в режиме Эс
        int identity = NotValidValue; // гарантированный бортовой номер в режиме 3/А
        // TODO перейти на использование stdssr::ModeS::AC::Altitude
        int altitude = NotValidValue; // высота (режим S)
        uint8_t flightStatus;         // для I048/230 - flight status
        bool bds10Received = false;
        uint8_t transponderLevel;
        uint8_t si; // для I048/230 - SI transponder capability
        // TODO stdssr::ModeS::Altitude::Resolution
        uint8_t altitudeReportingCapability{0};   // для I048/230 - altitude reporting capability
        uint8_t aircraftIdentificationCapability; // для I048/230
        uint8_t msscCapability;                   // для I048/230
        uint8_t acasStatus;                       // для I048/230
        uint8_t acasParameters;                   // для I048/230
        std::vector<MBData> mbdata;               // для I048/250

        std::vector<MBData> outdatedBds;          // устаревшие регистры
        std::vector<uint8_t> supportedBds;        // поддерживаемые регистры
        std::vector<uint8_t> usefullDataBds;      // регистры наполненные полезными данными
        std::vector<uint8_t> unrequestedBds;      // еще не запрошенные
        std::vector<uint8_t> nextRequests;        // регистры, которые будут запрошенны на следующем обороте
        std::vector<uint8_t> recentlyUpdatedBds;  // полученные на последнем обороте
        struct
        {
            uint32_t character8 : 6;
            uint32_t character7 : 6;
            uint32_t character6 : 6;
            uint32_t character5 : 6;
            uint32_t character4 : 6;
            uint32_t character3 : 6;
            uint32_t character2 : 6;
            uint32_t character1 : 6;
            bool isPresent = false;
        } aircraftIdentification;
        bool acasReportAdvisoryIsPresent = false; // для I048/260
        uint64_t acasReportAdvisory;              // для I048/260

    } modeS; // режим S

    bool isRollCall = false;
    bool isControlResponder = false;

    pack_t pack; // ответы, из которых сформирована координатная точка (исх. информация)

    bool missionCodeIsGarbled = false;
    bool unitCodeIsGarbled = false;
    bool bortNumberIsGarbled = false;
    bool heightIsGarbled = false;
};


} // end of namespace impl


namespace esgrlo
{


struct CoordinatePoint
{
    bool rejectedByTagc = false;
    double range = 0;
    double azimuth = 0;
    uint16_t amplitude = 0;
    pack_t pack;

    int heigth = impl::CoordinatePoint::NotValidValue;
    int bortNumber = impl::CoordinatePoint::NotValidValue;

    uint8_t fuelCapacity = 0;
    uint8_t disaster = 0;
    uint8_t infoDisaster = 0;
    uint8_t ur = 0;
    uint8_t di_4 = 0;
    uint8_t di_6 = 0;

    bool isFriendCommonRecognition = false;
    bool isFriendGuaranteedRecognition = false;
    bool isFriendIndividualRecognition = false;

    int M13 = 0;
    int M46 = 0;
};


struct AlarmBearing
{
    double azimuth = 0;
    int MA = 0;
};


} // end of namespace esgrlo
