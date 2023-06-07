# garden_sensor
IoT Garden Sensor Project
This project utilizes a SHT-30 industrial grade temperature and humidity sensor to monitor soil moisture and temperature through a NodeMCU (ESP8266) broadcasting data at a 2.4GHz frequency. Captured data is connected to an Adafruit IO cloud service via WiFi for access with any connected device. 

Bill of Materials:
1) SHT-30 Sensor
1.a: https://www.adafruit.com/product/4099?gclid=EAIaIQobChMIz7HKwO-x_wIVwDjUAR0BLgk2EAUYAiABEgLRcPD_BwE
1.b: https://www.amazon.com/Taidacent-Temperature-Humidity-Sensor-Anti-Condensation/dp/B07F9X9Q37/ref=sr_1_2?crid=168GPGM869LQC&keywords=sht-30&qid=1686165425&sprefix=sht-30%2Caps%2C127&sr=8-2

2) NodeMCU
2.a: https://www.amazon.com/Development-Wireless-Micropython-Programming-Soldered/dp/B0BVM3H46B/ref=sr_1_1?crid=1FLSOOBLJSS92&keywords=node+mcu+oled&qid=1686165640&s=electronics&sprefix=node+mcu+oled%2Celectronics%2C114&sr=1-1

3) Solar Manager and Panel
3.a: https://www.dfrobot.com/product-2000.html

4) Outdoor Rated Enclosure 
4.a: https://www.amazon.com/dp/B0BV7GJNQB/ref=twister_B0BHXFJQ3V?_encoding=UTF8&psc=1

Sensor Connection Pinout
NodeMCU <==> SHT-30
D1      <==> Yellow (SCL)
D2      <==> Blue (SDA)
G       <==> Green
3V3     <==> Red

Process
1) connect the SHT-30 per the pinout above. 
2) Flash the scanner.io and run to find the slave address of the device. This will then be used within the main code.
3) Create an account with Adafruit IO to be able to send and display data.
4) Use the information from the previous step within the main code as outlined in the firmware.
5) Flash the humnidity_01_public.io on your node mcu. Open the serial monitor to observe real time processing. 
