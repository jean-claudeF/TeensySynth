#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>
#include "Synth.h"
#include <MIDI.h>
 
// SimplePolySynth: begin automatically generated code
const int ledPin = 33;
Synth synth;


void uartMidi_NoteOn(byte channel, byte note, byte velocity);
void uartMidi_NoteOff(byte channel, byte note, byte velocity);
void uartMidi_ControlChange(byte channel, byte control, byte value);
void uartMidi_PitchBend(byte channel, int value);

void usbMidi_NoteOn(byte channel, byte note, byte velocity);
void usbMidi_NoteOff(byte channel, byte note, byte velocity);
void usbMidi_ControlChange(byte channel, byte control, byte value);
void usbMidi_PitchBend(byte channel, int value);

void blinkLedTask(void);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);



//---------------------------------------
//Potentiometers
word muxvalues[8];     //0...1023
#include "mux4051.h"
Mux4051 mymux;

//---------------------------------------------------------------
// Arduino setup() function
void setup()
{
   mymux.begin(33, 34, 35, A17);
  
  AudioMemory(96);

  MIDI.begin();
  MIDI.setHandleNoteOn(uartMidi_NoteOn);
  MIDI.setHandleNoteOff(uartMidi_NoteOff);
  MIDI.setHandleControlChange(uartMidi_ControlChange);
  MIDI.setHandlePitchBend(uartMidi_PitchBend);
  clearMIDIinputBuffer();
  /*
  usbMIDI.setHandleNoteOn(usbMidi_NoteOn);
  usbMIDI.setHandleNoteOff(usbMidi_NoteOff);
  usbMIDI.setHandleControlChange(usbMidi_ControlChange);
  usbMIDI.setHandlePitchChange(usbMidi_PitchBend);
  */
  
  synth.begin();
  Serial.begin(115200);
  Serial.println("READY");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

//---------------------------------------------------------------
//****************************************
void loop()
{
    //usbMIDI.read();
    
    MIDI.read();
    
    mymux.readAllChannels(muxvalues, 8);
    /*
    for(byte i=0; i<8; i++){
          Serial.print(muxvalues[i]);
          Serial.print('\t');
          
      }
      Serial.println();
      delay(100);
    */
    float f_off = muxvalues[0];
    synth.setNoteOffset(f_off/50);
    //Serial.println(f_off);
    blinkLedTask();
    
}
//*****************************************
//---------------------------------------------------------------
void clearMIDIinputBuffer() {
  // clear MIDI input buffer
  uint32_t m = micros();
  do {
  if (Serial1.read() >= 0) { 
  m = micros();
  }
  } while (micros() - m < 10000);
}

//---------------------------------------------------------------
void debug_note(byte channel, byte note, byte velocity){
    Serial.print(note);
    Serial.print("v");
    Serial.print(velocity);
    Serial.print("c");
    Serial.println(channel);
}

// uartMidi handler functions
void uartMidi_NoteOn(byte channel, byte note, byte velocity) {
    //velocity = 127 - velocity; should not be needed on a normal midi keyboard
    synth.noteOn(note, velocity);
    Serial.print("+");
    debug_note(channel, note, velocity);
    //usbMIDI.sendNoteOn(note, velocity, channel, 0);
}
//---------------------------------------------------------------
void uartMidi_NoteOff(byte channel, byte note, byte velocity) {
    //velocity = 127 - velocity; should not be needed on a normal midi keyboard
    synth.noteOff(note);
    Serial.print("-");
    debug_note(channel, note, velocity);
    //usbMIDI.sendNoteOff(note, velocity, channel, 0);
}
//---------------------------------------------------------------
void uartMidi_ControlChange(byte channel, byte control, byte value) {
    //usbMIDI.sendControlChange(control, value, channel, 0x00);
}
//---------------------------------------------------------------
void uartMidi_PitchBend(byte channel, int value) {
    //usbMIDI.sendPitchBend(value, channel, 0x00);
}
//---------------------------------------------------------------
// usbMidi handler functions
void usbMidi_NoteOn(byte channel, byte note, byte velocity) {
    synth.noteOn(note, velocity);
}
//---------------------------------------------------------------
void usbMidi_NoteOff(byte channel, byte note, byte velocity) {
    synth.noteOff(note);    
}
//---------------------------------------------------------------
void usbMidi_PitchBend(byte channel, int value) {
  
}
//---------------------------------------------------------------
void usbMidi_ControlChange(byte channel, byte control, byte value) {
    if (control == 64) {
        if (value == 0)
            synth.deactivateSustain();
        else if (value == 127)
            synth.activateSustain();
    }
}
//---------------------------------------------------------------
// blinkLedTask() function
void blinkLedTask(void)
{
    // set the following to static so that they
    // be kept here inside
    static int ledState = LOW;             // ledState used to set the LED
    static unsigned long previousMillis = 0;        // will store last time LED was updated
    static unsigned long currentMillis = 0;
    static unsigned long currentInterval = 0;
    static unsigned long ledBlinkOnInterval = 100;
    static unsigned long ledBlinkOffInterval = 2000;
    currentMillis = millis();
    currentInterval = currentMillis - previousMillis;
    
    if (ledState == LOW)
    {
        if (currentInterval > ledBlinkOffInterval)
        {
            previousMillis = currentMillis;
            ledState = HIGH;
            digitalWrite(ledPin, HIGH);
        }
    }
    else
    {
        if (currentInterval > ledBlinkOnInterval)
        {
            previousMillis = currentMillis;
            ledState = LOW;
            digitalWrite(ledPin, LOW);
        }
    }
}
// SimplePolySynth: end automatically generated code
