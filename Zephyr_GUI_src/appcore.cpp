#include "AppCore.h"

AppCore::AppCore(QObject *parent)
    : QObject(parent)
    , m_valueModel      {new QQmlObjectListModel<MeasurementValue>{this}}
    , m_xRange          {10000}
    , m_alarm           {""}
    , m_mode            {PressureMode}
{
    //qRegisterMetaType<QAbstractSeries *>();
    //qRegisterMetaType<QAbstractAxis *>();

    // Initialize MeasurementValue Objects here
    for (int i = 0; i<ValueEnumEnd; i++)
    {
        // TODO set presets
        //                                          name        unit       target   lower   upper   min     max     minActive   maxActive
        switch (i) {
        case P_aw:
            m_valueModel->append(new MeasurementValue("Paw",    "cmH2O",    35,     15,     40,     30,     40,     true,       true,       this));
            break;
        case Flow:
            m_valueModel->append(new MeasurementValue("Flow",   "l/min",    0,     -100,    100,      30,     20,     false,      false,      this));
            break;
        case V_t:
            m_valueModel->append(new MeasurementValue("Vt",     "ml",       450,    250,    800,    30,     20,     true,       true,       this));
            break;
        case P_peak:
            m_valueModel->append(new MeasurementValue("Ppeak",  "cmH2O",    0,      0,      0,      0,      0,      false,      false,      this));
            break;
        case P_mean:
            m_valueModel->append(new MeasurementValue("Pmean",  "cmH2O",    0,      0,      0,      0,      0,      false,      false,      this));
            break;
        case PEEP:
            m_valueModel->append(new MeasurementValue("PEEP",   "cmH2O",    10,      5,     25,      9,     11,     true,       true,       this));
            break;
        case RR:
            m_valueModel->append(new MeasurementValue("RR",     "b/min",    15,      6,     40,     13,     17,     true,       true,       this));
            break;
        case F_iO2:
            m_valueModel->append(new MeasurementValue("FiO2",   "%",        60,     21,    100,     55,     65,     true,       true,       this));
            break;
        case IE_ratio:
            m_valueModel->append(new MeasurementValue("I:E",    "",         2,      1,      4,      1,      3,      true,       true,       this));
            break;
        default:
            break;
        }
    }

    // Initialize communication
    _initializeSerialComm();

}

void AppCore::_initializeSerialComm()
{
    // Create Serial object and QThread
    p_ArduinoSerial_ = new SerialComm();
    p_SerialThread_ = new QThread();

    // Move Serial Object to dedicated thread
    p_ArduinoSerial_->moveToThread(p_SerialThread_);

    // Initialize Singal Slot connections
    connect(p_SerialThread_,    SIGNAL(started()),                          p_ArduinoSerial_,   SLOT(init()));
    connect(p_SerialThread_,    SIGNAL(finished()),                         p_ArduinoSerial_,   SLOT(deleteLater()));
    connect(this,               SIGNAL(_sendSerialMessage_(QByteArray)),    p_ArduinoSerial_,   SLOT(sendData(QByteArray)));

    connect(p_ArduinoSerial_,   SIGNAL(_connected_()),                          this,   SLOT(_onConnected()));
    connect(p_ArduinoSerial_,   SIGNAL(_connectionError_(QString)),             this,   SLOT(_onConnectionError(QString)));
    connect(p_ArduinoSerial_,   SIGNAL(_messageReceived_(QByteArray, qint64)),  this,   SLOT(_parseIncomingMessage(QByteArray, qint64)));

    // Start thread
    p_SerialThread_->start();
}

void AppCore::setSeries(int index, QAbstractSeries *series1, QAbstractSeries *series2)
{
    // Initialize the correct Series. Maybe implement Q_ENUM as id
    if (series1 && series2) {
        switch (index) {
        case 1:
            p_pressureSeriesA_ = static_cast<QXYSeries *>(series1);
            p_pressureSeriesB_ = static_cast<QXYSeries *>(series2);
            break;
        case 2:
            p_flowSeriesA_ = static_cast<QXYSeries *>(series1);
            p_flowSeriesB_ = static_cast<QXYSeries *>(series2);
            break;
        case 3:
            p_VolumeSeriesA_ = static_cast<QXYSeries *>(series1);
            p_VolumeSeriesB_ = static_cast<QXYSeries *>(series2);
            break;
#ifdef QT_DEBUG
        default:
            qDebug() << "ERROR: QXYSeries not configured";
            break;
#endif
        }
        _configureSeries(series1, series2);
    }
}

