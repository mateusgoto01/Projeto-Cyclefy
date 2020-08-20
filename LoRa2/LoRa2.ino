#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define CLIENT_ADDRESS 2
#define SERVER_ADDRESS 1

// Singleton instance of the radio driver
RH_RF95 driver;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);




void setup() 
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);

  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!manager.init())
    Serial.println("init failed");
    driver.setFrequency(868);

}


void loop()
{
  float peso = 10.5;
  float altura = 5.25;
  String data_string = "";
  uint8_t* char_data = (uint8_t *)data_string.c_str();
  manager.sendtoWait(char_data, sizeof(char_data), SERVER_ADDRESS);
  Serial.println("Mensagem enviada");
   
  delay(5000); // 5 segundos até a proxima atualização
}
