/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-sim800l-publish-data-to-cloud/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include "Button2.h"
#define BUTTON_1_PIN  23 // was 32 Tijdelijk dubbel gebruikt vanwege inputs te weinig
#define BUTTON_2_PIN  34 // was 23
#define BUTTON_3_PIN  35 // was 34
#define BUTTON_4_PIN  36 // was 35
#define BUTTON_5_PIN  39 // was 36
#define BUTTON_6_PIN  32 // was 39
#define BUTTON_7_PIN  15 // was 32 
#define BUTTON_8_PIN  33 // was 32
#define BUTTON_9_PIN  18 // was 33
#define BUTTON_10_PIN 19 // was 18
#define BUTTON_11_PIN  5 // was 19
#define BUTTON_12_PIN  13 // was 5
#define BUTTON_13_PIN  14 // Pulsecounter extern
// #define BUTTON_14_PIN  2 // 
// #define BUTTON_15_PIN  3 // 
// #define BUTTON_16_PIN  15 //

/////////////////////////////////////////////////////////////////
// Button2 button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14, button15, button16;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
Button2 button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13;
/////////////////////////////////////////////////////////////////

#include "FRAM.h"

uint32_t start;
uint32_t stop;
uint32_t sizeInBytes = 0;

// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "chili"; // APN 
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = "0000"; 

// Server details
const char server[] = "enisadesign.nl"; // The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char resource[] = "/ventil/post-data.php";         // resource path, for example: /post-data.php
const int  port = 80;                             // server port number

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

// SIM7000G Pinout
#define UART_BAUD           115200
#define PIN_DTR             25
#define PIN_TX              27
#define PIN_RX              26
#define PWR_PIN             4

#define I2C_SDA              21
#define I2C_SCL              22

// FRAM Memory
#define I2C_SDA_2            16
#define I2C_SCL_2            17

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM7000 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

// set GSM PIN, if any
#define GSM_PIN "0000"

// #define I2C_Freq 400000UL
// #define I2C_Freq_2 100000UL

const uint32_t I2C_Freq = 400000;
const uint32_t I2C_Freq_2 = 100000;

#include <Wire.h>
#include <TinyGsmClient.h>
#include <SPI.h>
#include <Ticker.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

// I2C for SIM7000G (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// I2C for FRAM sensor
TwoWire I2CFRAM = TwoWire(1);
FRAM fram(&I2CFRAM);    

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);        

#define uS_TO_S_FACTOR 1000000UL   /* Conversion factor for micro seconds to seconds */
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

void setup() {
  // Set serial monitor debugging window baud rate to 115200
  SerialMon.begin(115200);
  // Start I2C communication
 I2CPower.begin(I2C_SDA, I2C_SCL, I2C_Freq);
 I2CFRAM.begin(I2C_SDA_2, I2C_SCL_2, I2C_Freq_2);

  // Keep power when running from battery
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  // Set modem reset, enable, power pins
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);

  // Set GSM module baud rate and UART pins
   SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  
  delay(3000);
  button1.begin(BUTTON_1_PIN);
  button1.setPressedHandler(pressed);
  button2.begin(BUTTON_2_PIN);
  button2.setPressedHandler(pressed);
  button3.begin(BUTTON_3_PIN);
  button3.setPressedHandler(pressed);
  button4.begin(BUTTON_4_PIN);
  button4.setPressedHandler(pressed);
  button5.begin(BUTTON_5_PIN);
  button5.setPressedHandler(pressed);
  button6.begin(BUTTON_6_PIN);
  button6.setPressedHandler(pressed);
  button7.begin(BUTTON_7_PIN);
  button7.setPressedHandler(pressed);
  button8.begin(BUTTON_8_PIN);
  button8.setPressedHandler(pressed);
  button9.begin(BUTTON_9_PIN);
  button9.setPressedHandler(pressed);
  button10.begin(BUTTON_10_PIN);
  button10.setPressedHandler(pressed);
  button11.begin(BUTTON_11_PIN);
  button11.setPressedHandler(pressed);
  button12.begin(BUTTON_12_PIN);
  button12.setPressedHandler(pressed);
  button13.begin(BUTTON_13_PIN);
  button13.setPressedHandler(pressed);
 // button14.begin(BUTTON_14_PIN);
//  button14.setPressedHandler(pressed);
 // button15.begin(BUTTON_15_PIN);
 // button15.setPressedHandler(pressed);
 // button16.begin(BUTTON_16_PIN);
 // button16.setPressedHandler(pressed);

   // Serial.print("test16:\t");
  uint16_t machinenr = 21570;
  fram.write32(500, machinenr);
  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
  
// You might need to change the BME280 I2C address, in our case it's 0x76
//if (!fram.begin(0x50))
//{
//Serial.println("Could not find a valid FRAM sensor, check wiring!");
//while (1);
//}

  // You might need to change the FRAM I2C address, in our case it's 0x50
  int x = fram.begin(0x50);
  Serial.println(x, HEX);
}