void AppCore::setSeries(int index, QAbstractSeries *series)
{
    // Initialize the correct Series. Maybe implement Q_ENUM as id
    if (series) {
        switch (index) {
        case 1:
            p_VolumePressureSeries_ = static_cast<QXYSeries *>(series);
            break;
        default:
            qDebug() << "ERROR: QXYSeries not configured";
            break;
        }
        // FUTURE_TODO: _configureSeries(series);
    }
}

void AppCore::_configureSeries(QAbstractSeries *series1, QAbstractSeries *series2)
{
    // FUTURE_TODO: configure any other desired parameters.
    series1->useOpenGL();
    series2->useOpenGL();
}

void AppCore::_updateChart(double yVal, qint64 measurementTime, QXYSeries *series1, QXYSeries *series2)
{
    // Resetn time offset to stay within 0 to xRange and loop back to beginning of chart
    if (measurementTime > (m_xOffset_ + m_xRange))
    {
        m_xOffset_ = measurementTime;
        m_flipflopLines = !m_flipflopLines;
    }

    // Set xValue
    quint16 xVal = measurementTime - m_xOffset_;

    // flipflop between the two lines
    QXYSeries *increasingSeries = m_flipflopLines? series1: series2;
    QXYSeries *decreasingSeries = m_flipflopLines? series2: series1;

    // Add points to increasing line
    QVector<QPointF> pointsIncreasing = increasingSeries->pointsVector();
    pointsIncreasing.append(QPointF(double(xVal)/1000, yVal));

    // Remove points from decreasing line
    QVector<QPointF> pointsDecreasing = decreasingSeries->pointsVector();
    while (pointsDecreasing.count() && pointsDecreasing.at(0).x()*1000 < xVal+m_xSpace_)
        pointsDecreasing.removeFirst();

    // Update lines
    increasingSeries->replace(pointsIncreasing);
    decreasingSeries->replace(pointsDecreasing);
}


void AppCore::setTempValueObj(int index)
{
    // Create new temporary object to store values before committing to them
    m_valueModel->append(new MeasurementValue(m_valueModel->at(index)->get_name(),
                                              m_valueModel->at(index)->get_unit(),
                                              m_valueModel->at(index)->get_target(),
                                              m_valueModel->at(index)->get_lowerLimit(),
                                              m_valueModel->at(index)->get_upperLimit(),
                                              m_valueModel->at(index)->get_minAlarm(),
                                              m_valueModel->at(index)->get_maxAlarm(),
                                              m_valueModel->at(index)->get_minActive(),
                                              m_valueModel->at(index)->get_maxActive(),
                                              this));

    // Set remaining values
    m_valueModel->last()->update_warning(m_valueModel->at(index)->get_warning());
}

void AppCore::removeTempValueObj()
{
    // Remove all unwanted temporary objects (should only be one)
    while (m_valueModel->at(ValueEnumEnd))
        m_valueModel->remove(ValueEnumEnd);
}

void AppCore::updateTempTarget(int val)
{
    m_valueModel->last()->update_target(val);
}

void AppCore::updateTempMin(int val)
{
    m_valueModel->last()->update_minAlarm(val);
}

void AppCore::updateTempMax(int val)
{
    m_valueModel->last()->update_maxAlarm(val);
}

void AppCore::saveTempValues(int index)
{
    // TODO: send updates to controller

    // Commit changes anch check if it is the correct index
    if (m_valueModel->at(index)->get_name() == m_valueModel->at(index)->get_name())
    {
        m_valueModel->at(index)->update_target(m_valueModel->at(index)->get_target());
        m_valueModel->at(index)->update_minAlarm(m_valueModel->last()->get_minAlarm());
        m_valueModel->at(index)->update_maxAlarm(m_valueModel->last()->get_maxAlarm());
        m_valueModel->at(index)->update_warning(m_valueModel->last()->get_warning());
    }
    else
    {
        update_alarm("Error while setting values.");
    }
    removeTempValueObj();
}


