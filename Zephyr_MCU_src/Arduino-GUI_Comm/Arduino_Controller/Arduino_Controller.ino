// Include Class
#include "src/SerialCom.h"

// Create Object
SerialComm RPiComm;

void setup()
{
  // Initialise serial connection
  RPiComm.InitSerial(115200);
}

void loop()
{

  //RPiComm.ReadRpiSerial();
  // Set Message (csv values)
  //RPiComm.buffer_out = "Hello World!";
  // Send Message to GUI
  //RPiComm.SendBufferToRpi();
  //delay(1000);
}
