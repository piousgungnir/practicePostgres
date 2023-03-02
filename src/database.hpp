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

extern long id;

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
    virtual void addCoordinatePoint(const impl::CoordinatePoint& cp) const = 0;
//    получение координатной точки из базы по её id
    virtual impl::CoordinatePoint getByIdCoordinatePoint(const uint & id_coordinate_point) = 0;
//    получение координальных точек за период времени (нужно подумать над аргументами)
    [[nodiscard]] virtual std::vector<impl::CoordinatePoint> getByTimeIntervalCoordinatePoints(const std::chrono::high_resolution_clock::time_point & time_interval_start,
                                                                                 const std::chrono::high_resolution_clock::time_point & time_interval_end) const = 0;
//    получение координатных точек с одним бортовым номером
    [[nodiscard]] virtual std::vector<impl::CoordinatePoint> getByBortNumberCoordinatePoints(const uint & bort_number) const = 0;
////    получение координатных точек по отрезку дальности
    [[nodiscard]] virtual std::vector<impl::CoordinatePoint> getByRangeSegmentCoordinatePoints(const double & range_start,
                                                                                    const double & range_end) const = 0;
};

//реализация репозитория с исползованием Postgresql
//есть проблемы с ошибками при запросах. Я даже не обрабатываю случай когда ничего не возвращается, или у CP нет ответов,
//но это вопрос дальнейшего рефакторинга и все делается возможностями pqxx
class PostgresCPRepository : ICoordinatePointRepository {
private:
    std::unique_ptr<pqxx::connection> connection;
//    каст времени для запросов к бд
    static std::string timePointToString(const std::chrono::high_resolution_clock::time_point & timePoint){
        std::stringstream Css;
        Css << std::chrono::high_resolution_clock::to_time_t(timePoint);
        return Css.str();
    }
    static std::chrono::high_resolution_clock::time_point stringToTimePoint(const std::string & result){
        std::istringstream iss{result};
        std::time_t epochDateTime;
        iss >> epochDateTime;
        return std::chrono::high_resolution_clock::from_time_t(epochDateTime);
    }
    static void buildOneCpFromRow (const pqxx::row & row_cp, impl::CoordinatePoint & cp){
        cp.idCoordinatePoint = row_cp[0].as<uint>();
        cp.timestamp = stringToTimePoint(row_cp[1].as<std::string>());
        cp.range = row_cp[2].as<double>();
        cp.azimuth = row_cp[3].as<double>();
        cp.amplitude = row_cp[4].as<uint16_t>();
        cp.mode = static_cast<SystemMode> (row_cp[5].as<int>());
        cp.missionCode = row_cp[6].as<int>();
        cp.unitCode = row_cp[7].as<int>();
        cp.bortNumber = row_cp[8].as<int>();
        cp.altitude = row_cp[9].as<int>();
        cp.spi = row_cp[10].as<bool>();
        cp.xImpulse = row_cp[11].as<bool>();
        cp.militaryAlarm = row_cp[12].as<bool>();
        cp.isRollCall = row_cp[13].as<bool>();
        cp.isControlResponder = row_cp[14].as<bool>();
        cp.missionCodeIsGarbled = row_cp[15].as<bool>();
        cp.unitCodeIsGarbled = row_cp[16].as<bool>();
        cp.bortNumberIsGarbled = row_cp[17].as<bool>();
        cp.heightIsGarbled = row_cp[18].as<bool>();
    }
    static void buildRbsAnswerFromRow (const pqxx::row & row_rbs, impl::CoordinatePoint & cp) {
        Timestamp ts = stringToTimePoint(row_rbs[1].as<std::string>()); //   можно не использовать перемменную ts

        impl::PelengatedInfo rd{row_rbs[2].as<double>(),row_rbs[3].as<double>(),row_rbs[4].as<double>(),
                                row_rbs[5].as<uint32_t>(),row_rbs[6].as<uint32_t>(),row_rbs[7].as<uint32_t>()};

        POI::ResponseSignalInfo resSigInf;
        //            resSigInf.Dos;
        //            resSigInf.dDS;
        //            resSigInf.ASP;
        //            resSigInf.HIP;
        resSigInf.mode = static_cast<interrogator_v3::Mode> (row_rbs[8].as<int>());

        resSigInf.exInfo.rbs_modeOther.Code = row_rbs[9].as<uint32_t>();
        resSigInf.exInfo.rbs_modeOther.SPI = row_rbs[10].as<uint32_t>();
        resSigInf.exInfo.rbs_modeOther.nCode = row_rbs[11].as<uint32_t>();
        resSigInf.exInfo.rbs_modeOther.nSPI = row_rbs[12].as<uint32_t>();
        resSigInf.exInfo.rbs_modeOther.PBL = row_rbs[13].as<uint32_t>();
        resSigInf.exInfo.rbs_modeOther.AW = row_rbs[14].as<uint32_t>();

        impl::RawAnswer rans{ts, rd, resSigInf};

        impl::Answer ans(rans);
        ans.monopulseAzimuth = row_rbs[15].as<double>();
        ans.monopulseCorrection = row_rbs[16].as<double>();
        ans.phase = row_rbs[17].as<double>();
        ans.distance = row_rbs[18].as<double>();
        ans.azimuthFromDistance = row_rbs[19].as<double>();

        cp.pack.push_back(std::make_shared<impl::Answer>(ans));
    }
    static void buildSAnswerFromRow (const pqxx::row & row_s, impl::CoordinatePoint & cp) {
        Timestamp ts = stringToTimePoint(row_s[1].as<std::string>()); //   TODO можно не использовать перемменную ts, проверить каст с numeric

        impl::PelengatedInfo rd{row_s[2].as<double>(), row_s[3].as<double>(), row_s[4].as<double>(),
                                row_s[5].as<uint32_t>(), row_s[6].as<uint32_t>(), row_s[7].as<uint32_t>()};

        POI::ResponseSignalInfo resSigInf;
        //            resSigInf.Dos;
        //            resSigInf.dDS;
        //            resSigInf.ASP;
        //            resSigInf.HIP;

        resSigInf.mode = static_cast<interrogator_v3::Mode>(row_s[8].as<int>());

        resSigInf.exInfo.modeS.data[0] = row_s[9].as<uint32_t>();
        resSigInf.exInfo.modeS.data[1] = row_s[10].as<uint32_t>();
        resSigInf.exInfo.modeS.data[2] = row_s[11].as<uint32_t>();
        resSigInf.exInfo.modeS.data[3] = row_s[12].as<uint32_t>();
        resSigInf.exInfo.modeS.BDS1 = row_s[13].as<uint32_t>(); // будет ли каст в битовое поле, как это работает ?
        resSigInf.exInfo.modeS.BDS2 = row_s[14].as<uint32_t>();
        resSigInf.exInfo.modeS.sicAllCall = row_s[15].as<uint32_t>();
        resSigInf.exInfo.modeS.PC_type = row_s[16].as<uint32_t>();
        resSigInf.exInfo.modeS.PC_EC = row_s[17].as<uint32_t>();

        impl::RawAnswer rans{ts, rd, resSigInf};

        impl::Answer ans(rans);
        ans.monopulseAzimuth = row_s[18].as<double>();
        ans.monopulseCorrection = row_s[19].as<double>();
        ans.phase = row_s[20].as<double>();
        ans.distance = row_s[21].as<double>();
        ans.azimuthFromDistance = row_s[22].as<double>();

        cp.pack.push_back(std::make_shared<impl::Answer>(ans));
    }
public:
    void initDB() const override {
        pqxx::work txn(*connection);
        txn.exec("CREATE TABLE IF NOT EXISTS coordinate_point "
                 "("
                 "    id_coordinate_point    BIGINT           not null"
                 "        constraint coordinate_point_pk"
                 "            primary key, "
                 "    time_coordinate_point   numeric             not null," // тип
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
                 "( id_coordinate_point   bigint    not null,"
//                 "        constraint rbs_reply_coordinate_point_id_coordinate_point_fk"
//                 "            references coordinate_point,"
                 "    time_reply                 numeric          not null," // тип
                 "    distance                        double precision,"
                 "    azimuth                         double precision,"
                 "    phase                           double precision,"
                 "    sum_ampl                        numeric,"
                 "    dif_ampl                        numeric,"
                 "    pbl_ampl                        numeric,"
                 "    mode                            integer,"
                 "    code                            numeric,"
                 "    spi                             numeric,"
                 "    n_code                          numeric,"
                 "    n_spi                           numeric,"
                 "    is_under_pbl                    numeric,"
                 "    aw                              numeric,"
                 "    monopulse_azimuth               double precision,"
                 "    monopulse_correction            double precision,"
                 "    processed_phase                 double precision,"
                 "    processed_distance              double precision,"
                 "    azimuth_from_distance           double precision"
                 ");");

        txn.exec("CREATE TABLE IF NOT EXISTS s_reply"
                 "( id_coordinate_point   bigint    not null,"
//                 "        constraint rbs_reply_coordinate_point_id_coordinate_point_fk"
//                 "            references coordinate_point,"
                 "    time_reply         numeric         not null," // тип
                 "    distance                double precision,"
                 "    azimuth                 double precision,"
                 "    phase                   double precision,"
                 "    sum_ampl                numeric,"
                 "    dif_ampl                numeric,"
                 "    pbl_ampl                numeric,"
                 "    mode                    numeric,"
                 "    data0                   numeric,"
                 "    data1                   numeric,"
                 "    data2                   numeric,"
                 "    data3                   numeric,"
                 "    bsd1                    numeric,"
                 "    bsd2                    numeric,"
                 "    sic_all_call            numeric,"
                 "    pc_type                 numeric,"
                 "    pc_EC                   numeric,"
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
                            "(id_coordinate_point, time_coordinate_point, range, azimuth, amplitude, mode, mission_code, unit_code, bort_number, "
                            "altitude, spi, x_impulse, military_alarm, is_roll_call, is_control_responder, mission_code_is_garbled, unit_code_is_garbled, "
                            "bort_Number_is_garbled, height_is_garbled) "
                            "VALUES ($1, $2, $3, $4, $5, "
                            "$6, $7, $8, $9, "
                            "$10, $11, $12, $13, $14, "
                            "$15, $16, $17, $18, $19)");
        connection->prepare("add_rbs_reply",
                            "INSERT INTO rbs_reply "
                            "VALUES ($1, $2, $3, $4, $5, "
                            "$6, $7, $8, $9, "
                            "$10, $11, $12, $13, $14, "
                            "$15, $16, $17, $18, $19, "
                            "$20)");
        connection->prepare("add_s_reply",
                            "INSERT INTO s_reply "
                            "VALUES ($1, $2, $3, $4, $5, "
                            "$6, $7, $8, $9, "
                            "$10, $11, $12, $13, $14, "
                            "$15, $16, $17, $18, $19, "
                            "$20, $21, $22, $23"
                            ")");


        connection->prepare("get_coordinate_point_by_id",
                            "SELECT * FROM coordinate_point "
                            "WHERE id_coordinate_point = $1");
        connection->prepare("get_coordinate_point_by_time_interval",
                            "SELECT * FROM coordinate_point "
                            "WHERE time_coordinate_point >= $1 AND time_coordinate_point <= $2");
        connection->prepare("get_coordinate_point_by_bort_number",
                            "SELECT * FROM coordinate_point "
                            "WHERE bort_number = $1");
        connection->prepare("get_coordinate_point_by_segment_of_the_range",
                            "SELECT * FROM coordinate_point "
                            "WHERE range >= $1 AND range <= $2");


        connection->prepare("get_rbs_reply_by_id",
                            "SELECT * FROM rbs_reply "
                            "WHERE id_coordinate_point = $1");


        connection->prepare("get_s_reply_by_id",
                            "SELECT * FROM s_reply "
                            "WHERE id_coordinate_point = $1");
    }

