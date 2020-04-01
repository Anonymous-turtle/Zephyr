#include <Arduino.h>
//#include <Math.h>
#include "Sensors.h"

Sensors::Sensors(int i_inlet_pin, int i_throat_pin, int e_inlet_pin, int e_throat_pin, float i_inlet_slope, float i_throat_slope, float e_inlet_slope, float e_throat_slope, float i_inlet_offset, float i_throat_offset, float e_inlet_offset, float e_throat_offset, float discharge_coefficient, float fluid_density, float throat_diameter, float outlet_diameter):
  pin{i_inlet_pin, i_throat_pin, e_inlet_pin, e_throat_pin}, slope{i_inlet_slope, i_throat_slope, e_inlet_slope, e_throat_slope}, offset{i_inlet_offset, i_throat_offset, e_inlet_offset, e_throat_offset}, Cd{discharge_coefficient}, density{fluid_density}
{
  pinMode(i_inlet_pin, INPUT);
  pinMode(i_throat_pin, INPUT);
  pinMode(e_inlet_pin, INPUT);
  pinMode(e_throat_pin, INPUT);
  throat_area = PI * throat_diameter * throat_diameter / 4;
  outlet_area = PI * outlet_diameter * outlet_diameter / 4;
}

void Sensors::read_sensors()
{
  pressure[I_INLET] = analogRead(pin[I_INLET]) * slope[I_INLET] + offset[I_INLET];
  pressure[I_THROAT] = analogRead(pin[I_THROAT]) * slope[I_THROAT] + offset[I_THROAT];
  pressure[E_INLET] = analogRead(pin[E_INLET]) * slope[E_INLET] + offset[E_INLET];
  pressure[E_THROAT] = analogRead(pin[E_THROAT]) * slope[E_THROAT] + offset[E_THROAT];
  i_flow_rate = Cd * sqrt((2 * (pressure[I_INLET] - pressure[I_THROAT]) * outlet_area * outlet_area)/(density * ((outlet_area * outlet_area)/(throat_area * throat_area)-1)));
  e_flow_rate = Cd * sqrt((2 * (pressure[E_INLET] - pressure[E_THROAT]) * outlet_area * outlet_area)/(density * ((outlet_area * outlet_area)/(throat_area * throat_area)-1)));
}
