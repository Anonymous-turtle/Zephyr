#ifndef ACTUATORS_H
#define ACTUATORS_H
#include <Servo.h>

class Actuators
{
private:
  Servo valve1;
  Servo valve2;
  Servo motor;

public:
  Actuators(int valve1_pin, int valve2_pin, int motor_pin);
  bool set_valve1(float angle);
  bool set_valve2(float angle);
  bool set_motor(float percentage);
  bool change_valve1(float angle);
  bool change_valve2(float angle);
  bool change_motor(float percentage);
};

#endif
