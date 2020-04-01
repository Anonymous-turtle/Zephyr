#include "Control.h"

Control::Control(int i_mode, int i_trigger, float* i_kp, float* i_ki, float* i_kd, float i_max_pressure, float i_min_pressure, float i_set_volume, float i_set_flow, float i_set_breath_rate, float i_valve_expiration_position):
		mode(i_mode), trigger(i_trigger), max_pressure(i_max_pressure), min_pressure(i_min_pressure), set_volume(i_set_volume), set_flow(i_set_flow), set_breath_rate(i_set_breath_rate), valve_expiration_position(i_valve_expiration_position)
{
	kp[0] = *i_kp;
  kp[1] = *(++i_kp);
  kp[2] = *(++i_kp);
  ki[0] = *i_ki;
  ki[1] = *(++i_ki);
  ki[2] = *(++i_ki);
  kd[0] = *i_kd;
  kd[1] = *(++i_kd);
  kd[2] = *(++i_kd);
  stage = 3;
  volume = 0;
  prev_flow = 0;
  error = 0;
  inspiration_duration = 30.0/set_breath_rate;
  expiration_duration = 30.0/set_breath_rate;
  cycle_start_time = 0;
  stage_start_time = 0;
}

void Control::run_control_loop(const Sensors* sensors, Actuators* actuators)
{
  
}

void Control::pressure_inspiration()
{
  
}

void Control::volume_inspiration()
{
  
}

void Control::flow_inspiration()
{
  
}

void Control::start_expiration()
{
  
}

void Control::pid(int mode)
{
  
}