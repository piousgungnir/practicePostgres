#include <array>
#include <iostream>

#include "domain/CoordinatePoint.h"
#include "domain/ICpConsumers.h"
#include "domain/POIProtocol.h"

#include "endpoints/rli/PdpClient.h"

#include "network/NanomsgClient.h"

void template_main()
{
    //ответы имеются разных типов
    // RBS, S, НРЗ
    //типичная информация ответа RBS
    POI::ExtendedInfo info;
    info.rbs_modeOther.Code = 01234;  //код ответа
    info.rbs_modeOther.nCode = 00000; // биты недостоверности кода: 0 - достоверно, 1 - недостоверно
    info.rbs_modeOther.SPI = true;    // Специальная индикация положения (SPI)
    info.rbs_modeOther.nSPI = false;  // бит недостоверности
    info.rbs_modeOther.AW = false;    // авария
    info.rbs_modeOther.PBL = false;   // 0 - над ПБЛ, 1 - под ПБЛ;
                                      // под ПБЛ значит ответ не должен участвовать
                                      // в формировании информации по самолёту


    // самолёт, или как мы называем координатная точка
    // тоже бывает разных типов
    // RBS/S
    impl::CoordinatePoint cp;
    cp.modeS; //тут информация по режиму S; возможно есть лишнее. можно на первых порах исключить вектора
    //и НРЗ также синонимы Пароль/ЕС ГРЛО
    esgrlo::CoordinatePoint nrzCp;
}

class CpRepositoryMock : public IRbs_SConsumer
{
public:
    // IRbs_SConsumer interface
    void addCp(const impl::CoordinatePoint & cp) override
    {
        std::cout << std::hex << cp.modeS.AA << std::dec << std::endl;
    }
    void addAsInfo(asinfo_t &) override{};
};


int main()
{
    RemotePdp remotePdp;
    PdpClient pdpClient(&remotePdp, nullptr, nullptr);
    CpRepositoryMock cpRep;
    remotePdp.moduleAtcrbs()->addConsumer(&cpRep);


    NanomsgPipelineClient nnSub("tcp://172.17.0.1:49900"); // TODO: get from config
    ipc::connect(&pdpClient, &nnSub, ipc::ConnectionWay::OnlyReceive);


    // eventloop на минималках
    for (;;)
        std::this_thread::sleep_for(std::chrono::minutes{1});

    return 0;
}
