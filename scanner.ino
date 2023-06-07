#include <Arduino.h>
#include<Wire.h>  // include I2C library
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C
u8g2(U8G2_R0, /*clock=*/14,/*data=*/12, U8X8_PIN_NONE);

void setup() {
  //u8g2.begin();
  Serial.begin (9600);  // initialization of Serial communication at 9600 bps
  while(!Serial)
  {
    
  }
  Serial.println("I2C Scanning.......");
  byte count = 0;
  Wire.begin();  //  Initialize I2C protocol
  for(byte i; i<120; i++)   //   number of available addresses is 120
  {
    Wire.beginTransmission(i);  // trying to transmit data to every addresses
    if(Wire.endTransmission() == 0)  // if that addresses return data zero 
    {
      Serial.print("Found I2C 0x");
      Serial.println(i,HEX);  // print value of i which is I2C 
      count++;
      delay(10);
    }
  }
  Serial.print("Found ");
  Serial.print(count,HEX);
  Serial.println(" device");

}

void loop() {
  // put your main code here, to run repeatedly:

}
