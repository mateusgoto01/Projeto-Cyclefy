#include "Arduino.h"

class Multisonic{
    private:
        int pin1;
        int pin2;
        int pin3;
        int pin4;
        int pin5;
        int pin6;
        int pin7;
        int pin8;

    public:
        Multisonic(int trigger1, int echo1, int trigger2, int echo2, int trigger3, int echo3, int trigger4, int echo4);

        void MultisonicSetup();
        void AvaregeDistance();
            
};
