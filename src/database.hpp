//
// Created by gungnir on 15.02.2023.
//

#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include "domain/CoordinatePoint.h"
#include "pqxx/pqxx"
#include <iostream>
#include <memory>
#include <string>
#include <valarray>

namespace DB
{

//чисто виртуальный класс репозитория
class ICoordinatePointRepository
{
protected:
    virtual ~ICoordinatePointRepository() = default; // защищенный, чисто виртуальный деструктор

public:
//    инициализация базы данных ( создание таблиц )
    virtual void initDB() const = 0;
//    добавление в базу координатой точки
    virtual void addCoordinatePoint(impl::CoordinatePoint& cp) const = 0;
////    добавление в базу ответов данной координатной точки
//    virtual void addReply(impl::CoordinatePoint& cp) const = 0;
//    получение координатной точки из базы по её id
    virtual impl::CoordinatePoint getByIdCoordinatePoint(const uint & id_coordinate_point) = 0;
};

//реализация репозитория с исползованием Postgresql
class PostgresCPRepository : ICoordinatePointRepository {
private:
    std::unique_ptr<pqxx::connection> connection;
public:
    void initDB() const override {
        pqxx::work txn(*connection);
        txn.exec("CREATE TABLE IF NOT EXISTS coordinate_point "
                 "("
                 "    id_coordinate_point    BIGINT           not null"
                 "        constraint coordinate_point_pk"
                 "            primary key, "
                 "    time_coordinate_point   text             not null,"
                 "    range                   double precision not null,"
                 "    azimuth                 double precision not null,"
                 "    amplitude               numeric          not null,"
                 "    mode                    integer          not null,"
                 "    mission_code            integer,"
                 "    unit_code               integer,"
                 "    bort_number             integer,"
                 "    altitude                integer,"
                 "    spi                     bool,"
                 "    x_impulse               bool,"
                 "    military_alarm          bool,"
                 "    is_roll_call            bool,"
                 "    is_control_responder    bool,"
                 "    mission_code_is_garbled bool,"
                 "    unit_code_is_garbled    bool,"
                 "    bort_Number_is_garbled  bool,"
                 "    height_is_garbled       bool);");

        txn.exec("CREATE TABLE IF NOT EXISTS rbs_reply"
                 "( id_coordinate_point   bigint    not null"
                 "        constraint rbs_reply_coordinate_point_id_coordinate_point_fk"
                 "            references coordinate_point,"
                 "    time_reply                 text          not null,"
                 "    distance                        double precision,"
                 "    azimuth                         double precision,"
                 "    phase                           double precision,"
                 "    sum_ampl                        numeric,"
                 "    dif_ampl                        numeric,"
                 "    pbl_ampl                        numeric,"
                 "    mode                            integer,"
                 "    code                            bit varying(13),"
                 "    spi                             bit varying(1),"
                 "    n_code                          bit varying(13),"
                 "    n_spi                           bit varying(1),"
                 "    is_under_pbl                    bit varying(1),"
                 "    aw                              bit varying(1),"
                 "    monopulse_azimuth               double precision,"
                 "    monopulse_correction            double precision,"
                 "    processed_phase                 double precision,"
                 "    processed_distance              double precision,"
                 "    azimuth_from_distance           double precision"
                 ");");

        txn.exec("CREATE TABLE IF NOT EXISTS s_reply"
                 "( id_coordinate_point   bigint    not null"
                 "        constraint rbs_reply_coordinate_point_id_coordinate_point_fk"
                 "            references coordinate_point,"
                 "    time_reply         text         not null,"
                 "    distance                double precision,"
                 "    azimuth                 double precision,"
                 "    phase                   double precision,"
                 "    sum_ampl                numeric,"
                 "    dif_ampl                numeric,"
                 "    pbl_ampl                numeric,"
                 "    mode                    integer,"
                 "    data0                   integer,"
                 "    data1                   integer,"
                 "    data2                   integer,"
                 "    data3                   integer,"
                 "    bsd1                    integer,"
                 "    bsd2                    integer,"
                 "    sic_all_call            integer,"
                 "    pc_type                 integer,"
                 "    pc_EC                   integer,"
                 "    monopulse_azimuth       double precision,"
                 "    monopulse_correction    double precision,"
                 "    processed_phase         double precision,"
                 "    processed_distance      double precision,"
                 "    azimuth_from_distance   double precision"
                 ");");
        txn.commit();
    }