void AppCore::_parseIncomingMessage(QByteArray message, qint64 timestamp)
{
    // TODO! handle incoming message
    //qDebug() << "Message from Arduino: " << message;
    QList<QByteArray> buffer_strList = message.split(',');

    // Message From Arduino = pressure1,flow,volume,
    int bufferIndex = 0;
    double inlet_pressure = 0;
    double flow_ml_per_min = 0;
    double volume_ml = 0;

    for(int i = 0;i<buffer_strList.size()-1;++i){
        //        qDebug()<<"ArduinoBuffer["<<i<<"]:"<<buffer_strList.at(i);

        if( i == 0){
            inlet_pressure =  buffer_strList.at(bufferIndex).toDouble();
        }
        else if( i == 1){
            flow_ml_per_min =  buffer_strList.at(bufferIndex).toDouble();
        }
        else if( i == 2){
            volume_ml =  buffer_strList.at(bufferIndex).toDouble();
        }
        else{
            qDebug()<<"ERROR PARSING MESSAGE FROM ARDUINO - OUT OF INDEX";
        }
    }

    qDebug() << "Message from Arduino: " << message;

    // To update Pressure Curve
    _updateChart(inlet_pressure  , timestamp, p_pressureSeriesA_, p_pressureSeriesB_);

    // To update Flow Curve
    _updateChart(flow_ml_per_min  , timestamp, p_flowSeriesA_, p_flowSeriesB_);

    // To update Volume Curve
    _updateChart(volume_ml  , timestamp, p_VolumeSeriesA_, p_VolumeSeriesB_);


    m_valueModel->at(P_aw)->update_value(int(inlet_pressure));
    m_valueModel->at(Flow)->update_value(int(flow_ml_per_min));
    m_valueModel->at(V_t)->update_value(int(volume_ml));
    _updatePMeanMinMax();

    // TODO: get from Arduino
    m_valueModel->at(RR)->update_value(1);
    m_valueModel->at(F_iO2)->update_value(1);
    m_valueModel->at(IE_ratio)->update_value(1);

    _checkForAlarms();
}

void AppCore::_updatePMeanMinMax()
{
    // TEST:
    if (p_pressureSeriesA_->pointsVector().isEmpty())
        return;
    QVector<QPointF> pointVector;
    int min = 10000;
    int max = -1000;
    int sum = 0;
    int count = 0;
    pointVector = p_pressureSeriesA_->pointsVector();
    min = pointVector.first().y();
    max = min;
    sum = 0;
    count = pointVector.count();

    // find min, max from first series and calculate sum
    foreach(QPointF point, pointVector)
    {
        if (point.y() < min)
            min = point.y();
        else if (point.y() > max)
            max = point.y();

        sum += point.y();
    }

    pointVector = p_pressureSeriesB_->pointsVector();
    // find min, max from second series and calculate sum
    foreach(QPointF point, pointVector)
    {
        if (point.y() < min)
            min = point.y();
        else if (point.y() > max)
            max = point.y();

        sum += point.y();
    }
    count += pointVector.count();

    m_valueModel->at(P_peak)->update_value(max);
    m_valueModel->at(PEEP)->update_value(min);
    m_valueModel->at(P_mean)->update_value(int(sum/count));
}

void AppCore::_checkForAlarms()
{
    QString alarm = "";
    if (m_valueModel->count() >= ValueEnumEnd)
    {
        for (int i=0; i<ValueEnumEnd; i++)
        {
            // Update any active alarms
            if (m_valueModel->at(i)->get_maxActive() && m_valueModel->at(i)->get_value() >= m_valueModel->at(i)->get_maxAlarm())
            {
                if (!alarm.isEmpty())
                    alarm += " & ";
                alarm += (m_valueModel->at(i)->get_name() + " > " + QString::number(m_valueModel->at(i)->get_maxAlarm()));
            }
            else if (m_valueModel->at(i)->get_minActive() && m_valueModel->at(i)->get_value() <= m_valueModel->at(i)->get_minAlarm())
            {
                if (!alarm.isEmpty())
                    alarm += " & ";
                alarm += (m_valueModel->at(i)->get_name() + " < " + QString::number(m_valueModel->at(i)->get_minAlarm()));
            }

        }
        if (!alarm.isEmpty())
        {
            // Set new alarm if new alarm was found
            update_alarm(alarm);
        }
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "ERROR: Value Model is not initialized correctly";
    }
#endif
}

void AppCore::resetAlarms()
{
    // Only reset alarm if its not serial related
    if (m_serialConnected)
        update_alarm("");
}


void AppCore::_onConnected()
{
    // Connect
    m_serialConnected = true;
    update_alarm("");
}

void AppCore::_onConnectionError(QString error)
{
    m_serialConnected = false;
    update_alarm("Connection Error: " + error);
}








