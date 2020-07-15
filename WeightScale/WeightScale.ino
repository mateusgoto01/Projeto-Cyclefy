#include <HX711.h> //inclui a biblioteca do HX711

//Definindo os pinos para cada modulo HX711
#define HX711_SCK1 8
#define HX711_DT1 9
#define HX711_SCK2 10  
#define HX711_DT2 11
//Definindo como se estivesse duas balanças
HX711 scale1;
HX711 scale2; 

#define CBT1 2918367.0f // coloque aqui o valor encontrado / pelo peso do objeto que usou(em KG)
#define CBT2 // coloque aqui o valor encontrado / pelo peso do objeto que usou(em KG)
//variaveis que irão armazenar os valores das celulas de carga
float KG1 = 0; 
float KG2 = 0;
float total = 0;

void setup() {
    Serial.begin(9600);
    scale1.begin(HX711_DT1, HX711_SCK1); // configurando os pin para as duas balanças
    scale2.begin(HX711_DT2, HX711_SCK2);

    scale1.set_scale(CBT1); // seta a calibragem para o primeiro HX711
    scale2.set_scale(CBT2); // seta a calibragem para o primeiro HX711

    scale1.tare(); // Desconsiderando a massa da estrutura
    scale2.tare();

    Serial.println("Tara feita!"); // A tara está no void setup, então ao iniciar o arduino já deixe encima a estrutura

}

void loop() {

    scale1.power_up(); // liga as celulas de carga
    scale2.power_up();

    KG1 = scale1.get_units(5); // faz uma média de 5 das pesagem achadas de cada HX711
    KG2 = scale2.get_units(5);

    total = KG1 + KG2; // soma os pesos das duas balanças

    Serial.print("Balança 1: "); // mostra os valores achados por cada balança
    Serial.print(KG1, 1);
    Serial.print("Balança 2: ");
    Serial.println(KG2,1);
    Serial.print("O peso total é: "); // mostra o peso total
    Serial.println(total, 1);

    scale1.power_down(); // desliga as celulas de carga
    scale2.power_down();

    delay(1000);

}
