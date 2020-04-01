#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Arduino.h"

#define BUFFER_SIZE 1024
#define START_BYTE 0xAA

// define the serial port used for gcode input
#define RpiSerial Serial



class SerialComm
{
  public:

  //Set Variable: reset after each reading
  int pressure_set_maximum;
  int pressure_set_minimun;
  int volume_set;
  int flow_set;
  int breath_rate_set;

  //Modes:
  bool pressure_control = 0;
  bool volume_control = 0;
  bool patient_triggered = 0;
  bool timed_triggered = 0;
  bool dual_triggered = 0;
  bool variable_set = 0;

  //Alarms setting:
  int pressure_alarm_max = 0;
  int pressure_alarm_min = 0;
  int tidal_vol_alarm_min = 0;
  int tidal_vol_alarm_max = 0;
  bool sensor_fault_alarm = 0;
  bool disconnection_alarm = 0;
  bool oxygen_level_alarm = 0;
  bool power_source_alarm = 0;
  bool low_exp_pressure_alarm = 0;
  bool alarm_reset = 0;

  SerialComm(void);
  void ReadRpiSerial();
  void processBuffer();
  void SendBufferToRpi();
  String buffer_out = "";


  void InitSerial(long baud);

  private:

  char buffer_in[BUFFER_SIZE];
  int packet_len = 0;

  void blinkLed();
  float getfloatatlocation(String bufferstring, int pos);
  float intergercomma(int commapos);
  bool buffer_received = 0;

  uint16_t getCRC16( String data, int len );

  enum m_processState_{
    expectingStartByte,
    expectingLengthByte1,
    expectingLengthByte2,
    expectingMessage
  };
  m_processState_ state = expectingStartByte;
  int bufferindex = 0;
  int messageLengthByte1=0;
  int messageLengthByte2=0;
  int messageLength=0;
};

#endif
