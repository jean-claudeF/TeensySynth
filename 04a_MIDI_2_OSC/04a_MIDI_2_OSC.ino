/*
 * 04a_MIDI_2_OSC
 * MIDI controls oscillator for Teensy 3.2
 * Rx = Pin 9
 * Information about received MIDI is output to Serial over USB (use Serial monitor to see it)
 */
#include <MIDI.h>
#include <Audio.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

AudioSynthWaveform    waveform1;
AudioOutputI2S        i2s1;
AudioConnection       patchCord1(waveform1, 0, i2s1, 0);
AudioConnection       patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000  sgtl5000_1;

uint16_t frequency[128] PROGMEM = {8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 19, 21, 22, 23, 24, 26,
  28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131,
  139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554,
  587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 5920, 
  6645, 7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544};
//--------------------------------------
void setup() {
    
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOff(note_off);
    MIDI.setHandleNoteOn(note_on);
    AudioMemory(10);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.9);
    waveform1.begin(WAVEFORM_SINE);
    delay(1000);
}
//----------------------------------------

void note_on(byte channel, byte note, byte velocity){
     
      
      if (velocity > 0) {
            uint16_t f = frequency[note];
            waveform1.frequency(f);
            waveform1.amplitude(0.9);
      } else {
            // note off
            waveform1.amplitude(0);
      }

}


void note_off(byte channel, byte note, byte velocity){
      
      waveform1.amplitude(0);
}

//-----------------------------------------------
void loop(){
     MIDI.read();
}
