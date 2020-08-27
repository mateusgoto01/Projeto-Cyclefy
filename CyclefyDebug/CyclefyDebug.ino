#include <HX711_ADC.h>
#include <NewPing.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <EEPROM.h>
// pins para os HX711
const int HX711_sck_1 = 3;
const int HX711_dout_1 = 4;
const int HX711_sck_2 = 5;
const int HX711_dout_2 = 6;
const int HX711_sck_3 = 8;
const int HX711_dout_3 = 9;
const int HX711_sck_4 = 0;
const int HX711_dout_4 = 1;

const int calVal_eepromAdress_1 = 0; // eeprom adress for calibration value load cell 1 (4 bytes)
const int calVal_eepromAdress_2 = 4; // eeprom adress for calibration value load cell 2 (4 bytes)

HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2

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

#define SERVER_ADDRESS 1 // Endereço do servidor 
#define CLIENT_ADDRESS 3// mudar conforme as lixeiras forem aumentando. Não use 1, esse é do servidor
RH_RF95 driver;
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void setup() {
    Serial.begin(9600);
    Serial.println("Iniciando...");
    // Setup do sensor de peso
    float calibrationValue_1; // variavel float para armazenar o valor de calibração do HX711 1
    float calibrationValue_2; // variavel float para armazenar o valor de calibração do HX711 2
    float calibrationValue_3; // variavel float para armazenar o valor de calibração do HX711 3
    float calibrationValue_4; // variavel float para armazenar o valor de calibração do HX711 4

    calibrationValue_1 = 22542.00; //Valores de calibragem para o HX711 1
    calibrationValue_2 = 23650.00; //Valores de calibragem para o HX711 2


    LoadCell_1.begin(); // inicia todos os HX711
    LoadCell_2.begin();


    long stabilizingtime = 2000;
    boolean _tare = false;

    byte loadcell_1_rdy = 0;
    byte loadcell_2_rdy = 0;


    while ((loadcell_1_rdy + loadcell_2_rdy < 2)){ // inicializa todos os HX711 simultaneamente
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare); //inicializa com a função preparada para multiplas celulas de carga
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }

    LoadCell_1.setCalFactor(calibrationValue_1); // usa o valor de calibração para calibrar cada HX711
    LoadCell_2.setCalFactor(calibrationValue_2);

    // Setup do sensor de peso realizado
    if (!manager.init())
    Serial.println("init failed");
    driver.setFrequency(868);
    driver.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

  }


void loop(){
    // Coleta os datados do sensores de peso
    LoadCell_1.update();
    LoadCell_2.update();
    delay(10);
    float x = LoadCell_1.getData();
    float y = LoadCell_2.getData();
    delay(100);

    Serial.println("Peso de cada HX711");
    Serial.println(x);
    delay(10);
    Serial.println(y);
    delay(10);
  


    float Weight = x + y; //realiza a soma dos pesos e desconsidera 100kg que é o peso aproximado da lixeira
    Serial.println("O peso é: "); Serial.println(Weight);
    // Coleta de dados dos sensores de distância

    float a = sonar1.ping_cm();
    float b = sonar2.ping_cm();
    float c = sonar3.ping_cm();
    Serial.println("Valor individual das distâncias: ");
    Serial.println(a);
    delay(10);
    Serial.println(b);
    delay(10);
    Serial.println(c);
    delay(10);

    float Distance = (a + b + c)/3; // pega os valores de distancias observados e realiza a média
    Serial.println("A distância é: "); Serial.println(Distance);
    float Height = -((Distance/100) - 2.15); // troca o referêncial o valor 2,15 é a altura do teto até essa lixeira
    Serial.println("A altura é: "); Serial.println(Height);
    // chama a função que irá enviar esses dados
    String data_string = String(Weight) + "$" + String(Height);
    char data[12];
    data_string.toCharArray(data, 12);
    manager.sendtoWait((uint8_t *)data, sizeof(data), SERVER_ADDRESS);
    manager.waitPacketSent();
    delay(1000); // 1 segundos até a proxima atualização


}
