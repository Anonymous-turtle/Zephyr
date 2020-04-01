#include "src/SerialCom.h"
#include "src/Sensors.h"
#include "src/Actuators.h" 
#include "src/Control.h"

#define VALVE1 5
#define VALVE2 6
#define MOTOR 9

#define I_INLET_PIN A0
#define I_THROAT_PIN A1
#define E_INLET_PIN A2
#define E_THROAT_PIN A3     
#define I_INLET_SLOPE 1
#define I_THROAT_SLOPE 1
#define E_INLET_SLOPE 1
#define E_THROAT_SLOPE 1
#define I_INLET_OFFSET 0
#define I_THROAT_OFFSET 0
#define E_INLET_OFFSET 0
#define E_THROAT_OFFSET 0
#define DENSITY 1
#define THROAT_DIAM 0.005     ///Meters
#define OUTLET_DIAM 0.01
#define CD 0.95

#define MODE 0                  ///TODO: read from EEPROM of latest settings
#define TRIGGER 0
#define MAX_PRESSURE 10
#define MIN_PRESSURE 1
#define SET_VOLUME 1
#define SET_FLOW 1
#define SET_BREATH_RATE 1
#define VALVE_EXPIRATION_POSITION 0

SerialComm RPiComm;
const float kp[3] {0.1, 0.1, 0.1};
const float ki[3] {0.1, 0.1, 0.1};
const float kd[3] {0.1, 0.1, 0.1};

void setup()
{
  
}

void loop()
{
  static Sensors ptd(I_INLET_PIN, I_THROAT_PIN, E_INLET_PIN, E_THROAT_PIN, I_INLET_SLOPE, I_THROAT_SLOPE, E_INLET_SLOPE, E_THROAT_SLOPE, I_INLET_OFFSET, I_THROAT_OFFSET, E_INLET_OFFSET, E_THROAT_OFFSET, CD, DENSITY, THROAT_DIAM, OUTLET_DIAM);
  static Actuators act(VALVE1, VALVE2, MOTOR);
  static Control ctrl(MODE, TRIGGER, kp, ki, kd, MAX_PRESSURE, MIN_PRESSURE, SET_VOLUME, SET_FLOW, SET_BREATH_RATE, VALVE_EXPIRATION_POSITION);
  
}
