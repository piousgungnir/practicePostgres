#pragma once

#include <QFile>
#include <QJsonArray>
#include <QObject>
#include <QTextStream>

#include "domain/CoordinatePoint.h"
#include "domain/ICpConsumers.h"

//#include "icr/forms/ReplyForms/BaseReplyForm.h"

//#include "Settings.h"
#include "domain/azimuth/INorthDependent.h"

namespace form
{
struct FieldElementaryInfo
{
    //зашлушка
};
} // namespace form

class PdpReport
    : public IParolConsumer
    , public IRbs_SConsumer
    , public INorthDependent
//    , public Settupable
{

public:
    PdpReport();
    ~PdpReport() override;

    void addCp(const impl::CoordinatePoint &) override;
    void addCp(const esgrlo::CoordinatePoint &) override;

    void addAlarmBearing(const Azimuth &) override
    {
    }
    void addAsInfo(asinfo_t &) override
    {
    }

    void onNorth(int turnoverNumber) override;

    void setupConfig() /*override*/;

    void setIsRecord();

    void setFileName(const QString & value);

private:
    QJsonObject formImplGeneral(const impl::CoordinatePoint & cp);
    QJsonObject formEsgrloGeneral(const esgrlo::CoordinatePoint & cp);

    QJsonArray formRbsModeSAnswers(const impl::CoordinatePoint & cp);

    QJsonArray formParolAnswers(const esgrlo::CoordinatePoint & cp);

    QList<form::FieldElementaryInfo> formRadioInfo(const impl::Answer & answe);

    void writeReport();
    void writeRemainingData();

private:
    QJsonArray coordinatePoints;

    int currentTurnoverNum = 0;
    uint8_t m_pokCR = 2;

    bool isRecord = false;
    QString fileName;

    QStringList excludedImplGeneralItems = {
        "Azimuth",
        "Amplitude"};

    QStringList excludedRadioInfoItems = {
        "Monopulse azimuth",
        "Monopulse correct",
        "Phase diff"};
};
