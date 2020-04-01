#include "SerialComm.h"

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{

}

// Initialize outside of constructor so that the object can be moved to a different thread first before creating QSerialPort object and QTimer
void SerialComm::init()
{
    // Create Objects
    p_SerialPort_       = new QSerialPort();
    p_connectionTimer_  = new QTimer();

    // Connect necessary Signal - Slots
    connect(p_SerialPort_,          SIGNAL(readyRead()),                                this,       SLOT(_processData()));
    connect(p_SerialPort_,          SIGNAL(error(QSerialPort::SerialPortError)),        this,       SLOT(_processError(QSerialPort::SerialPortError)), Qt::DirectConnection);
    connect(p_connectionTimer_,     SIGNAL(timeout()),                                  this,       SLOT(openCommPort()));

    // Configure Serial Port and open it - TODO: change to be settable from UI
    setConfiguration("cu.usbserial-1460", 115200);
    openCommPort();
}

// Set the QSerialPort configuration
void SerialComm::setConfiguration(QString port, qint32 baudRate)
{
    p_SerialPort_->setPortName      (port);
    p_SerialPort_->setBaudRate      (baudRate);
    p_SerialPort_->setDataBits      (QSerialPort::Data8);
    p_SerialPort_->setParity        (QSerialPort::NoParity);
    p_SerialPort_->setStopBits      (QSerialPort::OneStop);
    p_SerialPort_->setFlowControl   (QSerialPort::NoFlowControl);
}

// Open communication
bool SerialComm::openCommPort()
{
    // Check if Serial port has been configured else return error
    if (p_SerialPort_->portName().isEmpty())
    {
        emit _connectionError_("Serial Connection was not configured");
    }
    else if(p_SerialPort_->open(QIODevice::ReadWrite))  // Attempt to open the connection
    {
        // Connection was successful
        p_connectionTimer_->stop(); // Stop trying to reconnect
        emit _connected_();         // Notify connection success
        m_error_ = 0;               // Reset error to 0
        return true;
    }

    // Connection unsuccessful
    emit _notConnected_();          // Notify connection failure
    return false;
}

// Compose message and send
void SerialComm::sendData(QByteArray message)
{
#ifdef USE_CRC
    int msgLen = message.length() + 2;                  // Add 2 bytes for crc
    quint16 crc = _getCRC(message, message.length());   // Calculate crc
#else
    int msgLen = message.length();
#endif
    QByteArray packet;
    packet += char(START_BYTE);         // Set start byte
    packet += char(msgLen >> 8);        // Set first message length byte
    packet += char(msgLen & 0xFF);      // Set second message length byte
    packet += message;                  // Add message
#ifdef USE_CRC
    packet += char(crc & 0xFF);         // Add first crc byte
    packet += char(crc >> 8);           // Add second crc byte
#endif

#ifdef QT_DEBUG
    if (p_SerialPort_->write(packet) == -1)
        qDebug() << "ERROR: Sending message failed";    // TODO: Maybe display to user
#endif
}

// Process incoming message
void SerialComm::_processData()
{
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();     // Safe timestamp of message arrival
    m_receiveState state = expectingStartByte;                  // Initialize state

    quint16 messageLengthByte1=0;   // Variable to store first length byte
    quint16 messageLengthByte2=0;   // Variable to store second length byte
    quint16 messageLength=0;        // Variable to store full length
    uchar currentChar;              // Variable to store each byte in iteration

    m_msgBuffer_ += p_SerialPort_->readAll();                   // Add incoming data to buffer

    // Iterate though Message and check for all sections
    for (int i=0; i<m_msgBuffer_.size(); ++i)
    {
        currentChar = uchar(m_msgBuffer_.at(i));
        switch(state){
        case expectingStartByte:    // Find start byte
            if(currentChar == START_BYTE)
                state = expectingLengthByte1;
            break;

        case expectingLengthByte1:  // Get first message byte
            messageLengthByte1 = currentChar;
            state = expectingLengthByte2;
            break;

        case expectingLengthByte2:  // Get second message byte and combine them
            messageLengthByte2 = currentChar;
            messageLength = messageLengthByte1 * 256 + messageLengthByte2;
            state = expectingMessage;
            break;
        case expectingMessage:
            // Extract full message
            QByteArray fullMsg = m_msgBuffer_.mid(i, messageLength);

            // Full message hasn't been received
            if (messageLength > fullMsg.size())
            {
                state = expectingStartByte;
                m_msgBuffer_ = fullMsg;
            }
#ifdef USE_CRC
            // Check crc
            else if (_getCRC(fullMsg, fullMsg.size()) == 0)
            {
                // crc successfule
                m_msgBuffer_.clear();
                emit _messageReceived_(fullMsg.remove(fullMsg.size()-2,2), timestamp);
                state = expectingStartByte;
            }
#ifdef QT_DEBUG
            else
            {
                qDebug() << "ERROR: Corrupted message received. Checksum wrong" << fullMsg;
            }
#endif
#else
            else
            {
                m_msgBuffer_.clear();
                emit _messageReceived_(fullMsg);
                state = expectingStartByte;
            }
#endif
            break;
        }
    }
}

