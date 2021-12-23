/*
 * Example for MIDI output for Teensy 3.2
 * MIDI output is on pin 10
 * Sends notes that increases in pitch, in a loop
 */

#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);   
const int channel = 1;

void setup() {
  MIDI.begin();
}

void loop() {
  int note;
  for (note=10; note <= 127; note++) {
    MIDI.sendNoteOn(note, 100, channel);
    delay(200);
    MIDI.sendNoteOff(note, 100, channel);
  }
  delay(2000);
}
