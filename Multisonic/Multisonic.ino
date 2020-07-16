#include "Multisonic.h"

Multisonic ultrasonics(10,11,12,13,14,15,16,17);


void setup() {
    Serial.begin(9600);
    ultrasonics.MultisonicSetup();
}

void loop() {
    ultrasonics.AvaregeDistance();
    delay(500);
}