    explicit PostgresCPRepository(const std::string& dbUrl) {
        connection = std::make_unique<pqxx::connection>(dbUrl); //создаем новое подключение
        initDB();
        connection->prepare("add_coordinate_point",
                                   "INSERT INTO coordinate_point "
                                   "VALUES ($1, $2, $3, $4, $5, "
                                   "$6, $7, $7, $8, $9, "
                                   "$10, $11, $12, $13, $14, "
                                   "$15, $16, $17, $18)");

        connection->prepare("add_rbs_reply",
                            "INSERT INTO rbs_reply "
                            "VALUES ($1, $2, $3, $4, $5, "
                            "$6, $7, $7, $8, $9, "
                            "$10, $11, $12, $13, $14, "
                            "$15, $16, $17, $18, $19 "
                            ")");

        connection->prepare("add_s_reply",
                            "INSERT INTO s_reply "
                            "VALUES ($1, $2, $3, $4, $5, "
                            "$6, $7, $7, $8, $9, "
                            "$10, $11, $12, $13, $14, "
                            "$15, $16, $17, $18, $19, "
                            "$20, $21, $22"
                            ")");

        connection->prepare("get_coordinate_point_by_id",
                            "SELECT * FROM coordinate_point "
                            "WHERE id_coordinate_point = $1");

        connection->prepare("get_rbs_reply_by_id",
                            "SELECT * FROM coordinate_point "
                            "WHERE id_coordinate_point = $1");

        connection->prepare("get_s_reply_by_id",
                            "SELECT * FROM coordinate_point "
                            "WHERE id_coordinate_point = $1");
    }

    void addCoordinatePoint(impl::CoordinatePoint& cp) const override {
        // Start a transaction
        pqxx::work txn(*connection);

        std::stringstream Css;
        cp.timestamp = std::chrono::high_resolution_clock::now();
        Css << std::chrono::high_resolution_clock::to_time_t(cp.timestamp);

        txn.exec_prepared("add_coordinate_point",
                          5, Css.str(), 1, 1, 1,
                          1, 1, 1, 1, 1,
                          false,false,false,false,false,
                          false,false,false);

        for(const Response& reply : cp.pack ){
            std::stringstream Rss;
            cp.timestamp = std::chrono::high_resolution_clock::now();
            Rss << std::chrono::high_resolution_clock::to_time_t(reply->raw.timestamp);
            if(isRbs(reply->raw.info.mode)){
                txn.exec_prepared("add_rbs_reply",
                                  5, Rss.str() , 1.0, 1.0, 1.0,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1,
                                  1.0, 1.0, 1.0, 1.0, 1.0);
            }
            if(isS(reply->raw.info.mode)){
                txn.exec_prepared("add_s_reply",
                                  5, Rss.str(),1.0,1.0,1.0,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1,
                                  1.0, 1.0, 1.0, 1.0, 1.0);
            }
        }
//        это результат, выше код для теста
//        txn.exec_prepared("add_coordinate_point",
//                          cp.idCoordinatePoint, timestamp_to_str_datetime(cp.timestamp) , cp.range, cp.azimuth, cp.amplitude,
//                          cp.mode, cp.missionCode, cp.unitCode, cp.bortNumber, cp.altitude,
//                          cp.spi, cp.xImpulse, cp.militaryAlarm, cp.isRollCall, cp.isControlResponder,
//                          cp.missionCodeIsGarbled, cp.unitCodeIsGarbled, cp.bortNumberIsGarbled, cp.heightIsGarbled);
//
//        for(const Response& reply : cp.pack ){
//            if(isRbs(reply->raw.info.mode)){
//                txn.exec_prepared("add_rbs_reply",
//                                  cp.idCoordinatePoint, timestamp_to_str_datetime(cp.timestamp) , reply->distance, reply->azimuthFromDistance, reply->phase,
//                                  reply->raw.radio.amplitude.sum, reply->raw.radio.amplitude.dif, reply->raw.radio.amplitude.pbl, reply->raw.info.mode, reply->raw.info.exInfo.rbs_modeOther.Code,
//                                  reply->raw.info.exInfo.rbs_modeOther.SPI, reply->raw.info.exInfo.rbs_modeOther.nCode, reply->raw.info.exInfo.rbs_modeOther.nSPI, reply->raw.info.exInfo.rbs_modeOther.PBL,
//                                  reply->raw.info.exInfo.rbs_modeOther.AW,
//                                  reply->monopulseAzimuth, reply->monopulseCorrection, reply->phase, reply->distance, reply->azimuthFromDistance);
//            }
//            if(isS(reply->raw.info.mode)){
//                txn.exec_prepared("add_s_reply",
//                                  cp.idCoordinatePoint, timestamp_to_str_datetime(cp.timestamp) , reply->distance, reply->azimuthFromDistance, reply->phase,
//                                  reply->raw.radio.amplitude.sum, reply->raw.radio.amplitude.dif, reply->raw.radio.amplitude.pbl, reply->raw.info.mode, reply->raw.info.exInfo.modeS.data[0],
//                                  reply->raw.info.exInfo.modeS.data[1], reply->raw.info.exInfo.modeS.data[2], reply->raw.info.exInfo.modeS.data[3], reply->raw.info.exInfo.modeS.BDS1, reply->raw.info.exInfo.modeS.BDS2,
//                                  reply->raw.info.exInfo.modeS.sicAllCall, reply->raw.info.exInfo.modeS.PC_type, reply->raw.info.exInfo.modeS.PC_EC,
//                                  reply->monopulseAzimuth, reply->monopulseCorrection, reply->phase, reply->distance, reply->azimuthFromDistance);
//            }
//        }

        txn.commit();
    }

