#include <HX711_ADC.h>
#include <NewPing.h>
#include <LoRa.h>

// pins para os HX711
const int HX711_sck_1 = 3;
const int HX711_dout_1 = 4;
const int HX711_sck_2 = 5;
const int HX711_dout_2 = 6;
const int HX711_sck_3 = 7;
const int HX711_dout_3 = 8;
const int HX711_sck_4 = 9;
const int HX711_dout_4 = 10;

HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); //HX711 3
HX711_ADC LoadCell_4(HX711_dout_4, HX711_sck_4); //HX711 4
// fim dos pins relacionados ao sensorialmente to peso

// Setup para os ultrassônicos
#define PING_PIN1 A0 // pino de trigger e echo para o primeiro ultrassônico
#define PING_PIN2 A2 // pino de trigger e echo para o segundo ultrassônico
#define PING_PIN3 A3 // pino de trigger e echo para o terceiro ultrassônico
#define MAX_DISTANCE 400 // define a distância maxima do sensor em cm

NewPing sonar1(PING_PIN1, PING_PIN1, MAX_DISTANCE); //define o pino de echo, trigger e a distância maxima de cada ultrassônico
NewPing sonar2(PING_PIN1, PING_PIN1, MAX_DISTANCE);
NewPing sonar3(PING_PIN1, PING_PIN1, MAX_DISTANCE);
// Fim do setup dos ultrassônicos
// Setup do modulo LoRA
const int NSS = A5; // define o pin do Chip Select
const int Reset = A6; // define o pin do reset
const int DIO = A1; // define o pin DIO1

byte LocalAddress = 0xBB; // endereço do dispositivo LoRa, por enquanto um genérico
byte Destination = 0xFF; // endereço do dispositivo que irá receber as informações 0xFF envia para todos ao seu alcance
// Fim do setup do módulo LoRA
void setup() {
    Serial.begin(96000);
    Serial.println("Iniciando...");
    // Setup do sensor de peso
    float calibrationValue_1; // variavel float para armazenar o valor de calibração do HX711 1
    float calibrationValue_2; // variavel float para armazenar o valor de calibração do HX711 2
    float calibrationValue_3; // variavel float para armazenar o valor de calibração do HX711 3
    float calibrationValue_4; // variavel float para armazenar o valor de calibração do HX711 4

    calibrationValue_1 = 0.0; //Valores de calibragem para o HX711 1
    calibrationValue_2 = 0.0; //Valores de calibragem para o HX711 2
    calibrationValue_3 = 0.0; //Valores de calibragem para o HX711 3
    calibrationValue_4 = 0.0; //Valores de calibragem para o HX711 4

    LoadCell_1.begin(); // inicia todos os HX711
    LoadCell_2.begin();
    LoadCell_3.begin();
    LoadCell_4.begin();

    long stabilizingtime = 2000;
    boolean _tare = true;

    byte loadcell_1_rdy = 0;
    byte loadcell_2_rdy = 0;
    byte loadcell_3_rdy = 0;
    byte loadcell_4_rdy = 0;

    while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy + loadcell_4_rdy) < 4) { // inicializa todos os HX711 simultaneamente
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare); //inicializa com a função preparada para multiplas celulas de carga
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
    if (!loadcell_4_rdy) loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);
    Serial.println("Celulas de cargas iniciadas");

    LoadCell_1.setCalFactor(calibrationValue_1); // usa o valor de calibração para calibrar cada HX711
    LoadCell_2.setCalFactor(calibrationValue_2);
    LoadCell_3.setCalFactor(calibrationValue_3);
    LoadCell_4.setCalFactor(calibrationValue_4);
    // Setup do sensor de peso realizado
    // Setup do LoRA
    LoRa.setPins(NSS, Reset, DIO); // Declaro onde os pinos estão localizados
    LoRa.begin(868E6); // Começo o LoRa e defino sua frequência

  }


}
void loop(){
    // Coleta os datados do sensores de peso
    float x = LoadCell_1.getData();
    float y = LoadCell_2.getData();
    float z = LoadCell_3.getData();
    float w = LoadCell_4.getData();

    float Weight = x + y + z + w - 100; //realiza a soma dos pesos e desconsidera 100kg que é o peso aproximado da lixeira
    Serial.print("O peso é: "); Serial.println(Weight);
    // Coleta de dados dos sensores de distância

    float a = sonar1.ping_cm();
    float b = sonar2.ping_cm();
    float c = sonar3.ping_cm();

    float Distance = (a + b + c)/3; // pega os valores de distancias observados e realiza a média
    Serial.print("A distância é: "); Serial.println(Distance);
    float Height = Distance - 2.15; // troca o referêncial o valor 2,15 é a altura do teto até essa lixeira
    Serial.print("A altura é: "); Serial.println(Height);
    // chama a função que irá enviar esses dados
      LoRa.beginPacket();                   // Inicia o pacote da mensagem
      LoRa.write(Destination);              // Adiciona o endereco de destino
      LoRa.write(LocalAddress);             // Adiciona o endereco do remetente
      LoRa.print(Weight);                   // Vetor da mensagem do valor do peso encontrado
      LoRa.print(Height);                   // Vetor da mensagem do valor da altura
      LoRa.endPacket();                     // Finaliza o pacote e envia
      Serial.println("Mensagem enviada");
        delay(5000); // delay de 5s


}