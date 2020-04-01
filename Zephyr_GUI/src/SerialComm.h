#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

#define START_BYTE 0xAA     // Start byte of message (Needs to be the same on receiver!)
#define USE_CRC true

/* NOTES:
 *      - Message Composition: START_BYTE MessageLengthByte1 MessageLengthByte2 Message[Identifier, Values...] CRCbyte1 CRCbyte2
 *      - Implementation is mean to run on seperate thread:
 *          p_ArduinoSerial_    = new SerialComm();
 *          p_SerialThread_     = new QThread();
 *          p_ArduinoSerial_->moveToThread(p_SerialThread_);
 *
 *          connect(p_SerialThread_,    SIGNAL(started()),  p_ArduinoSerial_,   SLOT(init()));
 *          connect(p_SerialThread_,    SIGNAL(finished()), p_ArduinoSerial_,   SLOT(deleteLater()));
 *
 *      - Send and receive messages via signal and slot connection:
 *          connect(p_ArduinoSerial_, SIGNAL(_messageReceived_(QByteArray, qint64)), this, SLOT(_parseIncomingMessage(QByteArray, qint64)));
 *          connect(this, SIGNAL(_sendSerialMessage_(QByteArray)), p_ArduinoSerial_, SLOT(sendData(QByteArray)));
 *
 *      - Automatically reconeccts when connection is lost
 *      - Serial port is currently hardcoded
 *
 *
 * TODO's:
 *      - Make other configuration parameters settable if necessary for other uses
 */

class SerialComm : public QObject
{
    Q_OBJECT

public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm() {
        // Delete objects when this object is deleted because they are initialized without parent so they can be moved to a different thread.
        p_SerialPort_->deleteLater();
        p_connectionTimer_->deleteLater();
    }

private:
    QSerialPort *p_SerialPort_;         // Qt Serial implementation
    QTimer      *p_connectionTimer_;    // Timer to reconnect

    QByteArray m_msgBuffer_;            // Message buffer to account for incomplete messages

    int m_error_ = 0;                   // Current error code

    enum m_receiveState{                // State in received message
        expectingStartByte,                 // Looking for start of message
        expectingLengthByte1,               // Extracting first byte of message length
        expectingLengthByte2,               // Extracting second byte of message length
        expectingMessage                    // Extracting message (And check CRC)
    };



public slots:
    void init();                                            // Call when thread started to initialize everything
    void setConfiguration(QString port, qint32 baudRate);   // Configure Serial before opening (set port and baudrate)
    bool openCommPort();                                    // Opens Serial Connection
    void sendData(QByteArray message);                      // Call to transmit data

private slots:
    void _processError(QSerialPort::SerialPortError error);     // Gets triggered when error occurrs
    quint16 _getCRC(QByteArray byte_arr, int messageLength );   // Calculate Modbus CRC16 value
    void _processData();                                        // Process incoming messages

signals:
    void _messageReceived_(QByteArray, qint64);     // Emit to publish received emssage
    void _connectionError_(QString);                // Emit to publish any connection errors
    void _notConnected_();                          // Emit when connection has failed
    void _connected_();                             // Emit when connection successful
};

#endif // SERIALCOMM_H
