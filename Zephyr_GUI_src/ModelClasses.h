#ifndef MODELCLASSES_H
#define MODELCLASSES_H

// include Qt Classes
#include <QObject>
#include <QString>

// include library headers
#include "QQmlVarPropertyHelpers.h"
#include "QQmlAutoPropertyHelpers.h"

// Object to store parameters for the different measurement values
class MeasurementValue;

class MeasurementValue: public QObject {
    Q_OBJECT
    QML_CONSTANT_AUTO_PROPERTY      (QString            , name)
    QML_CONSTANT_AUTO_PROPERTY      (QString            , unit)
    QML_CONSTANT_AUTO_PROPERTY      (int                , upperLimit)
    QML_CONSTANT_AUTO_PROPERTY      (int                , lowerLimit)
    QML_READONLY_AUTO_PROPERTY      (double             , value)
    QML_READONLY_AUTO_PROPERTY      (int                , target)
    QML_READONLY_AUTO_PROPERTY      (int                , maxAlarm)
    QML_READONLY_AUTO_PROPERTY      (int                , minAlarm)
    QML_READONLY_AUTO_PROPERTY      (int                , warning)
    QML_READONLY_AUTO_PROPERTY      (bool               , minActive)
    QML_READONLY_AUTO_PROPERTY      (bool               , maxActive)

public:
    explicit MeasurementValue (QString name, QString unit, int target, int lowerLimit, int upperLimit,  int minAlarm,  int maxAlarm, bool minActive,bool maxActive, QObject * parent = Q_NULLPTR)
        : QObject (parent)
        , m_name                    {name}
        , m_unit                    {unit}
        , m_upperLimit              {upperLimit}
        , m_lowerLimit              {lowerLimit}
        , m_value                   {0}
        , m_target                  {target}
        , m_maxAlarm                {maxAlarm}
        , m_minAlarm                {minAlarm}
        , m_warning                 {0}
        , m_minActive               {minActive}
        , m_maxActive               {maxActive}

    {}
};


#endif // MODELCLASSES_H
