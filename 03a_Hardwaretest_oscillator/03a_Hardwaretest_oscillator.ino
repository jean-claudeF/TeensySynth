// 

#include <Audio.h>


AudioSynthWaveform    waveform1;
AudioOutputI2S        i2s1;
AudioConnection       patchCord1(waveform1, 0, i2s1, 0);
AudioConnection       patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000  sgtl5000_1;

int count;

void setup() {
    AudioMemory(10);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.3);
    waveform1.begin(WAVEFORM_BANDLIMIT_SAWTOOTH);
    delay(1000);
  
}




void loop() {
    waveform1.frequency(440);
    waveform1.amplitude(0.9);
    delay(250);
    waveform1.amplitude(0);
    delay(750);
}
