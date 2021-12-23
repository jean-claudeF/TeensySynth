/*
 * 06_MIDI_2_OSC_ADSR
 * with key buffer   (idea: www.notesandvolts.com)
 * Serial.print to test key buffer that seems to hang sometimes
 * TEST WITH notes&volts buffer
 * added MIDI panic button
 */
#include <MIDI.h>
#include <Audio.h>
#include "frequencies.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=386.16668701171875,344
AudioSynthWaveform       waveform2;      //xy=388.16668701171875,390.16668701171875
AudioMixer4              mixer1;         //xy=582.1666717529297,364.1666717529297
AudioEffectEnvelope      envelope1;      //xy=745.1666717529297,365.1666717529297
AudioOutputI2S           i2s1;           //xy=901.1666870117188,373
AudioConnection          patchCord1(waveform1, 0, mixer1, 0);
AudioConnection          patchCord2(waveform2, 0, mixer1, 1);
AudioConnection          patchCord3(mixer1, envelope1);
AudioConnection          patchCord4(envelope1, 0, i2s1, 0);
AudioConnection          patchCord5(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=594.1666870117188,450.9999694824219
// GUItool: end automatically generated code


//--------------------------------------



const float DIV127 = (1.0 / 127.0);

const byte MIDI_panic = 2;

void setup() {
    pinMode(MIDI_panic, INPUT_PULLUP);
  
    Serial.begin(115200);
    
    AudioMemory(10);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.6);
    waveform1.begin(WAVEFORM_SINE);
    waveform2.begin(WAVEFORM_SINE);
    mixer1.gain(0, 0.5);
    mixer1.gain(1, 0.5);  
    envelope1.attack(10);
    envelope1.decay(10);
    envelope1.sustain(200);
    envelope1.release(500);
    envelope1.releaseNoteOn(5);        //should reduce clicks https://forum.pjrc.com/archive/index.php/t-61599.html
    delay(100);
    clearMIDIinputBuffer();
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOff(note_off);
    MIDI.setHandleNoteOn(note_on);
    
    Serial.println("READY!");
}
//----------------------------------------------------------------------------
// keyboard buffer:
const byte BUFFER = 8; 
byte buff[BUFFER];
byte buffSize = 0;
byte globalNote = 0;
byte globalVelocity = 0;

void clear_keyBuff(){
  
   for ( int i = 0; i < BUFFER; ++i ) {
        buff[i] = 0; 
   }
   buffSize = 0;
   //oscStop();
}


void keyBuff(byte note,  bool playNote) {
  // playNote: True -> play note and add it to buffer
  //           False -> stop note and remove it from buffer           
  
    
  // Add Note to buffer and play it:
  if (playNote == true && (buffSize < BUFFER) ) {
      oscPlay(note);
      buff[buffSize] = note;
      buffSize++;
      Serial.print( " P");
      Serial.print(note);
      return;
  }

  // Remove Note
  else if (playNote == false && buffSize != 0) {
      
      // search through the buffer for note:
      for (byte found = 0; found < buffSize; found++) {
          
          // if the note is found:
          if (buff[found] == note) {
                
                // close the gap and reduce the buffer size:
                for (byte gap = found; gap < (buffSize - 1); gap++) {
                    buff[gap] = buff[gap + 1];
                }
                buffSize--;
            
                buff[buffSize] = 255;         // why this ??

                // play the last note in the buffer, if it is not empty:
                if (buffSize != 0) {
                      oscPlay(buff[buffSize - 1]);
                      Serial.print( " p");
                      Serial.print(buff[buffSize - 1]);
                      return;
                }
                
          else {
                oscStop();
                
                Serial.print(" S B:");
                Serial.println(buffSize);
                return;
            }
          }
      }
  }
  
}


//-----------------------------------------------
void loop(){
     MIDI.read();
     if (digitalRead(MIDI_panic) == 0) {
          MIDIpanic();
     }
}
//----------------------------------------------

//-----------------------------------------------------
void clearMIDIinputBuffer() {
  // clear MIDI input buffer
  uint32_t m = micros();
  do {
    if (Serial1.read() >= 0) { 
        m = micros();
        }
  } while (micros() - m < 10000);
}
//----------------------------------------
void note_on(byte channel, byte note, byte velocity){
     if ( note > 23 && note < 127 ) {
          Serial.print(" +");
          Serial.print(note);
          globalNote = note;
          globalVelocity = velocity;
          keyBuff(note,  true);
          
     }
     else { Serial.println("*"); }
}
//---------------------------------------------

void note_off(byte channel, byte note, byte velocity){
      if ( note > 23 && note < 127 ) {
            Serial.print(" -");
            Serial.print(note);
            keyBuff(note, false);
            
      }
      else { Serial.println("_"); }
}
//---------------------------------------------


void MIDIpanic(){
    Serial.println("STOP");
    delay(20);
    oscStop();

    Serial.println("BUFFER:");
    for ( int i = 0; i <= buffSize; ++i ) {
        Serial.print(buff[i]);
        Serial.print("    "); 
    }
    Serial.print("Buffer size: ");
    Serial.println(buffSize);
    buffSize = 0;
   delay(50);
}


//--------------------------------------------------------
void oscPlay(byte note) {
    waveform1.frequency(noteFreqs[note]);
    waveform2.frequency(noteFreqs[note] *2 + 5);
    float velo = (globalVelocity * DIV127);
    waveform1.amplitude(velo);
    waveform2.amplitude(velo);
    envelope1.noteOn();
}
//----------------------------------------------------------
void oscStop() {
    envelope1.noteOff();
}
