/*  muxtest.ino
 *  Read 8 potentiometers through a 4051 multiplexer
 */

word values[8];     //0...1023

//---------------------------------------
#include "mux4051.h"
//---------------------------------------

Mux4051 mymux;
 
void setup() 
{
       
       mymux.begin(33, 34, 35, A17);
       Serial.begin(115200);
       Serial.println("HELLO");
  }

//--------------------------------------------------- 
void loop() 
{
 
      word v = mymux.readChannel(1);
      //Serial.println(v);

      mymux.readAllChannels(values, 8);
      for(byte i=0; i<8; i++){
          Serial.print(values[i]);
          Serial.print('\t');
          
      }
      Serial.println();
      delay(100);
 }