    void addCoordinatePoint(const impl::CoordinatePoint& cp) const override {
        // Start a transaction
        pqxx::work txn(*connection);

//        txn.exec_prepared("add_coordinate_point",
//                          11, timePointToString(cp.timestamp), 1, 1, 1,
//                          1, 1, 1, 1, 1,
//                          false,false,false,false,false,
//                          false,false,false);
////        std::stringstream RBSss;
////        cp.timestamp = std::chrono::high_resolution_clock::now();
////        RBSss << std::chrono::high_resolution_clock::to_time_t(reply->raw.timestamp);
//
//        txn.exec_prepared("add_rbs_reply",
//                          6, timePointToString(cp.timestamp), 1.0, 1.0, 1.0,
//                          1, 1, 1, 1, 1,
//                          1, 1, 1, 1, 1,
//                          1.0, 1.0, 1.0, 1.0);
//
//        txn.exec_prepared("add_s_reply",
//                          6, timePointToString(cp.timestamp),1.0,1.0,1.0,
//                          1, 1, 1, 1, 1,
//                          1, 1, 1, 1, 1,
//                          1, 1, 1,
//                          1.0, 1.0, 1.0, 1.0);

//        это результат, выше код для теста
        txn.exec_prepared("add_coordinate_point",
//                          cp.idCoordinatePoint
                          ++id, std::chrono::high_resolution_clock::to_time_t(cp.timestamp) , cp.range, cp.azimuth, cp.amplitude,
                          static_cast<int>(cp.mode), cp.missionCode, cp.unitCode, cp.bortNumber, cp.altitude,
                          cp.spi, cp.xImpulse, cp.militaryAlarm, cp.isRollCall, cp.isControlResponder,
                          cp.missionCodeIsGarbled, cp.unitCodeIsGarbled, cp.bortNumberIsGarbled, cp.heightIsGarbled);

        for(const Response& reply : cp.pack ){
            if(isRbs(reply->raw.info.mode)){
                txn.exec_prepared("add_rbs_reply",
//                                  cp.idCoordinatePoint
                                  id, std::chrono::high_resolution_clock::to_time_t(reply->raw.timestamp), reply->distance, reply->azimuthFromDistance, reply->phase,
                                  reply->raw.radio.amplitude.sum, reply->raw.radio.amplitude.dif, reply->raw.radio.amplitude.pbl, static_cast<int>(reply->raw.info.mode), reply->raw.info.exInfo.rbs_modeOther.Code,
                                  reply->raw.info.exInfo.rbs_modeOther.SPI, reply->raw.info.exInfo.rbs_modeOther.nCode, reply->raw.info.exInfo.rbs_modeOther.nSPI, reply->raw.info.exInfo.rbs_modeOther.PBL,
                                  reply->raw.info.exInfo.rbs_modeOther.AW,
                                  reply->monopulseAzimuth, reply->monopulseCorrection, reply->phase, reply->distance, reply->azimuthFromDistance);
            }
            if(isS(reply->raw.info.mode)){
                txn.exec_prepared("add_s_reply",
//                                  cp.idCoordinatePoint
                                  id, std::chrono::high_resolution_clock::to_time_t(reply->raw.timestamp), reply->distance, reply->azimuthFromDistance, reply->phase,
                                  reply->raw.radio.amplitude.sum, reply->raw.radio.amplitude.dif, reply->raw.radio.amplitude.pbl, static_cast<int>(reply->raw.info.mode), reply->raw.info.exInfo.modeS.data[0],
                                  reply->raw.info.exInfo.modeS.data[1], reply->raw.info.exInfo.modeS.data[2], reply->raw.info.exInfo.modeS.data[3], reply->raw.info.exInfo.modeS.BDS1, reply->raw.info.exInfo.modeS.BDS2,
                                  reply->raw.info.exInfo.modeS.sicAllCall, reply->raw.info.exInfo.modeS.PC_type, reply->raw.info.exInfo.modeS.PC_EC,
                                  reply->monopulseAzimuth, reply->monopulseCorrection, reply->phase, reply->distance, reply->azimuthFromDistance);
            }
        }

        txn.commit();
    }

