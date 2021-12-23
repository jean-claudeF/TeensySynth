/*
 * MIDI receive example for Teensy 3.2
 * Rx = Pin 9
 * Information about received MIDI is output to Serial over USB (use Serial monitor to see it)
 */
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(57600);
  Serial.println("MIDI Input Test");
}

unsigned long t=0;

void loop() {
  int type, note, velocity, channel, d1, d2;
      if (MIDI.read()) {                    // Is there a MIDI message incoming ?
          
          byte type = MIDI.getType();
          
          switch (type) {
            case midi::NoteOn:
                  note = MIDI.getData1();
                  velocity = MIDI.getData2();
                  channel = MIDI.getChannel();
                  if (velocity > 0) {
                    Serial.println(String("Note On:  ch=")  + channel + ", note=" + note + ", velocity=" + velocity);
                  } else {
                    Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
                  }
                  break;
            case midi::NoteOff:
                  note = MIDI.getData1();
                  velocity = MIDI.getData2();
                  channel = MIDI.getChannel();
                  Serial.println(String("Note Off: ch=")  + channel + ", note=" + note + ", velocity=" + velocity);
                  break;
            default:
                  d1 = MIDI.getData1();
                  d2 = MIDI.getData2();
                  Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
          }
          t = millis();
    }
    if (millis() - t > 10000) {
        t += 10000;
        Serial.println("(inactivity)");
    }
}
