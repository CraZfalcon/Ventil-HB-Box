#include <Arduino.h>

#include "Button2.h"
#define BUTTON_1_PIN  12
#define BUTTON_2_PIN  23
#define BUTTON_3_PIN  25
#define BUTTON_4_PIN  27
#define BUTTON_5_PIN  19
#define BUTTON_6_PIN  4
#define BUTTON_7_PIN  27
#define BUTTON_8_PIN  32
#define BUTTON_9_PIN  33
#define BUTTON_10_PIN  16
#define BUTTON_11_PIN  17
#define BUTTON_12_PIN  5
#define BUTTON_13_PIN  13
#define BUTTON_14_PIN  26
#define BUTTON_15_PIN  14
#define BUTTON_16_PIN  15
Button2 button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14, button15, button16;

#include "FRAM.h"
FRAM fram;

uint32_t start;
uint32_t stop;
uint32_t sizeInBytes = 0;

#include <Wire.h>


void setup() {
  Serial.begin(115200);
  delay(50);

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
  button14.begin(BUTTON_14_PIN);
  button14.setPressedHandler(pressed);
  button15.begin(BUTTON_15_PIN);
  button15.setPressedHandler(pressed);
  button16.begin(BUTTON_16_PIN);
  button16.setPressedHandler(pressed);

  Wire.begin();
  int rv = fram.begin(0x50);
  if (rv != 0)
  {
    Serial.print("INIT ERROR: ");
    Serial.println(rv);
  }

  uint16_t machinenr = 21570;
  fram.write32(500, machinenr);
}


void loop() {
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
  button14.loop();
  button15.loop();
  button16.loop();
}

