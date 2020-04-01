#ifndef APPCORE_H
#define APPCORE_H

// include Qt Modules
#include <QtCharts/QAreaSeries>
#include <QtCharts/QXYSeries>
#include <QtCharts/QAbstractSeries>
#include <QtCore/QObject>
#include <QTimer>
#include <QDateTime>
#include <QThread>

// include library headers
#include "QQmlObjectListModel.h"

// include own classes
#include "ModelClasses.h"
#include "SerialComm.h"

QT_CHARTS_USE_NAMESPACE

//Q_DECLARE_METATYPE(QAbstractSeries *)
//Q_DECLARE_METATYPE(QAbstractAxis *)

/* NOTES:
 *      - Hosts all data and handles data steam
 *      -
 *
 *
 * TODO's:
 *      - Finish communication with Controller
 *          - Receive data
 *          - Send mode changes
 *          - Send alarm changes
 *          - Implement receive confirmation from controller
 *          - Request settings from controller
 *      - Improve alarm strategy
 *      - Implement Comm connection functionality
 *      - Evaluate default values
 *      - Evaluate control values
 *      - Implement advanced trigger types
 *      - Check calculated values!
 *      - Consider setting IE ad double and not int
 *      - Figure out deployment
 *
 * MAYBES:
 *      - Remote connection
 *      - E-mail notofication
 *      - More advanced configuration menu
 *      - Adjust for bigger screens
 *      - Improve for mouse
 *      - Device id
 *
 */

class AppCore : public QObject {
    Q_OBJECT

    // Variables and Objects that need to be accessible from within QML
    QML_OBJMODEL_PROPERTY           (valueModel         , MeasurementValue)    // Object list model to contain all values. They need to be initialized in constructor and linked to ValueEnum as indices

    QML_READONLY_AUTO_PROPERTY      (quint16            , xRange)   // Range in xAxis in plots
    QML_READONLY_AUTO_PROPERTY      (QString            , alarm)    // Current alarm. Displayed in UI.

    QML_WRITABLE_AUTO_PROPERTY      (int                , mode)     // Selected mode


public:
    explicit AppCore( QObject *parent = 0);

    // Enum to itentify items in valueModel.
    enum ValueEnum {
        P_aw,           // Airway pressure
        Flow,           // Flow to patient
        V_t,            // Tidal Volume to patient

        P_peak,         // Peak airway pressure
        P_mean,         // Mean airway pressure (unused)
        PEEP,           // Positive end expiratory pressure

        RR,             // Respiratory rate
        F_iO2,          // Fraction of inspired oxygen
        IE_ratio,       // Inspiration:Expiration ratio
        ValueEnumEnd
    };
    Q_ENUM(ValueEnum)

    enum ModeEnum {
        PressureMode,   // Ventilate to target pressure
        VolumeMode      // Ventilate to taget volume
    };
    Q_ENUM(ModeEnum)

private:
    // Objects for serial implementation
    SerialComm *p_ArduinoSerial_;
    QThread *p_SerialThread_;

    // QXYSeries for pressue chart
    QXYSeries *p_pressureSeriesA_;
    QXYSeries *p_pressureSeriesB_;

    // QXYSeries for flow chart
    QXYSeries *p_flowSeriesA_;
    QXYSeries *p_flowSeriesB_;

    // QXYSeries for volume chart
    QXYSeries *p_VolumeSeriesA_;
    QXYSeries *p_VolumeSeriesB_;

    // QXYSeries for volume-pressure chart (unused)
    QXYSeries *p_VolumePressureSeries_;


    bool m_flipflopLines = true;        // Variable to flip flop between lines for overlapping display
    qint64 m_xOffset_ = 0;              // Timestamp offset on plot x-axis
    quint16 m_xSpace_ = 100;            // Spacing between line (Separation strip)

    bool m_serialConnected = false;     // Store serial connection for alarm handling

public slots:
    // Initialize plots
    void setSeries(int index, QAbstractSeries *series1, QAbstractSeries *series2);
    void setSeries(int index, QAbstractSeries *series);

    // Deal with temp object to set values before comitting them
    void setTempValueObj(int index);
    void removeTempValueObj();
    void saveTempValues(int index);
    void updateTempTarget(int val);
    void updateTempMin(int val);
    void updateTempMax(int val);

    // Handle alamrs
    void resetAlarms();

private slots:
    // Initialize the communication
    void _initializeSerialComm();

    // Handle charts
    void _updateChart(double yVal, qint64 measurementTime, QXYSeries *series1, QXYSeries *series2);
    void _configureSeries(QAbstractSeries *series1, QAbstractSeries *series2);

    // Handle incoming data
    void _parseIncomingMessage(QByteArray message, qint64 timestamp);
    void _updatePMeanMinMax();
    void _checkForAlarms();

    // Handle Serial COnnection
    void _onConnected();
    void _onConnectionError(QString error);

signals:
    // Emit to send message (needed for signal slot connection to other thread)
    void _sendSerialMessage_(QByteArray);
};

#endif // APPCORE_H


