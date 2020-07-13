//Definindo os pinos para cada modulo HX711
#define HX711_SCK1 8
#define HX711_DT1 9

unsigned long ReadCount();
unsigned long convert;



void setup()
{
   pinMode(HX711_DT1, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(HX711_SCK1, OUTPUT);         //saída para SCK
   
   Serial.begin(9600);
}

void loop()
{
  convert = ReadCount();
  
  Serial.println(convert);
  
  delay(2000);
}

unsigned long ReadCount()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(HX711_SCK1, LOW);
  
  while(digitalRead(HX711_DT1));
  
  for(i=0;i<24;i++)
  {
     digitalWrite(HX711_SCK1, HIGH);
     Count = Count << 1;
     digitalWrite(HX711_SCK1, LOW);
     if(digitalRead(HX711_DT1)) Count++;
  
  }
  
  digitalWrite(HX711_SCK1, HIGH);
  Count = Count^0x800000;
  digitalWrite(HX711_SCK1, LOW);
  
  return(Count);


} 
