/*

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>

#include "frequencies.h"

 
// SimplePolySynth: begin automatically generated code

class Voice 
{
public:
 
    byte note = 0; // to store the current or previous note played
    byte isNoteOn = 0; // used by the sustain function
    byte isSustain = 0;
    
    AudioSynthWaveform               wf;
    AudioEffectEnvelope              env;
    AudioConnection                  *patchCord[1]; // total patchCordCount:1 including array typed ones.

    Voice() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(wf, 0, env, 0);
        
    }

    //---------------------------------------------
    void begin()  {
        wf.begin(1.0, 110, WAVEFORM_SINE);
        //wf.begin(1.0, 110, WAVEFORM_BANDLIMIT_SAWTOOTH);
        env.attack(0);
        env.decay(0);
        env.sustain(1);
        env.release(500);
        env.releaseNoteOn(5); 
    }
    //---------------------------------------------     
    void noteOn(byte Note, byte velocity) {
        float newAmp = 0.0f;
        if (Note >= sizeof(noteFreqs)) return;
        
        note = Note;
        isNoteOn = 1;
        
        newAmp = (float)velocity*(1.0f / 127.0f);
        wf.frequency(noteFreqs[note]);
        wf.amplitude(newAmp);
        env.noteOn();
    }
    //---------------------------------------------
    void noteOff()  {
        isNoteOn = 0;
        if (!isSustain)
            env.noteOff();
    }
    //---------------------------------------------
    bool isNotPlaying()  {
        if (!env.isActive())
            return true;
        else
            return false;
    }
    
    
};
// SimplePolySynth: end automatically generated code
*/
