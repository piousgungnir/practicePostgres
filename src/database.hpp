//
// Created by gungnir on 15.02.2023.
//

#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

#include <memory>
#include <string>
#include "pqxx/pqxx"

namespace DB
{

//чисто виртуальный класс репозитория
class ICoordinatePointRepository
{
protected:
    ICoordinatePointRepository() = default; // стандартный конструктор ?нужен ли, пусть будет
    virtual ~ICoordinatePointRepository() = default; // защищенный, чисто виртуальный деструктор

public:
    ICoordinatePointRepository(const ICoordinatePointRepository &) = delete;             // запрет копирования
    ICoordinatePointRepository & operator=(const ICoordinatePointRepository &) = delete; // запрет присваивания

//    инициализация базы данных ( создание таблиц )
    virtual void initDB() = 0;
//    добавление в базу координатой точки
    virtual void addCoordinatePoint() = 0;
//    получение координатной точки из базы по её id
    virtual void getByIdCoordinatePoint() = 0;
};

//реализация репозитория с использованием Postgresql
//пока что 1 репозиторий - 1 подключение, но можно реализовать и с пулом подключений при асинхронной работе
class PostgresCPRepository : ICoordinatePointRepository {

    std::unique_ptr<pqxx::connection> connection;
public:
    explicit PostgresCPRepository(const std::string& dbUrl) {
        connection = std::make_unique<pqxx::connection>(dbUrl); //создаем новое подключение
    }

};

}

#endif //_DATABASE_HPP_