void loop()
{
mainpulseCounter();
button1.loop();
button2.loop();
button3.loop();
button4.loop();
button5.loop();
button6.loop();
button7.loop();
button8.loop();
button9.loop();
button10.loop();
button11.loop();
button12.loop();
button13.loop();
// button14.loop();
// button15.loop();
// button16.loop();
}
/////////////////////////////////////////////////////////////////
void pressed(Button2& btn) {
if (btn == button1) {
// Read Sensor 1 counter
uint16_t address = 10000; // wherever
long sensor_1 = fram.read32(address);
// Serial.print(" Pump 1 strokes "); Serial.print(sensor_1); Serial.println(" times");
// Sensor 1 counter write ++
fram.write32(address, sensor_1+1);
} else if (btn == button2) {
// Read Sensor 2 counter
uint16_t address = 11000; // wherever
long sensor_2 = fram.read32(address);
// Serial.print(" Pump 2 strokes "); Serial.print(sensor_2); Serial.println(" times");
// Sensor 2 counter write ++
fram.write32(address, sensor_2+1);
}
else if (btn == button3) {
// Read Sensor 3 counter
uint16_t address = 12000; // wherever
long sensor_3 = fram.read32(address);
// Serial.print(" Pump 3 strokes "); Serial.print(sensor_3); Serial.println(" times");
// Sensor 3 counter write ++
fram.write32(address, sensor_3+1); 
}
else if (btn == button4) {
// Read Sensor 4 counter
uint16_t address = 13000; // wherever
long sensor_4 = fram.read32(address);
// Serial.print(" Pump 4 strokes "); Serial.print(sensor_4); Serial.println(" times");
// Sensor 4 counter write ++
fram.write32(address, sensor_4+1);
}
else if (btn == button5) {
// Read Sensor 5 counter
uint16_t address = 14000; // wherever
long sensor_5 = fram.read32(address);
// Serial.print(" Pump 5 strokes "); Serial.print(sensor_5); Serial.println(" times");
// Sensor 5 counter write ++
fram.write32(address, sensor_5+1);
}
else if (btn == button6) {
// Read Sensor 6 counter
uint16_t address = 15000; // wherever
long sensor_6 = fram.read32(address);
// Serial.print(" Pump 6 strokes "); Serial.print(sensor_6); Serial.println(" times");
// Sensor 6 counter write ++
fram.write32(address, sensor_6+1);
}
else if (btn == button7) {
// Read Sensor 7 counter
uint16_t address = 16000; // wherever
long sensor_7 = fram.read32(address);
// Serial.print(" Pump 7 strokes "); Serial.print(sensor_7); Serial.println(" times");
// Sensor 7 counter write ++
fram.write32(address, sensor_7+1);
}
else if (btn == button8) {
// Read Sensor 8 counter
uint16_t address = 17000; // wherever
long sensor_8 = fram.read32(address);
// Serial.print(" Pump 8 strokes "); Serial.print(sensor_8); Serial.println(" times");
// Sensor 8 counter write ++
fram.write32(address, sensor_8+1);
}
else if (btn == button9) {
// Read Sensor 9 counter
uint16_t address = 18000; // wherever
long sensor_9 = fram.read32(address);
// Serial.print(" Pump 9 strokes "); Serial.print(sensor_9); Serial.println(" times");
// Sensor 9 counter write ++
fram.write32(address, sensor_9+1);
}
else if (btn == button10) {
// Read Sensor 10 counter
uint16_t address = 19000; // wherever
long sensor_10 = fram.read32(address);
// Serial.print(" Pump 10 strokes "); Serial.print(sensor_10); Serial.println(" times");
// Sensor 10 counter write ++
fram.write32(address, sensor_10+1);
}
else if (btn == button11) {
// Read Sensor 11 counter
uint16_t address = 20000; // wherever
long sensor_11 = fram.read32(address);
// Serial.print(" Pump 11 strokes "); Serial.print(sensor_11); Serial.println(" times");
// Sensor 11 counter write ++
fram.write32(address, sensor_11+1);
}
else if (btn == button12) {
// Read Sensor 12 counter
uint16_t address = 21000; // wherever
long sensor_12 = fram.read32(address);
// Serial.print(" Pump 12 strokes "); Serial.print(sensor_12); Serial.println(" times");
// Sensor 12 counter write ++
fram.write32(address, sensor_12+1);
}
else if (btn == button13) {
// Read Sensor 13 counter
uint16_t address = 22000; // wherever
long sensor_13 = fram.read32(address);
// Serial.print(" Pump 13 strokes "); Serial.print(sensor_13); Serial.println(" times");
// Sensor 13 counter write ++
fram.write32(address, sensor_13+1);
}
// else if (btn == button14) {
// Read Sensor 14 counter
// uint16_t address = 23000; // wherever
// long sensor_14 = fram.read32(address);
// Serial.print(" Pump 14 strokes "); Serial.print(sensor_14); Serial.println(" times");
// Sensor 14 counter write ++
// fram.write32(address, sensor_14+1);
// }
// else if (btn == button15) {
// Read Sensor 15 counter
// uint16_t address = 24000; // wherever
// long sensor_15 = fram.read32(address);
// Serial.print(" Pump 15 strokes "); Serial.print(sensor_15); Serial.println(" times");
// Sensor 15 counter write ++
// fram.write32(address, sensor_15+1);
// }
// else if (btn == button16) {
// Read Sensor 16 counter
// uint16_t address = 25000; // wherever
// long sensor_16 = fram.read32(address);
// Serial.print(" Pump 16 strokes "); Serial.print(sensor_16); Serial.println(" times");
// Sensor 16 counter write ++
//fram.write32(address, sensor_16+1);
//}
}

