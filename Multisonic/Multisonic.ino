#include "Multisonic.h"

Multisonic ultrasonics(10,11,12,13,14,15,16,17); // Defino os pinos dos 4 sensores ultrassonicos


void setup() {
    Serial.begin(9600); //Começa a comunicação serial 
    ultrasonics.MultisonicSetup(); // Realiza o Setup dos ultrassonicos
}

void loop() {
    ultrasonics.AvaregeDistance(); // Função que realiza a captura de distancia de cada sensor e faz sua média
                                   // e printa no serial 
    delay(500); // tempo de 500ms para a proxima capitação de distância média dos sensores
}