    impl::CoordinatePoint getByIdCoordinatePoint(const uint & id_coordinate_point) override {
        pqxx::work txn(*connection);
        auto response_cp = txn.exec_prepared("get_coordinate_point_by_id",id_coordinate_point);

        impl::CoordinatePoint cp;
        buildOneCpFromRow(response_cp[0], cp);

        auto response_rbs = txn.exec_prepared("get_rbs_reply_by_id", id_coordinate_point);
//        for(int i = 0; i < response_rbs.size(); i++){ //for test
//            std::cout << "RBS check " << response_rbs[i][0].as<int>() << std::endl;
//        }
        for(auto row_rbs : response_rbs){
            buildRbsAnswerFromRow(row_rbs, cp);
        }

        auto response_s = txn.exec_prepared("get_s_reply_by_id", id_coordinate_point);
//        for(int i = 0; i < response_s.size(); i++){
//            std::cout << "RBS check " << response_s[i][0].as<int>() << std::endl;
//        }
        for(auto row_s : response_s){
            buildSAnswerFromRow(row_s, cp);
        }
        return cp;
    }

    [[nodiscard]] std::vector<impl::CoordinatePoint> getByTimeIntervalCoordinatePoints(const std::chrono::high_resolution_clock::time_point & time_interval_start,
                                                                         const std::chrono::high_resolution_clock::time_point & time_interval_end) const override{
        std::vector<impl::CoordinatePoint> CoordinatePoints{}; //   !!!можно сразу увеличивать размер для оптимизации, либо вообще использовать array и брать размер из sql запроса, вектор проще но спорно
        pqxx::work txn(*connection);

        auto response_cp = txn.exec_prepared("get_coordinate_point_by_time_interval", timePointToString(time_interval_start),
                                             timePointToString(time_interval_end));
//        auto response_cp = txn.exec_prepared("get_coordinate_point_by_time_interval", 1677520450, 1677520498); //for test
        for(auto row_cp : response_cp){
            impl::CoordinatePoint cp;
            buildOneCpFromRow(row_cp, cp);

            auto response_rbs = txn.exec_prepared("get_rbs_reply_by_id", row_cp[0].as<uint>());
            for(auto row_rbs : response_rbs){
                buildRbsAnswerFromRow(row_rbs, cp);
            }

            auto response_s = txn.exec_prepared("get_s_reply_by_id", row_cp[0].as<uint>());
            for(auto row_s : response_s){
                buildSAnswerFromRow(row_s, cp);
            }
        }
        return CoordinatePoints;
    }

