#include "comIOarduino.h"
#include <string.h>
#include "eiCom.h"
#include <Arduino.h>


comIOarduino::comIOarduino()
{
    comReadBufferLen = eiCom::READBUFFER_SIZE;
    comport = 0; //  use Serial, if want Serial1, Serial2, Serial3 if comport = 1,2,3
    baudrate = 9600;
    mode = SERIAL_8N1; // map to arduino

}
int comIOarduino::open()
{
    Serial.begin(9600);//baudrate);//, SERIAL_8N1 );
    return 0;
}

void comIOarduino::close()
{
  Serial.end();
}

int comIOarduino::read(unsigned char * buffer, int size)
{
  if(Serial.available() > 0 )
  {
    return Serial.readBytes( buffer, size);
  }
  return 0;
}


int comIOarduino::write(unsigned char * msgBuffer, int len)
{
//  if(Serial.available() > 0 )
  {
    return Serial.write(msgBuffer, len);
  }
}
void comIOarduino::sleep(int ms)
{

  delay(ms);
}
