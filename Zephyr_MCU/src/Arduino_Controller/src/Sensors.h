#ifndef SENSORS_H
#define SENSORS_H

#define I_INLET 0
#define I_THROAT 1
#define E_INLET 2
#define E_THROAT 3

class Sensors
{
private:
  float pressure[4];
  float i_flow_rate;
  float e_flow_rate;
  float slope[4];
  float offset[4];
  float Cd;
  float density;
  float throat_area;
  float outlet_area;
  int pin[4];

public:
  Sensors(int i_inlet_pin, int i_throat_pin, int e_inlet_pin, int e_throat_pin, float i_inlet_slope, float i_throat_slope, float e_inlet_slope, float e_throat_slope, float i_inlet_offset, float i_throat_offset, float e_inlet_offset, float e_throat_offset, float discharge_coefficient, float fluid_density, float throat_diameter, float outlet_diameter);
  void read_sensors();
  float get_i_venturi_throat_pressure() {return pressure[I_INLET];}
  float get_i_venturi_inlet_pressure() {return pressure[I_THROAT];}
  float get_e_venturi_throat_pressure() {return pressure[E_INLET];}
  float get_e_venturi_inlet_pressure() {return pressure[E_THROAT];}
  float get_i_flow_rate() {return i_flow_rate;}
  float get_e_flow_rate() {return e_flow_rate;}
};

#endif
