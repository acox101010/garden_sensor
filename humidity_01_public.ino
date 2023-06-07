#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <SHT31.h>
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define SHT31_ADDRESS   0x44 // scanner.ino used to find slave address at 0x44 (decimal 68)
#define MSG_BUFFER_SIZE (50)
#define WLAN_SSID       "YOUR SSID HERE"
#define WLAN_PASS       "YOUR WIFI PW HERE"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  "YOUR SERVER PORT HERE"
#define AIO_USERNAME    "YOUR USERNAME YOUR CREATED ON ADAFRUIT"
#define AIO_KEY         "YOUR ADAFRUIT IO "

uint32_t start;
uint32_t stop;
uint32_t connectionFails = 0;
unsigned long lastMsg = 0;
byte hum = 0;  //Stores humidity value
byte temp = 0; //Stores temperature value

char msg1[MSG_BUFFER_SIZE]; 
char msg2[MSG_BUFFER_SIZE];

SHT31      sht;
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature1");
Adafruit_MQTT_Publish Humidity1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity1");

//U8G2 Constructors
U8G2_SSD1306_128X64_NONAME_F_SW_I2C
u8g2(U8G2_R0, /*clock=*/14,/*data=*/12, U8X8_PIN_NONE);


void setup() 
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  Serial.begin(115200); /* begin serial for debug */
  Wire.begin(D2, D1); /* join i2c bus with SDA=D2 and SCL=D1 of NodeMCU */
  sht.begin(SHT31_ADDRESS);
  Wire.setClock(1000000);
// Set Sensor variable to read SHT-30 data
  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();
  pinMode(BUILTIN_LED, OUTPUT);
  delay(10);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("connecting...");
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("Connecting to WiFi"); //°C converted to °F  
    u8g2.sendBuffer();
  }
  
  // print wifi status
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  connect();
  }

void connect()
{
  Serial.print(F("Connecting to Adafruit IO... "));
  Serial.println("Connecting to Adafruit IO... ");
  u8g2.setFont(u8g2_font_7x14B_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("Connecting to Cloud"); //°C converted to °F  
  u8g2.sendBuffer();
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); 
      break;
      case 2: Serial.println(F("ID rejected")); 
      break;
      case 3: Serial.println(F("Server unavail")); 
      break;
      case 4: Serial.println(F("Bad user/pass")); 
      break;
      case 5: Serial.println(F("Not authed")); 
      break;
      case 6: Serial.println(F("Failed to subscribe")); 
      break;
      default: Serial.println(F("Connection failed")); 
      break;
    }
    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() 
{
  if (sht.isConnected())
  {
    start = micros();
    bool b = sht.read();         // default = true/fast       slow = false
    stop = micros();
    int error = sht.getError();
    uint16_t stat = sht.readStatus();
    temp = sht.getTemperature()* 9 / 5 + 32 , 1;
    hum = sht.getHumidity(), 1;
    Serial.print("\t");
    Serial.print("Temp-F: ");
    Serial.println(temp); //°C converted to °F
    u8g2.setFont(u8g2_font_7x14B_tr);
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("Temp °F"); //°C converted to °F
    u8g2.setCursor(50, 15);
    u8g2.print(temp); //°C converted to °F
    delay(5000);
    Serial.print("\t");
    Serial.print("Moisture %: ");
    Serial.println(hum);
    u8g2.setCursor(0, 40);
    u8g2.print("Moisture %"); 
    u8g2.setCursor(70, 40);
    u8g2.print(hum);
    u8g2.setCursor(0, 60);
    u8g2.print("WiFi Connected"); //°C converted to °F
    u8g2.sendBuffer();
    delay(5000);
  }
  if (!sht.isConnected())
  {
    connectionFails++;
    Serial.print(millis());
    Serial.print("\tNot connected:\t");
    Serial.print(connectionFails);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("Sensor Error"); //°C converted to °F
    u8g2.sendBuffer();
  }

  if(!mqtt.ping(3))
  {
    if(!mqtt.connected()){
      connect();
    }   
  }
  if (!Temperature1.publish(sht.getTemperature()* 9 / 5 + 32 , 1)) 
  {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    } 
  if (! Humidity1.publish(sht.getHumidity(), 1)) 
  {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    }
  else
  {
    Serial.println(F("Sent!"));
  } 
  // Handle the Sensor failure by outputting to display
  // Potential to add as network message. 
}


