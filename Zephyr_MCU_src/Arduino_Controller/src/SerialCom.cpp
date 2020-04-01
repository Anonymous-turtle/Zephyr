/**
 *  example buffer message:
 *  "Vmode"
 *  "VAR H30 L5 BR12 TV400"
 * 
 */
 
#include "Arduino.h"
#include "SerialCom.h"
//#include <SoftwareSerial.h>

// define the serial port used for gcode input
#define RpiSerial Serial


SerialComm::SerialComm(void)
{
  initSerial();
  packet_len=0;
}

void SerialComm::initSerial()
{
  RpiSerial.begin(115200);
  //RpiSerial.println("We Good!\n");
}

void SerialComm::readRpiSerial()
{
    int echo = 0;
    static int bufferindex=0;
    if(RpiSerial.available()) 
    {
      //delay(20); // wait for buffer to fill
      buffer_in[bufferindex++]=RpiSerial.read();
      if(bufferindex==BUFFER_SIZE)
      {
        bufferindex=0;
      }
      if(bufferindex>1 && buffer_in[bufferindex-2]==13 && buffer_in[bufferindex-1]==10) //check end char
      {
        buffer_in[bufferindex]=0;
        buffer_in[bufferindex-1]=0;
        buffer_in[bufferindex-2]=0;
        packet_len = bufferindex;
        bufferindex=0;
        if(echo)
        {
          RpiSerial.print("buffer is: \"");
          RpiSerial.print(buffer_in);
          RpiSerial.print("\" len: ");
          RpiSerial.println(packet_len);
        }
  
        buffer_received = 1;

      }
    }
}

void SerialComm::processBuffer()
{
  bool echo_confirm = 0;

  // reset position values for new reading
  int pressure_set_maximum_pos = 0;
  int pressure_set_minimun_pos = 0;
  int volume_set_pos = 0;
  int flow_set_pos = 0;
  int breath_rate_set_pos = 0;

  if( buffer_received )
  {
      //RpiSerial.print("Reading ");
      //RpiSerial.print(packet_len);
      //RpiSerial.print(" bytes from: ");
      //RpiSerial.println(buffer_in);

      // parse buffer
      for( int i = 0; i < packet_len; i++ )
      {
        // read mode set
        if( buffer_in[i] == 'P' && strncmp(buffer_in+i, "Pmode", 5) == 0) pressure_control = 1;
        else if( buffer_in[i] == 'V' && strncmp(buffer_in+i, "Vmode", 5) == 0) volume_control = 1;
        else if( buffer_in[i] == 'V' && strncmp(buffer_in+i, "Vtrig", 5) == 0) patient_triggered = 1;
        else if( buffer_in[i] == 'T' && strncmp(buffer_in+i, "Ttrig", 5) == 0) timed_triggered = 1;
        else if( buffer_in[i] == 'D' && strncmp(buffer_in+i, "Dtrig", 5) == 0) dual_triggered = 1;
        else if( buffer_in[i] == 'V' && strncmp(buffer_in+i, "VAR", 3) == 0) variable_set = 1;

        // record position of settings in buffer
        else if( buffer_in[i] == 'L' ) pressure_set_minimun_pos = i+1;
        else if( buffer_in[i] == 'H' ) pressure_set_maximum_pos = i+1;
        else if( buffer_in[i] == 'T' && strncmp(buffer_in+i, "TV", 2)) volume_set_pos = i+2;
        else if( buffer_in[i] == 'F' ) flow_set_pos = i+1;
        else if( buffer_in[i] == 'B' && strncmp(buffer_in+i, "BR", 2)) breath_rate_set_pos = i+2;
      }

      /** 
        if(echo_confirm)
        {
        RpiSerial.print("Set pid kp ");
        RpiSerial.print(kp);
        }
       */

       //check state and act according to state, reset in main after state change
       //Change getfloatatlocation to getintatlocation if no work

      if( variable_set )
      {
        if(pressure_set_maximum_pos != 0) pressure_set_maximum = getfloatatlocation(buffer_in,pressure_set_maximum_pos);
        if(pressure_set_minimun_pos != 0) pressure_set_minimun = getfloatatlocation(buffer_in,pressure_set_minimun_pos);
        if(volume_set_pos != 0) volume_set = getfloatatlocation(buffer_in,volume_set_pos);
        if(flow_set_pos != 0) flow_set = getfloatatlocation(buffer_in,flow_set_pos);
        if(breath_rate_set_pos != 0) breath_rate_set = getfloatatlocation(buffer_in,breath_rate_set_pos);
      }

      if( pressure_control )
      {

      }
    
      if( volume_control )
      {

      }

      if (patient_triggered)
      {

      }

      if (timed_triggered)
      {
   
      }

      if (timed_triggered)
      {
   
      }

      
      //reset bufferin count
      buffer_received = 0;
  }
}

void SerialComm::blinkLed()
{
  static bool state = 1;
  state = !state;
  digitalWrite(13, state);
}

float SerialComm::getfloatatlocation(String bufferstring, int pos) 
{
    int index = pos;
    int commapos=0;
    String tempstr;
    int isnegative = 0;
    
    while( isDigit(bufferstring[index]) || bufferstring[index] == '.' || bufferstring[index] == '-')
    {
        if(bufferstring[index] != '.' && bufferstring[index] != '-')
        {  
          tempstr += bufferstring[index];
        }
        if(bufferstring[index] == '-')
        {
          isnegative = 1;
        }
        if(bufferstring[index] == '.')
        {
        commapos = index-pos;
        }
        index++;
    } 
    
    float val = (float)(tempstr.toInt());
    if ( commapos != 0 ) 
    {
      int commaoffset;
      if(isnegative) commaoffset = tempstr.length()-commapos+1+1;
      else commaoffset = tempstr.length()-commapos+1;
      val = val / intergercomma(commaoffset);
    }

    if( isnegative ) val = -val;

    //RpiSerial.print("val: ");
    //RpiSerial.println(val);
    //RpiSerial.println(bufferstring);
    
    return val;
}

float SerialComm::intergercomma(int commapos){
      if(commapos == 1 ) return 1.0;
      if(commapos == 2 ) return 10.0;
      if(commapos == 3 ) return 100.0;
      if(commapos == 4 ) return 1000.0;
  }

void SerialComm::SendBufferToRpi()
{
  if ( buffer_out != "")
  {
  buffer_out += '\n';
  RpiSerial.println(this->buffer_out);
  //this->buffer_out = ""; 
  }


}
