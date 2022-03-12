//mux4051.h

class Mux4051 {
public:
    word values[8];
    int A0pin;
    int A1pin;
    int A2pin;
    int outpin;
    //---------------------------------------------------
    void begin(int addr0, int addr1, int addr2, int out){
          A0pin = addr0;
          A1pin = addr1;
          A2pin = addr2;
          outpin = out;
          pinMode(A0pin, OUTPUT);
          pinMode(A1pin, OUTPUT);
          pinMode(A2pin, OUTPUT);
          
    }
    //----------------------------------------------------
    void setChannel( byte ch ){
        byte a0 = ch & 1;
        byte a1 = (ch & 2) >> 1;
        byte a2 = (ch & 4) >>2;
        digitalWrite(A0pin, a0);
        digitalWrite(A1pin, a1);
        digitalWrite(A2pin, a2); 
        delayMicroseconds(20);
    }
    //----------------------------------------------------
    word readChannel(byte ch){
        setChannel(ch);
        word value = analogRead(outpin);
        return value;
    }
    //----------------------------------------------------
    void readAllChannels(word values[], int nbvalues){
        for (byte i=0; i<nbvalues; i++){
            setChannel(i);
            values[i] = readChannel(i);
            //Serial.println(values[i]);
        }
    }
};
