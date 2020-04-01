#include "Actuators.h"

Actuators::Actuators(int valve1_pin, int valve2_pin, int motor_pin)
{
  valve1.attach(valve1_pin);
  valve2.attach(valve2_pin);
  motor.attach(motor_pin);
}

bool Actuators::set_valve1(float angle)
{
  if (angle >= 0 && angle <= 180)
  {
    valve1.write(angle);
    return true;
  }
  return false;
}

bool Actuators::set_valve2(float angle)
{
  if (angle >= 0 && angle <= 180)
  {
    valve2.write(angle);
    return true;
  }
  return false;
}

bool Actuators::set_motor(float percentage)
{
  if (percentage >= 0 && percentage <= 100)
  {
    motor.write(percentage * 180 / 100);
    return true;
  }
  return false;
}

bool Actuators::change_valve1(float angle)
{
  return set_valve1(valve1.read() + angle);
}

bool Actuators::change_valve2(float angle)
{
  return set_valve2(valve2.read() + angle);
}

bool Actuators::change_motor(float percentage)
{
  return set_motor(motor.read() + percentage * 180 / 100);
}
