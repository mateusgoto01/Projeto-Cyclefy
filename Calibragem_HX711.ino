#include <HX711.h> //inclui a biblioteca do HX711

//Definindo os pinos para cada modulo HX711
#define HX711_SCK1 8
#define HX711_DT1 9
#define HX711_SCK2 10  
#define HX711_DT2 11
//Definindo como se estivesse duas balanças
HX711 scale1;
HX711 scale2; 
//Variaveis que irão armazenar valores encontrados pelas celulas de carga
float medida1 = 0;
float medida2 = 0;

void setup()
{
    Serial.begin(9600);
    scale1.begin(HX711_DT1, HX711_SCK1); // configurando os pin para as duas balanças
    scale2.begin(HX711_DT2, HX711_SCK2);

    scale1.set_scale(); // limpando a escala da primeira balança
    scale2.set_scale(); // limpando a escala da segunda balança

    scale1.tare(); //zerando a massa da estrutura
    scale2.tare();

    Serial.println("Tara feito"); // A tara está no void setup, então ao iniciar o arduino já deixe encima a estrutura

}

void loop()
{
	medida1 = scale1.get_units(20); //salva na variavel um valor na média de 20 respostas
    medida2 = scale1.get_units(20);

    
    Serial.print("Medida do primeiro HX711: "); // printa no serial os valores encontrados
    Serial.print(medida1, 3); // printa até 3 casas decimais 
    Serial.print(" Media do segundo HX711: ");
    Serial.print(medida2, 3);
    Serial.println();

    scale1.power_down(); // desliga os sensores
    scale2.power_down();
    delay(1000);
    scale1.power_up(); // liga os sensores
    scale2.power_up();

}