void pressed(Button2& btn) {
  if (btn == button1) {
    // Read Sensor 1 counter
    uint16_t address = 10000; // wherever
    long sensor_1 = fram.read32(address);
    Serial.print(" Pump 1 strokes "); Serial.print(sensor_1); Serial.println(" times");
    // Sensor 1 counter write ++
    fram.write32(address, sensor_1+1);
  } else if (btn == button2) {
    // Read Sensor 2 counter
    uint16_t address = 11000; // wherever
    long sensor_2 = fram.read32(address);
    Serial.print(" Pump 2 strokes "); Serial.print(sensor_2); Serial.println(" times");
    // Sensor 2 counter write ++
    fram.write32(address, sensor_2+1);
  }
  else if (btn == button3) {
    // Read Sensor 3 counter
    uint16_t address = 12000; // wherever
    long sensor_3 = fram.read32(address);
    Serial.print(" Pump 3 strokes "); Serial.print(sensor_3); Serial.println(" times");
    // Sensor 3 counter write ++
    fram.write32(address, sensor_3+1); 
  }
  else if (btn == button4) {
    // Read Sensor 4 counter
    uint16_t address = 13000; // wherever
    long sensor_4 = fram.read32(address);
    Serial.print(" Pump 4 strokes "); Serial.print(sensor_4); Serial.println(" times");
    // Sensor 4 counter write ++
    fram.write32(address, sensor_4+1);
  }
  else if (btn == button5) {
    // Read Sensor 5 counter
    uint16_t address = 14000; // wherever
    long sensor_5 = fram.read32(address);
    Serial.print(" Pump 5 strokes "); Serial.print(sensor_5); Serial.println(" times");
    // Sensor 5 counter write ++
    fram.write32(address, sensor_5+1);
  }
  else if (btn == button6) {
    // Read Sensor 6 counter
    uint16_t address = 15000; // wherever
    long sensor_6 = fram.read32(address);
    Serial.print(" Pump 6 strokes "); Serial.print(sensor_6); Serial.println(" times");
    // Sensor 6 counter write ++
    fram.write32(address, sensor_6+1);
  }
  else if (btn == button7) {
    // Read Sensor 7 counter
    uint16_t address = 16000; // wherever
    long sensor_7 = fram.read32(address);
    Serial.print(" Pump 7 strokes "); Serial.print(sensor_7); Serial.println(" times");
    // Sensor 7 counter write ++
    fram.write32(address, sensor_7+1);
  }
  else if (btn == button8) {
    // Read Sensor 8 counter
    uint16_t address = 17000; // wherever
    long sensor_8 = fram.read32(address);
    Serial.print(" Pump 8 strokes "); Serial.print(sensor_8); Serial.println(" times");
    // Sensor 8 counter write ++
    fram.write32(address, sensor_8+1);
  }
  else if (btn == button9) {
    // Read Sensor 9 counter
    uint16_t address = 18000; // wherever
    long sensor_9 = fram.read32(address);
    Serial.print(" Pump 9 strokes "); Serial.print(sensor_9); Serial.println(" times");
    // Sensor 9 counter write ++
    fram.write32(address, sensor_9+1);
  }
  else if (btn == button10) {
    // Read Sensor 10 counter
    uint16_t address = 19000; // wherever
    long sensor_10 = fram.read32(address);
    Serial.print(" Pump 10 strokes "); Serial.print(sensor_10); Serial.println(" times");
    // Sensor 10 counter write ++
    fram.write32(address, sensor_10+1);
  }
  else if (btn == button11) {
    // Read Sensor 11 counter
    uint16_t address = 20000; // wherever
    long sensor_11 = fram.read32(address);
    Serial.print(" Pump 11 strokes "); Serial.print(sensor_11); Serial.println(" times");
    // Sensor 11 counter write ++
    fram.write32(address, sensor_11+1);
  }
  else if (btn == button12) {
    // Read Sensor 12 counter
    uint16_t address = 21000; // wherever
    long sensor_12 = fram.read32(address);
    Serial.print(" Pump 12 strokes "); Serial.print(sensor_12); Serial.println(" times");
    // Sensor 12 counter write ++
    fram.write32(address, sensor_12+1);
  }
  else if (btn == button13) {
    // Read Sensor 13 counter
    uint16_t address = 22000; // wherever
    long sensor_13 = fram.read32(address);
    Serial.print(" Pump 13 strokes "); Serial.print(sensor_13); Serial.println(" times");
    // Sensor 13 counter write ++
    fram.write32(address, sensor_13+1);
  }
  else if (btn == button14) {
    // Read Sensor 14 counter
    uint16_t address = 23000; // wherever
    long sensor_14 = fram.read32(address);
    Serial.print(" Pump 14 strokes "); Serial.print(sensor_14); Serial.println(" times");
    // Sensor 14 counter write ++
    fram.write32(address, sensor_14+1);
  }
  else if (btn == button15) {
    // Read Sensor 15 counter
    uint16_t address = 24000; // wherever
    long sensor_15 = fram.read32(address);
    Serial.print(" Pump 15 strokes "); Serial.print(sensor_15); Serial.println(" times");
    // Sensor 15 counter write ++
    fram.write32(address, sensor_15+1);
  }
  else if (btn == button16) {
    // Read Sensor 16 counter
    uint16_t address = 25000; // wherever
    long sensor_16 = fram.read32(address);
    Serial.print(" Pump 16 strokes "); Serial.print(sensor_16); Serial.println(" times");
    // Sensor 16 counter write ++
    fram.write32(address, sensor_16+1);
  }
}

void mainpulseCounter() {
  static const unsigned long REFRESH_INTERVAL = 10000; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
    lastRefreshTime += REFRESH_INTERVAL;
                sendpulseString();
  }
}

void sendpulseString() {
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
  long sensor_14 = fram.read32(23000);
  long sensor_15 = fram.read32(24000);
  long sensor_16 = fram.read32(25000);
  Serial.print(machinenr);
  Serial.print(" , ");
  Serial.print(sensor_1);
  Serial.print(" , ");
  Serial.print(sensor_2);
  Serial.print(" , ");
  Serial.print(sensor_3);
  Serial.print(" , ");
  Serial.print(sensor_4);
  Serial.print(" , ");
  Serial.print(sensor_5);
  Serial.print(" , ");
  Serial.print(sensor_6);
  Serial.print(" , ");
  Serial.print(sensor_7);
  Serial.print(" , ");
  Serial.print(sensor_8);
  Serial.print(" , ");
  Serial.print(sensor_9);
  Serial.print(" , ");
  Serial.print(sensor_10);
  Serial.print(" , ");
  Serial.print(sensor_11);
  Serial.print(" , ");
  Serial.print(sensor_12);
  Serial.print(" , ");
  Serial.print(sensor_13);
  Serial.print(" , ");
  Serial.print(sensor_14);
  Serial.print(" , ");
  Serial.print(sensor_15);
  Serial.print(" , ");
  Serial.println(sensor_16);
}