#include "HX711-multi.h" // inclui a biblioteca para usar multiplos HX711

#define CLK A0 //Definindo o pino de clock de todos os modulos para A0
#define DOUT1 A1 //Definindo o pino de dato do primeiro modulo para A1
#define DOUT2 A2 //Definindo o pino de dato do segundo modulo para A2
#define DOUT3 A3 //Definindo o pino de dato do terceiro modulo para A3

#define Calibration_Message "A calibração começou"

#define Tare_Timeout_Seconds 4 //time out para fazer a tara

byte DOUTS[3] = {DOUT1, DOUT2, DOUT3}; //Cria a variavel byte para amazenar os datos

#define Channel_Count sizeof(DOUTS)/sizeof(byte) // divide os datos pelos byte, para retornar um valor como se fosse apenas um HX711

long int Results[Channel_Count]; // armazena o resultado do contador de canal em uma variavel inteira

HX711MULTI scales(Channel_Count, DOUTS, CLK);


void tare() { //void responsavel por fazer a tara de acordo com a biblioteca
  bool TareSuccessful = false;

  unsigned long TareStartTime = millis();
  while (!TareSuccessful && millis()<(TareStartTime+Tare_Timeout_Seconds*1000)) {
    TareSuccessful = scales.tare(20,10000); 
  }
}
  
void SendRawData(){ //Void responsavel por mandar os datos brutos que for coletado das celulas de carga
  scales.read(Results);
  for (int i=0; i<scales.get_count(); ++i) { //começa a armazenar numa int i os valores dos bytes da celula de carga
    Serial.print( -Results[i]); //mostra no serial os resultados
    Serial.print( (i!=scales.get_count()-1)?"\t":"\n");
    }
    delay(10);
  }
  
void setup() {
   Serial.begin(115200);
   Serial.println(Calibration_Message);
   Serial.flush();

   tare();
}

void loop() {
  SendRawData(); // chamo a void de monstrar os datos brutos no serial
  if (Serial.available()>0) {
    while (Serial.available()){
        Serial.read();
      }
      tare(); // refaz a tara toda checagem
    }

}
