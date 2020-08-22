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
    driver.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);

}


void loop()
{
  float peso = 350.8425;
  float altura = 3.7125;

  String data_string = String(peso) + "$" + String(altura);
  char data[12];
  data_string.toCharArray(data, 12);
  Serial.println(data_string);
  manager.sendtoWait((uint8_t *)data, sizeof(data), SERVER_ADDRESS);
  manager.waitPacketSent();
  Serial.println("Mensagem enviada");
  delay(1000); // 1 segundos até a proxima atualização


}
