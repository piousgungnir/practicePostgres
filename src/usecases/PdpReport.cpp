#include "PdpReport.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

//#include <stdssr/ModeS/SignalFormats.h>

//#include "icr/forms/ReplyForms/Rbs.h"
//#include "icr/forms/ReplyForms/Parol.h"
//#include "icr/forms/ReplyForms/ModeS.h"
//#include "icr/forms/CoordinatePointForms/RbsConsolidatedCpForm.h"
//#include "icr/forms/CoordinatePointForms/ParolConsolidatedCpForm.h"

QJsonObject makeJsonObject(QList<form::FieldElementaryInfo> data)
{
    QJsonObject obj;

    /*
    for (auto & field : data)
        obj[field.name] = field.value;
    */

    return obj;
}

PdpReport::PdpReport()
{
    //здесь вызов setupConfig() был
    //    auto poiSettings = allshare::Distributor::get<Settings<config::AppSpecific>>();
    //    poiSettings->add(this);
}

PdpReport::~PdpReport()
{
    writeRemainingData();
}

void PdpReport::addCp(const impl::CoordinatePoint & cp)
{
    if (!isRecord)
        return;

    QJsonObject cpData;
    cpData["Impl General"] = formImplGeneral(cp);
    cpData["Replies"] = formRbsModeSAnswers(cp);

    coordinatePoints.push_back(cpData);
}

void PdpReport::addCp(const esgrlo::CoordinatePoint & cp)
{
    if (!isRecord)
        return;

    QJsonObject cpData;
    cpData["Esgrlo general"] = formEsgrloGeneral(cp);
    cpData["Replies"] = formParolAnswers(cp);

    coordinatePoints.push_back(cpData);
}

void PdpReport::onNorth(int turnoverNumber)
{
    if (!isRecord)
        return;

    currentTurnoverNum = turnoverNumber;
    writeReport();
}

void PdpReport::setupConfig()
{
    /*
    if (!settings())
        return;

    settings()->beginGroup("ESGRLO/POK");
    {
        m_pokCR = static_cast<uint8_t>(settings()->value("pokCR", 2).toUInt());
    }
    settings()->endGroup();
    settings()->beginGroup("PdpReport/filters");
    {
        excludedImplGeneralItems = settings()->value("excludedImplGeneralItems").toStringList();
        excludedRadioInfoItems = settings()->value("excludedRadioInfoItems").toStringList();
    }
    settings()->endGroup();
    */
}

void PdpReport::setIsRecord()
{
    isRecord = !isRecord;

    if (not isRecord)
        writeRemainingData();
}

QJsonObject PdpReport::formImplGeneral(const impl::CoordinatePoint & cp)
{
    /*
    auto data = form::makeCpRbs(cp);

    auto json = makeJsonObject(data);
    for (auto name : excludedImplGeneralItems)
    {
        if (json.contains(name))
            json.remove(name);
    }

    return json;
    */
}

QJsonObject PdpReport::formEsgrloGeneral(const esgrlo::CoordinatePoint & cp)
{
    /*
    auto data = form::makeCpParol(cp);
    return makeJsonObject(data);
    */
}

QJsonArray PdpReport::formRbsModeSAnswers(const impl::CoordinatePoint & cp)
{
    QJsonArray answers;
    for (auto & answer : cp.pack)
    {
        QList<form::FieldElementaryInfo> data;

        if (interrogator_v3::isRbs(answer.get()->raw.info.mode))
        {
            // data = form::makeRbs(*answer);
        }
        else if (interrogator_v3::isS(answer.get()->raw.info.mode))
        {
            // data = form::makeS(*answer);
        }

        data += formRadioInfo(*answer);

        auto json = makeJsonObject(data);

        for (auto name : excludedRadioInfoItems)
        {
            if (json.contains(name))
                json.remove(name);
        }
        answers.push_back(json);
    }
    answers.push_front(QString::number(cp.pack.size()));

    return answers;
}

QJsonArray PdpReport::formParolAnswers(const esgrlo::CoordinatePoint & cp)
{
    QJsonArray answers;

    /*
    for (auto & answer : cp.pack)
    {
        auto data = form::makeParol(*answer, m_pokCR);

        data += formRadioInfo(*answer);

        auto json = makeJsonObject(data);

        for (auto name : excludedRadioInfoItems)
        {
            if (json.contains(name))
                json.remove(name);
        }
        answers.push_back(json);
    }
    */

    return answers;
}

QList<form::FieldElementaryInfo> PdpReport::formRadioInfo(const impl::Answer & answer)
{
    /*
    auto radioInfo = form::makeRadioInfo(answer);

    return radioInfo;
    */
}

void PdpReport::writeReport()
{
    QFile report(fileName);

    if (report.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QJsonObject turnover;

        turnover["CPs"] = coordinatePoints;
        turnover["turnover"] = currentTurnoverNum;


        QJsonDocument doc;
        if (report.size() == 0)
        {
            QJsonArray array;
            array.push_back(turnover);
            doc.setArray(array);
            report.write(doc.toJson());
        }
        else //Костыльная вставка объекта в массив уже записанного файла
        {
            report.seek(report.size() - 3);
            report.write(", \n");
            doc.setObject(turnover);
            report.seek(report.size());
            report.write(doc.toJson());
            report.write("] ");
        }

        coordinatePoints = {};

        report.close();
    }
}

void PdpReport::writeRemainingData()
{
    if (not coordinatePoints.isEmpty())
    {
        currentTurnoverNum++; //запись оставшихся данных
        writeReport();
    }
}

void PdpReport::setFileName(const QString & value)
{
    fileName = value;
}
