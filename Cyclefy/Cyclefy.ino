#include <HX711_ADC.h>
#include <NewPing.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h> // Não é usado realmente no codigo. Mas, é necessário para o LoRa funcionar

// pins para os HX711
const int HX711_sck_1 = 0;
const int HX711_dout_1 = 1;
const int HX711_sck_2 = 3;
const int HX711_dout_2 = 4;
const int HX711_sck_3 = 5;
const int HX711_dout_3 = 6;
const int HX711_sck_4 = 7;
const int HX711_dout_4 = 8;

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
// Setup do LoRa transmissor
#define SERVER_ADDRESS 1 // Endereço do servidor 
#define CLIENT_ADDRESS 2 // mudar conforme as lixeiras forem aumentando. Não use 1, esse é do servidor
RH_RF95 driver;
RHReliableDatagram manager(driver, CLIENT_ADDRESS);


void setup() {
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
    boolean _tare = false;

    byte loadcell_1_rdy = 0;
    byte loadcell_2_rdy = 0;
    byte loadcell_3_rdy = 0;
    byte loadcell_4_rdy = 0;

    while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy + loadcell_4_rdy) < 4) { // inicializa todos os HX711 simultaneamente
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare); //inicializa com a função preparada para multiplas celulas de carga
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
    if (!loadcell_4_rdy) loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);

    LoadCell_1.setCalFactor(calibrationValue_1); // usa o valor de calibração para calibrar cada HX711
    LoadCell_2.setCalFactor(calibrationValue_2);
    LoadCell_3.setCalFactor(calibrationValue_3);
    LoadCell_4.setCalFactor(calibrationValue_4);
    
    // Setup do LoRa
    manager.init();
    driver.setFrequency(868);
    driver.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

  }


}
void loop(){
    // Coleta os datados do sensores de peso
    float x = LoadCell_1.getData();
    float y = LoadCell_2.getData();
    float z = LoadCell_3.getData();
    float w = LoadCell_4.getData();

    float Weight = x + y + z + w ; //realiza a soma dos pesos e desconsidera 100kg que é o peso aproximado da lixeira

    // Coleta de dados dos sensores de distância

    float a = sonar1.ping_cm();
    float b = sonar2.ping_cm(); 
    float c = sonar3.ping_cm();

    float Distance = (a + b + c)/100; // pega os valores de distancias observados e troca unidade de cm para m

    float Height = 2.15 - Distance; // troca o referêncial o valor 2,15 é a altura do teto até essa lixeira
    // a função que irá enviar esses dados
    String data_string ="&" + String(Weight) + "$" + String(Height) + "$"; //Coloca os dados captados em uma String
    char data[14]; // Variavel char de 14 
    data_string.toCharArray(data, 14); // transforma a String em um Char
    manager.sendtoWait((uint8_t *)data, sizeof(data), SERVER_ADDRESS); // Envia a mensagem para o endereço do servidor
    manager.waitPacketSent(); // Espera o envio completo do pacote da mensagem
    delay(1000); // 1 segundos até a proxima atualização
}