    [[nodiscard]] std::vector<impl::CoordinatePoint> getByBortNumberCoordinatePoints(const uint & bort_number) const override {
        std::vector<impl::CoordinatePoint> CoordinatePoints{}; //   !!!можно сразу увеличивать размер для оптимизации, либо вообще использовать array и брать размер из sql запроса, вектор проще но спорно
        pqxx::work txn(*connection);

        //        auto response_cp = txn.exec_prepared("get_coordinate_point_by_time_interval", timePointToString(time_interval_start), timePointToString(time_interval_end));
        auto response_cp = txn.exec_prepared("get_coordinate_point_by_bort_number", bort_number);
        for(auto row_cp : response_cp){
            impl::CoordinatePoint cp;
            buildOneCpFromRow(row_cp, cp);

            auto response_rbs = txn.exec_prepared("get_rbs_reply_by_id", row_cp[0].as<uint>());
            for(auto row_rbs : response_rbs){
                buildRbsAnswerFromRow(row_rbs, cp);
            }

            auto response_s = txn.exec_prepared("get_s_reply_by_id", row_cp[0].as<uint>());
            for(auto row_s : response_s){
                buildSAnswerFromRow(row_s, cp);
            }

            CoordinatePoints.push_back(cp);
        }
        return CoordinatePoints;
    }