// Calculate CRC
#ifdef USE_CRC
uint16_t SerialComm::_getCRC( QByteArray message, int messageLength )
{
    // taken from: https://stackoverflow.com/questions/19358246/crc-ccitt-to-crc16-modbus-implementation
    /*************************************************************************
     * phpCrc16 v1.1 -- CRC16/CCITT implementation
     *
     * By Matteo Beccati <matteo@beccati.com>
     *
     * Original code by:
     * Ashley Roll
     * Digital Nemesis Pty Ltd
     * www.digitalnemesis.com
     * ash@digitalnemesis.com
     *
     * Test Vector: "123456789" (character string, no quotes)
     * Generated CRC: 0x29B1
     * Modified for Zephyr implementation
     *
     *************************************************************************/

    static const uint16_t _modbusCRC16_Table[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
    };

    uint16_t crc = 0xFFFF;
    uint8_t  tableIndex;
    int index = 0;

    while ( messageLength-- )
    {
        tableIndex = uint8_t (message.at(index++) ^ crc);
        crc >>= 8;
        crc ^= _modbusCRC16_Table[tableIndex];
    }

    return crc;
}
#endif

// Handle Error
void SerialComm::_processError(QSerialPort::SerialPortError error)
{

    //    NoError                      0	No error occurred.
    //    DeviceNotFoundError	       1	An error occurred while attempting to open an non-existing device.
    //    PermissionError              2	An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.
    //    OpenError                    3	An error occurred while attempting to open an already opened device in this object.
    //    NotOpenError                 13	This error occurs when an operation is executed that can only be successfully performed if the device is open. This value was introduced in QtSerialPort 5.2.
    //    ParityError                  4	Parity error detected by the hardware while reading data.
    //    FramingError                 5	Framing error detected by the hardware while reading data.
    //    BreakConditionError	       6	Break condition detected by the hardware on the input line.
    //    WriteError                   7	An I/O error occurred while writing the data.
    //    ReadError                    8	An I/O error occurred while reading the data.
    //    ResourceError                9	An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system.
    //    UnsupportedOperationError	   10	The requested device operation is not supported or prohibited by the running operating system.
    //    TimeoutError                 12	A timeout error occurred. This value was introduced in QtSerialPort 5.2.
    //    UnknownError                 11	An unidentified error occurred.

    // Deal with new errors
    if (error > 0 && m_error_ != error)
    {
        m_error_ = error;
#ifdef QT_DEBUG
        qDebug() <<"SERIAL CONNECTION ERROR: " << QString::number(error);
#endif
        // Close serial port if still open
        if (p_SerialPort_->isOpen())
            p_SerialPort_->close();

        // Try to reconnect
        p_connectionTimer_->start(1000);

        // Identify error and publish it
        switch (error) {
        case 1:
            emit _connectionError_("Device not found.");
            break;
        case 2:
            emit _connectionError_("Permission error.");
            break;
        case 3:
            emit _connectionError_("Port already open.");
            break;
        case 4:
            emit _connectionError_("Parity error.");
            break;
        case 5:
            emit _connectionError_("Framing error.");
            break;
        case 6:
            emit _connectionError_("Break Condition error.");
            break;
        case 7:
            emit _connectionError_("Write error.");
            break;
        case 8:
            emit _connectionError_("Read error.");
            break;
        case 9:
            emit _connectionError_("Resource error.");
            break;
        case 10:
            emit _connectionError_("Unsupported Operation.");
            break;
        case 11:
            emit _connectionError_("Unknown");
            break;
        case 12:
            emit _connectionError_("Timeout");
            break;
        default:
            emit _connectionError_("Unknown");
            break;
        }
    }
}