    impl::CoordinatePoint getByIdCoordinatePoint(const uint & id_coordinate_point) override {
        pqxx::work txn(*connection);
        auto response_cp = txn.exec_prepared("get_coordinate_point_by_id",id_coordinate_point);

        impl::CoordinatePoint cp;

        std::istringstream iss{response_cp[0][1].as<std::string>()};
        std::time_t epochDateTime;
        iss >> epochDateTime;
        cp.timestamp = std::chrono::high_resolution_clock::from_time_t(epochDateTime);

//        из-за того что полей много, так будет явней, чем способ из примера, имхо
        cp.idCoordinatePoint = response_cp[0][0].as<uint>();
//        fromString(cp.timestamp, ); // уже не TODO написать каст
        cp.range = response_cp[0][2].as<double>();
        cp.azimuth = response_cp[0][3].as<double>();
        cp.amplitude = response_cp[0][4].as<uint16_t>();
        cp.mode = static_cast<SystemMode> (response_cp[0][5].as<int>());
        cp.missionCode = response_cp[0][5].as<int>();
        cp.unitCode = response_cp[0][6].as<int>();
        cp.bortNumber = response_cp[0][7].as<int>();
        cp.altitude = response_cp[0][8].as<int>();
        cp.spi = response_cp[0][9].as<bool>();
        cp.xImpulse = response_cp[0][10].as<bool>();
        cp.militaryAlarm = response_cp[0][11].as<bool>();
        cp.isRollCall = response_cp[0][12].as<bool>();
        cp.isControlResponder = response_cp[0][13].as<bool>();
        cp.missionCodeIsGarbled = response_cp[0][14].as<bool>();
        cp.unitCodeIsGarbled = response_cp[0][15].as<bool>();
        cp.bortNumberIsGarbled = response_cp[0][16].as<bool>();
        cp.heightIsGarbled = response_cp[0][17].as<bool>();

