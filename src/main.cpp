#include "domain/CoordinatePoint.h"
#include "database.hpp"

int main()
{
    impl::CoordinatePoint cp;
    DB::PostgresCPRepository PRep("postgresql://localhost:5432/default_database?user=username&password=password");
    PRep.addCoordinatePoint(cp);
    PRep.getByIdCoordinatePoint(5);
    auto responseCP = PRep.getByIdCoordinatePoint(5);
    std::cout << "idcp: " << responseCP.idCoordinatePoint << std::endl;
//    std::cout << "Time: " << timestamp_to_str_datetime(responseCP.timestamp) << std::endl;

//    Timestamp st;
//
//    impl::PelengatedInfo rd{1,1,1,1,1,1};
//
//    POI::ResponseSignalInfo exif;
//    exif.Dos;
//    exif.dDS;
//    exif.ASP;
//    exif.HIP;
//    exif.mode;
//
//    exif.exInfo.rbs_modeOther.Code;
//    exif.exInfo.rbs_modeOther.nCode;
//    exif.exInfo.rbs_modeOther.nSPI;
//    exif.exInfo.rbs_modeOther.PBL;
//    exif.exInfo.rbs_modeOther.AW;
//
//    exif.exInfo.modeS.data[0];
//    exif.exInfo.modeS.data[1];
//    exif.exInfo.modeS.data[2];
//    exif.exInfo.modeS.data[3];
//    exif.exInfo.modeS.BDS1;
//    exif.exInfo.modeS.BDS2;
//    exif.exInfo.modeS.sicAllCall;
//    exif.exInfo.modeS.PC_type;
//    exif.exInfo.modeS.PC_EC;
//
//    impl::RawAnswer rans{st, rd, exif};
//
//    impl::Answer ans(rans);
//    ans.monopulseAzimuth;
//    ans.monopulseCorrection;
//    ans.phase;
//    ans.distance;
//    ans.azimuthFromDistance;


//    std::cout << "range: " << responseCP.range << std::endl;
//    std::cout << "amplitude: " << responseCP.amplitude << std::endl;
//    std::cout << "mode: " << responseCP.mode << std::endl;
//    std::cout << "heightIsGarbled: " << responseCP.heightIsGarbled << std::endl;

//    //ответы имеются разных типов
//    // RBS, S, НРЗ
//    //типичная информация ответа RBS
////    POI::ExtendedInfo info;
////    info.modeS.data;
////    info.modeS.BDS1;
////    info.modeS.BDS2;
////    info.modeS.PC_EC;
////    info.modeS.sicAllCall;
////    info.rbs_modeOther.AW;
////    info.rbs_modeOther.Code;
////    info.rbs_modeOther.PBL;
////    info.rbs_modeOther.SPI;
////    info.rbs_modeOther.nCode;
////    info.rbs_modeOther.nSPI;
////    info._reserved;
////    info._ser_value;
////    info.parol_modeAlert.value;
////    info.peleng_ASHP.value;
////    info.peleng_HIP.skw;
////    info.rbs_mode4.PBL;
////    info.rbs_mode4.Gr;
////    info.rbs_mode4.KodM4;
////    info.rbs_mode4.POKM4;
//
//
//
//    // самолёт, или как мы называем координатная точка
//    // тоже бывает разных типов
//    // RBS/S
//    impl::CoordinatePoint cp;
////    cp.modeS.altitude;
////    cp.modeS.AA;
////    cp.modeS.acasParameters;
////    cp.modeS.acasReportAdvisory;
////    cp.modeS.acasReportAdvisoryIsPresent;
////    cp.modeS.acasStatus;
////    cp.modeS.aircraftIdentification;
////    cp.modeS.aircraftIdentificationCapability;
////    cp.modeS.altitudeReportingCapability;
////    cp.modeS.bds10Received;
////    cp.modeS.flightStatus;
////    cp.modeS.identity;
////    cp.modeS.mbdata;
////    cp.modeS.msscCapability;
////    cp.modeS.nextRequests;
////    cp.modeS.outdatedBds;
////    cp.modeS.recentlyUpdatedBds;
////    cp.modeS.si;
////    cp.modeS.supportedBds;
////    cp.modeS.transponderLevel;
////    cp.modeS.unrequestedBds;
////    cp.modeS.usefullDataBds;
//
////    +idCoordinatePoint
////    +timeCoordinatePoint
//    cp.idCoordinatePoint;
//    cp.timestamp;
//    cp.range;
//    cp.azimuth;
//    cp.amplitude;
//    cp.mode;
//    cp.missionCode;
//    cp.unitCode;
//    cp.bortNumber;
//    cp.altitude;
//    cp.spi;
//    cp.xImpulse;
//    cp.militaryAlarm;
//    cp.isRollCall;
//    cp.isControlResponder;
//    cp.missionCodeIsGarbled;
//    cp.unitCodeIsGarbled;
//    cp.bortNumberIsGarbled;
//    cp.heightIsGarbled;
//
////    cp.mark;
////    cp.rejectedByTagc;
////    cp.unitCode;
//
//
////    +idCoordinatePoint;
//    cp.pack[0]->raw.timestamp; //!!!
//    cp.pack[0]->distance; //processedDistance
//    cp.pack[0]->raw.radio.azimuth;
//    cp.pack[0]->phase; //processedPhase
//    cp.pack[0]->raw.radio.amplitude.sum; //sum_ampl
//    cp.pack[0]->raw.radio.amplitude.dif; //dif_ampl
//    cp.pack[0]->raw.radio.amplitude.pbl; //pbl_ampl
//    cp.pack[0]->raw.info.mode;
//
//    //RBS
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.Code;
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.SPI;
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.nCode;
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.nSPI;
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.PBL; //isUnderPbl
//    cp.pack[0]->raw.info.exInfo.rbs_modeOther.AW;
//
//    //modeS
//    cp.pack[0]->raw.info.exInfo.modeS.data;
//    cp.pack[0]->raw.info.exInfo.modeS.sicAllCall;
//    cp.pack[0]->raw.info.exInfo.modeS.PC_EC;
//    cp.pack[0]->raw.info.exInfo.modeS.BDS2;
//    cp.pack[0]->raw.info.exInfo.modeS.BDS1;
//    cp.pack[0]->raw.info.exInfo.modeS.PC_type;
//
//    cp.pack[0]->monopulseAzimuth;
//    cp.pack[0]->monopulseCorrection;
//    cp.pack[0]->raw.radio.phase;
//    cp.pack[0]->raw.radio.distance;
//    cp.pack[0]->azimuthFromDistance;
//
//
//
//
////    cp.pack[0]->raw.info.ASP;
////    cp.pack[0]->raw.info.Dos;
////    cp.pack[0]->raw.info.HIP;
////    cp.pack[0]->raw.info.dDS;
////    cp.pack[0]->raw.info.values;
//
//
//
//







    //тут информация по режиму S; возможно есть лишнее. можно на первых порах исключить вектора
    //и НРЗ также синонимы Пароль/ЕС ГРЛО
//    esgrlo::CoordinatePoint nrzCp;
}