void mainpulseCounter()
{
  static const unsigned long REFRESH_INTERVAL = 10000; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
    lastRefreshTime += REFRESH_INTERVAL;
                sendpulseString();
  }
}

void sendpulseString()
{  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  }
  else {
    SerialMon.println(" OK");
    
    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client.connect(server, port)) {
      SerialMon.println(" fail");
    }
    else {
      SerialMon.println(" OK");

//    Serial print counter values
long machinenr = fram.read32(500);
long sensor_1 = fram.read32(10000);
long sensor_2 = fram.read32(11000);
long sensor_3 = fram.read32(12000);
long sensor_4 = fram.read32(13000);
long sensor_5 = fram.read32(14000);
long sensor_6 = fram.read32(15000);
long sensor_7 = fram.read32(16000);
long sensor_8 = fram.read32(17000);
long sensor_9 = fram.read32(18000);
long sensor_10 = fram.read32(19000);
long sensor_11 = fram.read32(20000);
long sensor_12 = fram.read32(21000);
long sensor_13 = fram.read32(22000);
// long sensor_14 = fram.read32(23000);
// long sensor_15 = fram.read32(24000);
// long sensor_16 = fram.read32(25000);
 Serial.print(machinenr);
 Serial.print(",");
 Serial.print(sensor_1);
 Serial.print(",");
 Serial.print(sensor_2);
 Serial.print(",");
 Serial.print(sensor_3);
// Serial.print(",");
// Serial.print(sensor_4);
// Serial.print(",");
// Serial.print(sensor_5);
// Serial.print(",");
// Serial.print(sensor_6);
// Serial.print(",");
// Serial.print(sensor_7);
// Serial.print(",");
// Serial.print(sensor_8);
// Serial.print(",");
// Serial.print(sensor_9);
// Serial.print(",");
// Serial.print(sensor_10);
// Serial.print(",");
// Serial.print(sensor_11);
// Serial.print(",");
// Serial.print(sensor_12);
// Serial.print(",");
// Serial.print(sensor_13);
// Serial.print(",");
// Serial.print(sensor_14);
// Serial.print(",");
// Serial.print(sensor_15);
// Serial.print(",");
// Serial.println(sensor_16);
    
// Making an HTTP POST request
SerialMon.println("Performing HTTP POST request...");
// String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(machinenr)
//                             + "&value2=" + String(sensor_1) + "&value3=" + String(sensor_2) + "&value4=" + String(sensor_3)
//                             + "&value5=" + String(sensor_4) + "&value6=" + String(sensor_5) + "&value7=" + String(sensor_6)
//                            + "&value8=" + String(sensor_7) + "&value9=" + String(sensor_8) + "&value10=" + String(sensor_9)
//                             + "&value11=" + String(sensor_10) + "&value12=" + String(sensor_11) + "&value13=" + String(sensor_12)
 //                            + "&value14=" + String(sensor_13) + "&value15=" + String(sensor_14) + "&value16=" + String(sensor_15)
//                             + "&value17=" + String(sensor_16) + "";

String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(machinenr)
                             + "&value2=" + String(sensor_1) + "&value3=" + String(sensor_2) + "&value4=" + String(sensor_3)
                             + "&value5=" + String(sensor_4) + "&value6=" + String(sensor_5) + "&value7=" + String(sensor_6)
                             + "&value8=" + String(sensor_7) + "&value9=" + String(sensor_8) + "&value10=" + String(sensor_9)
                             + "&value11=" + String(sensor_10) + "&value12=" + String(sensor_11) + "&value13=" + String(sensor_12)
                             + "&value14=" + String(sensor_13) + "";
client.print(String("POST ") + resource + " HTTP/1.1\r\n");
client.print(String("Host: ") + server + "\r\n");
client.println("Connection: close");
client.println("Content-Type: application/x-www-form-urlencoded");
client.print("Content-Length: ");
client.println(httpRequestData.length());
client.println();
client.println(httpRequestData);

unsigned long timeout = millis();
while (client.connected() && millis() - timeout < 10000L) {
// Print available data (HTTP response from server)
while (client.available()) {
char c = client.read();
SerialMon.print(c);
timeout = millis();
}
}
SerialMon.println();
    
// Close client and disconnect
client.stop();
SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
}