#ifndef VENTILATOR_CONTROL_H
#define VENTILATOR_CONTROL_H

#include "Sensors.h"
#include "Actuators.h"

#define PRESSURE_CONTROL 0
#define VOLUME_CONTORL 1
#define FLOW_CONTROL 2

#define TIME_TRIGGER 0
#define PATIENT_TRIGGER 1
#define DUAL_TRIGGER 2

#define INSPIRATION 0
#define POST_INSP_HOLD 1
#define EXPIRATION 2
#define POST_EXP_HOLD 3

class Control
{
private:
  int mode;
  int trigger;
  int stage;
  float kp[3];
  float ki[3];
  float kd[3];
  float volume;
  float prev_flow;
  float error;
  float max_pressure;
  float min_pressure;
  float set_volume;
  float set_flow;
  float set_breath_rate;
  float inspiration_duration;
  float expiration_duration;
  int cycle_start_time;
  int stage_start_time;
  float valve_expiration_position;
  
public:
  Control(int i_mode, int i_trigger, float* i_kp, float* i_ki, float* i_kd, float i_max_pressure, float i_min_pressure, float i_set_volume, float i_set_flow, float i_set_breath_rate, float i_valve_expiration_position);
  void run_control_loop(const Sensors* sensors, Actuators* actuators);
  void pressure_inspiration();
  void volume_inspiration();
  void flow_inspiration();
  void start_expiration();
  
  int get_mode() {return mode;}
  int get_trigger() {return trigger;}
  int get_stage() {return stage;}
  float get_volume() {return volume;}
  float get_max_pressure() {return max_pressure;}
  float get_min_pressure() {return min_pressure;}
  float get_set_volume() {return set_volume;}
  float get_set_flow() {return set_flow;}
  float get_set_breath_rate() {return set_breath_rate;}
  float get_inpiration_duration() {return inspiration_duration;}
  float get_inspiration_duration() {return expiration_duration;}
  
  void set_mode(int m) {mode = m;}
  void set_trigger (int g) {trigger = g;}
  void set_stage(int s) {stage = s;}
  void set_max_pressure(float p) {max_pressure = p;}
  void set_min_pressure(float p) {min_pressure = p;}
  void set_set_volume(float v) {set_volume = v;}
  void set_set_flow(float f) {set_flow = f;}
  void set_set_breath_rate(float b) {set_breath_rate = b;}
  void set_valve_expiration_position(float vp) {valve_expiration_position = (vp >= 0 && vp <= 180)?vp:valve_expiration_position;}
};

#endif