    [[nodiscard]] std::vector<impl::CoordinatePoint> getByRangeSegmentCoordinatePoints(const double & range_start,
                                                                            const double & range_end) const override {
        std::vector<impl::CoordinatePoint> CoordinatePoints{}; //   !!!можно сразу увеличивать размер для оптимизации, либо вообще использовать array и брать размер из sql запроса, вектор проще но спорно
        pqxx::work txn(*connection);

        //        auto response_cp = txn.exec_prepared("get_coordinate_point_by_time_interval", timePointToString(time_interval_start), timePointToString(time_interval_end));
        auto response_cp = txn.exec_prepared("get_coordinate_point_by_segment_of_the_range", range_start, range_end);
        for(auto row_cp : response_cp){
            impl::CoordinatePoint cp;
            buildOneCpFromRow(row_cp, cp);

            auto response_rbs = txn.exec_prepared("get_rbs_reply_by_id", row_cp[0].as<uint>());
            for(auto row_rbs : response_rbs){
                buildRbsAnswerFromRow(row_rbs, cp);
            }

            auto response_s = txn.exec_prepared("get_s_reply_by_id", row_cp[0].as<uint>());
            for(auto row_s : response_s){
                buildSAnswerFromRow(row_s, cp);
            }

            CoordinatePoints.push_back(cp);
        }
        return CoordinatePoints;
    }
};
}

#endif //_DATABASE_HPP_