        auto response_rbs = txn.exec_prepared("get_rbs_reply_by_id", id_coordinate_point);
        for(auto reply_rbs : response_rbs){
//            тя же ло
            Timestamp ts ; // уже не TODO каст времени в строку
//            fromString(ts, reply_rbs[1].as<std::string>());

            impl::PelengatedInfo rd{reply_rbs[2].as<double>(),reply_rbs[3].as<double>(),reply_rbs[4].as<double>(),
                                    reply_rbs[5].as<uint32_t>(),reply_rbs[6].as<uint32_t>(),reply_rbs[7].as<uint32_t>()};

            POI::ResponseSignalInfo resSigInf;
//            resSigInf.Dos;
//            resSigInf.dDS;
//            resSigInf.ASP;
//            resSigInf.HIP;
            resSigInf.mode = static_cast<interrogator_v3::Mode> (reply_rbs[8].as<int>());

            resSigInf.exInfo.rbs_modeOther.Code = reply_rbs[9].as<uint32_t>();
            resSigInf.exInfo.rbs_modeOther.nCode = reply_rbs[10].as<uint32_t>();
            resSigInf.exInfo.rbs_modeOther.nSPI = reply_rbs[11].as<uint32_t>();
            resSigInf.exInfo.rbs_modeOther.PBL = reply_rbs[12].as<uint32_t>();
            resSigInf.exInfo.rbs_modeOther.AW = reply_rbs[13].as<uint32_t>();

            impl::RawAnswer rans{ts, rd, resSigInf};

            impl::Answer ans(rans);
            ans.monopulseAzimuth = reply_rbs[14].as<double>();
            ans.monopulseCorrection = reply_rbs[15].as<double>();
            ans.phase = reply_rbs[16].as<double>();
            ans.distance = reply_rbs[17].as<double>();
            ans.azimuthFromDistance = reply_rbs[18].as<double>();

            cp.pack.push_back(std::make_shared<impl::Answer>(ans));
        }

        auto response_s = txn.exec_prepared("get_s_reply_by_id", id_coordinate_point);
        for(auto reply_s : response_s){
            //            тя же ло
            Timestamp ts; // уже не TODO каст времени в строку
//            fromString(ts, reply_s[1].as<std::string>());

            impl::PelengatedInfo rd{reply_s[2].as<double>(),reply_s[3].as<double>(),reply_s[4].as<double>(),
                reply_s[5].as<uint32_t>(),reply_s[6].as<uint32_t>(),reply_s[7].as<uint32_t>()};

            POI::ResponseSignalInfo resSigInf;
//            resSigInf.Dos;
//            resSigInf.dDS;
//            resSigInf.ASP;
//            resSigInf.HIP;

            resSigInf.mode = static_cast<interrogator_v3::Mode> (reply_s[8].as<int>());

            resSigInf.exInfo.modeS.data[0] = reply_s[9].as<uint32_t>();
            resSigInf.exInfo.modeS.data[1] = reply_s[10].as<uint32_t>();
            resSigInf.exInfo.modeS.data[2] = reply_s[11].as<uint32_t>();
            resSigInf.exInfo.modeS.data[3] = reply_s[12].as<uint32_t>();
            resSigInf.exInfo.modeS.BDS1 = reply_s[13].as<uint32_t>(); // будет ли каст в битовое поле, как это работает ?
            resSigInf.exInfo.modeS.BDS2 = reply_s[14].as<uint32_t>();
            resSigInf.exInfo.modeS.sicAllCall = reply_s[15].as<uint32_t>();
            resSigInf.exInfo.modeS.PC_type = reply_s[16].as<uint32_t>();
            resSigInf.exInfo.modeS.PC_EC = reply_s[17].as<uint32_t>();

            impl::RawAnswer rans{ts, rd, resSigInf};

            impl::Answer ans(rans);
            ans.monopulseAzimuth = reply_s[18].as<double>();
            ans.monopulseCorrection = reply_s[19].as<double>();
            ans.phase = reply_s[20].as<double>();
            ans.distance = reply_s[21].as<double>();
            ans.azimuthFromDistance = reply_s[22].as<double>();

            cp.pack.push_back(std::make_shared<impl::Answer>(ans));
        }
        return cp;
    }
};

}

#endif //_DATABASE_HPP_
