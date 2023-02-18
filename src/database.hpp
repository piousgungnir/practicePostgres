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

namespace DB
{

//чисто виртуальный класс репозитория
class ICoordinatePointRepository
{
protected:
    virtual ~ICoordinatePointRepository() = default; // защищенный, чисто виртуальный деструктор

public:
//    инициализация базы данных ( создание таблиц )
    virtual void initDB() = 0;
//    добавление в базу координатой точки
    virtual void addCoordinatePoint(impl::CoordinatePoint cp) = 0;
//    получение координатной точки из базы по её id
    virtual impl::CoordinatePoint getByIdCoordinatePoint(const std::string& idCP) = 0;
};

//реализация репозитория с исползованием Postgresql
//пока что 1 репозиторий - 1 подключение, но можно реализовать и с пулом подключений при асинхронной работе
class PostgresCPRepository : ICoordinatePointRepository {
private:
    std::unique_ptr<pqxx::connection> connection;
public:
    void initDB() override {
        pqxx::work txn(*connection);
        txn.exec("create table \"coordinatepoint\" "
                 "("
                 "    \"idcoordinatepoint\"    BIGINT           not null"
                 "        constraint \"CoordinatePoint_pk\""
                 "            primary key, "
                 "    \"timeCoordinatePoint\"  text       not null,"
                 "    \"range \"               double precision not null,"
                 "    \"azimuth \"             double precision not null,"
                 "    \"amplitude \"           numeric          not null,"
                 "    mode                   integer          not null,"
                 "    \"missionCode\"          integer,"
                 "    \"unitCode \"            integer,"
                 "    \"bortNumber\"           integer,"
                 "    altitude               integer,"
                 "    spi                    bool,"
                 "    \"xImpulse\"             bool,"
                 "    \"militaryAlarm\"        bool,"
                 "    \"isRollCall\"           bool,"
                 "    \"isControlResponder\"   bool,"
                 "    \"missionCodeIsGarbled\" bool,"
                 "    \"unitCodeIsGarbled\"    bool,"
                 "    \"bortNumberIsGarbled\"  bool,"
                 "    \"heightIsGarbled\"      bool);");
        txn.commit();
    }

    PostgresCPRepository(const std::string& dbUrl, const bool& initDatabaseFlag) {
        connection = std::make_unique<pqxx::connection>(dbUrl); //создаем новое подключение
        if (initDatabaseFlag == true) {
            initDB();
        }
        connection->prepare("addCP",
                                   "INSERT INTO coordinatepoint "
//                                   "(idcoordinatePoint, timecoordinatecoint, range, azimuth, amplitude, " //затуп с заглавными буквами, pqxx делает все lowcase, не понял
//                                   "mode, missionCode, unitCode, bortNumber, altitude, "
//                                   "spi, xImpulse, militaryAlarm, isRollCall, isControlResponder, "
//                                   "missionCodeIsGarbled, unitCodeIsGarbled, bortNumberIsGarbled, heightIsGarbled) "
                                   "VALUES ($1, $2, $3, $4, $5, "
                                   "$6, $7, $7, $8, $9, "
                                   "$10, $11, $12, $13, $14, "
                                   "$15, $16, $17, $18)");

        connection->prepare("getById",
                            "SELECT * FROM coordinatepoint "
                            "WHERE idcoordinatepoint = $1");
    }

    void addCoordinatePoint(impl::CoordinatePoint cp) override {
        // Start a transaction
        pqxx::work txn(*connection);
        // query
//        txn.exec_prepared("addCP",
//                          2);
        txn.exec_prepared("addCP",
                          5, timestamp_to_str_datetime(cp.timestamp).c_str() ,1,1,1,
                          1,1,1,1,1,
                          false,false,false,false,false,
                          false,false,false);
        txn.commit();
    }

    impl::CoordinatePoint getByIdCoordinatePoint(const std::string& idCP) override {
        pqxx::work txn(*connection);
        pqxx::result response = txn.exec_prepared("getById",idCP);

        impl::CoordinatePoint cp;

//        for(auto column : response[0]){ // вывод на экран всех столбцов строки
//            std::cout << pqxx::to_string(column) << std::endl;
//        }

        cp.idCoordinatePoint = std::stoi(pqxx::to_string(response[0][0]));
//        cp.timestamp = pqxx::to_string(response[0][1]); // ??? пробелма с кастом, нагуглил только boost либы. И каст с postgrsql timestomp в time_point, тоже
        cp.range = std::stod(pqxx::to_string(response[0][2]));
        cp.azimuth = std::stod(pqxx::to_string(response[0][3]));
        cp.amplitude = (uint16_t)std::stoll(pqxx::to_string(response[0][4])); // ??? как кастить си стринг в uint_16(
        cp.mode = static_cast<SystemMode>(std::stoi(pqxx::to_string(response[0][5])));
        cp.missionCode = std::stoi(pqxx::to_string(response[0][6]));
        cp.unitCode = std::stoi(pqxx::to_string(response[0][7]));
        cp.bortNumber = std::stoi(pqxx::to_string(response[0][8]));
        cp.altitude = std::stoi(pqxx::to_string(response[0][9]));
        std::istringstream(pqxx::to_string(response[0][10])) >> cp.spi;
        std::istringstream(pqxx::to_string(response[0][11])) >> cp.xImpulse;
        std::istringstream(pqxx::to_string(response[0][12])) >> cp.militaryAlarm;
        std::istringstream(pqxx::to_string(response[0][13])) >> cp.isRollCall;
        std::istringstream(pqxx::to_string(response[0][14])) >> cp.isControlResponder;
        std::istringstream(pqxx::to_string(response[0][15])) >> cp.missionCodeIsGarbled;
        std::istringstream(pqxx::to_string(response[0][16])) >> cp.unitCodeIsGarbled;
        std::istringstream(pqxx::to_string(response[0][17])) >> cp.bortNumberIsGarbled;
        std::istringstream(pqxx::to_string(response[0][18])) >> cp.heightIsGarbled;

        return cp;
    }
};

}

#endif //_DATABASE_HPP_
