#include "SerialCom.h"
//#include <SoftwareSerial.h>

SerialComm::SerialComm(void)
{

}

// Initialize Serial connection. Needs to be called before anything works
void SerialComm::InitSerial(long baud)
{
    RpiSerial.begin(baud);
}

// Read incoming data. Only reads one byte at a time to not block toe contol loop.
void SerialComm::ReadRpiSerial()
{
    if(RpiSerial.available())
    {
        byte currentChar = RpiSerial.read();
        switch(state){
        case expectingStartByte:
            if(currentChar == START_BYTE)
                state = expectingLengthByte1;
            break;

        case expectingLengthByte1:
            messageLengthByte1 = currentChar;
            state = expectingLengthByte2;
            break;
        case expectingLengthByte2:
            messageLengthByte2 = currentChar;
            messageLength = messageLengthByte1 * 256 + messageLengthByte2;
            state = expectingMessage;
            break;
        case expectingMessage:
            if (bufferindex < messageLength)
            {
                buffer_in[bufferindex++] = currentChar;
            }
            else if (bufferindex >= messageLength)
            {
                char message[messageLength-2];
                for(int i = 0; i < messageLength-2; ++i) {
                    message[i] = buffer_in[i];
                }
                if (getCRC16(String(message), messageLength) == 0)
                {
                  // TODO: message was received successfully. Parse through it and update values. Send confirmation
                }
                messageLength = 0;
                messageLengthByte1 = 0;
                messageLengthByte2 = 0;
                bufferindex = 0;
                messageLength = 0;
                state = expectingStartByte;
            }
            break;
        }
    }
}

// Not finalized yes. Julien's template
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

// Not finalized yes. Julien's template
void SerialComm::blinkLed()
{
    static bool state = 1;
    state = !state;
    digitalWrite(13, state);
}

// Not finalized yes. Julien's template
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

// Not finalized yes. Julien's template
float SerialComm::intergercomma(int commapos){
    if(commapos == 1 ) return 1.0;
    if(commapos == 2 ) return 10.0;
    if(commapos == 3 ) return 100.0;
    if(commapos == 4 ) return 1000.0;
}

void SerialComm::SendBufferToRpi()
{
    if (buffer_out != "")
    {
        int msgLen = buffer_out.length() + 2;
        RpiSerial.write(START_BYTE);
        RpiSerial.write(msgLen >> 8);
        RpiSerial.write(msgLen & 0xFF);
        RpiSerial.print(this->buffer_out);
        //RpiSerial.write(getCRC8(buffer_out, buffer_out.length()));
        uint16_t crc = getCRC16(buffer_out, buffer_out.length());
        RpiSerial.write(crc & 0xFF);
        RpiSerial.write(crc >> 8);

    }
}

// from http://www.ccontrolsys.com/w/How_to_Compute_the_Modbus_RTU_Message_CRC
uint16_t SerialComm::getCRC16(String data, int len )
{
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)data[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                            // Else LSB is not set
                crc >>= 1;                    // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}
