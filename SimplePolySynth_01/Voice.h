// Modified PolySynth from Manncicken
// with 2 oscillators in Voice
// (rest is unchanged, except separate tab for frequencies)

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "frequencies.h"
 
// SimplePolySynth: begin automatically generated code

class Voice 
{
public:
    byte note = 0; // to store the current or previous note played
    byte isNoteOn = 0; // used by the sustain function
    byte isSustain = 0;
    
    
    AudioSynthWaveform              wf1;
    AudioSynthWaveform              wf2;
    AudioMixer4                     mixer4;
    AudioEffectEnvelope             env;
    AudioConnection                  *patchCord[3]; // total patchCordCount:3 including array typed ones.

// constructor (this is called when class-object is created)
    Voice() { 
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(wf1, 0, mixer4, 0);
        patchCord[pci++] = new AudioConnection(wf2, 0, mixer4, 1);
        patchCord[pci++] = new AudioConnection(mixer4, 0, env, 0);
        
    }

    // used to simplify initialization from the "parent" class
    void begin()
    {
        wf1.begin(1.0, 110, WAVEFORM_SINE);
        wf2.begin(1.0, 110, WAVEFORM_SINE);
    }
    
    /*
     * this takes care of all the tasks that
     * needs to be taken care of when doing
     * a note on/off
     */
     
    void noteOn(byte Note, byte velocity)
    {
        float newAmp = 0.0f;
        if (Note >= sizeof(noteFreqs)) return;
        
        note = Note;
        isNoteOn = 1;
        
        newAmp = (float)velocity*(1.0f / 127.0f)/2;
          
        wf1.frequency(noteFreqs[note]);
        wf2.frequency(noteFreqs[note]+2);
        
        wf1.amplitude(newAmp);
        wf2.amplitude(newAmp);
        
        env.noteOn();
    }
    
    void noteOff()
    {
        isNoteOn = 0;
        if (!isSustain)
        {
            env.noteOff();
        }
    }
    
    bool isNotPlaying()
    {
        if (!env.isActive())
            return true;
        else
            return false;
    }
    
    
};
// SimplePolySynth: end automatically generated code
