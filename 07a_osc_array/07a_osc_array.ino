
#include <Audio.h>

AudioSynthWaveform    wave[4];
AudioOutputI2S        i2s1;
AudioMixer4           mixer1;
AudioConnection       patchCord1(mixer1, 0, i2s1, 0);
AudioConnection       patchCord2(mixer1, 0, i2s1, 1);
AudioControlSGTL5000  sgtl5000_1;

AudioConnection  *patchCord[10]; 

//--------------------------------------
void setup() {
    
    AudioMemory(10);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.9);

    for(int i=0; i<=3; i++){
        patchCord[i] = new AudioConnection(wave[i], 0, mixer1, i);
        wave[i].begin(WAVEFORM_SINE);  
        wave[i].frequency((i+1) * 440);
        wave[i].amplitude(0.3);
    }       
}
//----------------------------------------
void loop(){

}
