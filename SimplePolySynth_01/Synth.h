#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>
#include "Voice.h"
#include "mixers.h"
 
// SimplePolySynth: begin automatically generated code

class Synth 
{
public:
    const static int VOICE_COUNT = 6;
    const float DIV127 = (1.0 / 127.0);
    const float DIV100 = 0.01;
    const float DIV64 = (1.0/64.0);
    const float DIV360BY127 = (360.0/127.0);
    const float DIV360BY120 = (3.0);
    
    Voice                            voices[8];
    AudioMixer8                      mixVoices;
    //AudioOutputPT8211_2              pt8211_2;
    AudioOutputI2S                   i2s;
    AudioControlSGTL5000 sgtl5000;
    AudioConnection                  *patchCord[12]; // total patchCordCount:12 including array typed ones.

    Synth() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(mixVoices, 0, i2s, 0);
        patchCord[pci++] = new AudioConnection(mixVoices, 0, i2s, 1);
        //patchCord[pci++] = new AudioConnection(mixVoices, 0, pt8211_2, 0);
        //patchCord[pci++] = new AudioConnection(mixVoices, 0, pt8211_2, 1);
        for (int i = 0; i < 8; i++) {
            patchCord[pci++] = new AudioConnection(voices[i].env, 0, mixVoices, i);
        }
        
    }

    // used to simplify initialization from the "parent" class
    void begin()
    {
        sgtl5000.enable();
        sgtl5000.volume(0.9);
        sgtl5000.lineOutLevel(13);
        for (int i = 0; i < VOICE_COUNT; i++)
        {
            voices[i].begin();
            mixVoices.gain(i, 0.8f/VOICE_COUNT); // set max to 0.8 to avoid clippin
        }
    }
    
    void noteOn(byte note, byte velocity)
    {
        //digitalWrite(NOTE_PRESSED_STATE_LED, HIGH); 
        //any note "pressed"
        // fist checks if this note is allready playing
        // it that is the case then it "reuses" this "slot"
        // this makes sure that not all "slots" is filled
        // with the same playing note
        // if the MIDI keyboard is for some reason
        // not sending a noteoff (my keyboard is sometimes glitching)
        // and when sending MIDI from my computer for fast playing songs
        for (int i = 0; i < VOICE_COUNT; i++) 
        {
            // first check if the note was played recently
            if (voices[i].note == note) 
            {
                voices[i].noteOn(note, velocity);
                //digitalWrite(NOTE_OVERFLOWN_LED, LOW);
                return; 
            }
        }
        // then if the note has not allready been played
        // // second see if there is any free "spot"
        for (int i = 0; i < VOICE_COUNT; i++) 
        {
            if (voices[i].isNotPlaying())
            {
                voices[i].noteOn(note, velocity);
                //digitalWrite(NOTE_OVERFLOWN_LED, LOW); // clear overflown notification
                return;
            }
        }
        //digitalWrite(NOTE_OVERFLOWN_LED, HIGH); // this is a notification that there was no free spots
    }
    
    void noteOff(byte note)
    {
        //digitalWrite(NOTE_PRESSED_STATE_LED, LOW); //any note "released"
        for (int i = 0; i < VOICE_COUNT; i++)
        {
            if (voices[i].note == note)
            {
                voices[i].noteOff();
                return;
            }
        }
    }
    
    void activateSustain()
    {
        for (int i = 0; i < VOICE_COUNT; i++)
        {
            voices[i].isSustain = 1;
        }
    }
    
    void deactivateSustain()
    {
        for (int i = 0; i < VOICE_COUNT; i++)
        {
            voices[i].isSustain = 0;
            if (!voices[i].isNoteOn)
                voices[i].noteOff();
        }
    }
    
};
// SimplePolySynth: end automatically generated code
