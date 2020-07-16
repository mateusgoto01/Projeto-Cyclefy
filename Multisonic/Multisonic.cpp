#include "Multisonic.h"

Multisonic::Multisonic(int trigger1, int echo1, int trigger2, int echo2, int trigger3, int echo3, int trigger4, int echo4){
    //Define com o que cada pin esta associado
    pin1 = trigger1; 
    pin2 = echo1;
    pin3 = trigger2;
    pin4 = echo2;
    pin5 = trigger3;
    pin6 = echo3;
    pin7 = trigger4;
    pin8 = echo4;
      
    // Define todos os pin de Trigger como outputs
    pinMode(pin1, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin5, OUTPUT);
    pinMode(pin7, OUTPUT);
    // Define todos os pin de Echo como inputs
    pinMode(pin2, INPUT);
    pinMode(pin4, INPUT);
    pinMode(pin6, INPUT);
    pinMode(pin8, INPUT);

}

void Multisonic::MultisonicSetup() {
    //Setup faz com que todos os trigger comece com nivel baixo para evitar qualquer retorno de echo errado.
    digitalWrite(pin1, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin5, LOW);
    digitalWrite(pin7, LOW);

}

void Multisonic::AvaregeDistance() {
    // variavel float que armazenará o tempo de nivel alto de cada pulso dos echos
    float pulse1;
    float pulse2;
    float pulse3;
    float pulse4;
    // variavel float que armazenará as distancias encontradas por cada ultrassonico
    float dist1;
    float dist2;
    float dist3;
    float dist4;
    // variavel float que armazenará a média aritmética das distância
    float Avarege;
    // Faz a emissão de um trigger por 10us para começar a ver o tempo de resposta do echo e poder descobrir a distância
    digitalWrite(pin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin1, LOW);
    pulse1 = pulseIn(pin2, HIGH); // capita o tempo de nivel alto do echo
    dist1 = pulse1/58.82; // Formula de velocidade média do som para achar a distância
    // Repete o processo acima para o segundo sensor
    digitalWrite(pin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin3, LOW);
    pulse2 = pulseIn(pin4, HIGH);
    dist2 = pulse2/58.82;
    // Repete o processo acima para o terceiro sensor
    digitalWrite(pin5, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin5, LOW);
    pulse3 = pulseIn(pin6, HIGH);
    dist3 = pulse3/58.82;
    // Repete o processo acima para o quarto sensor
    digitalWrite(pin7, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin7, LOW);
    pulse4 = pulseIn(pin8, HIGH);
    dist4 = pulse4/58.82;

    Avarege = (dist1 + dist2 + dist3 + dist4)/4; // realiza a média aritimética das distâncias

    Serial.println(Avarege); // Mostra no serial monitor a média aritimética das distâncias
    
}