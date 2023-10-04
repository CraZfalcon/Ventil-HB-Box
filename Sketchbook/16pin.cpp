#include <Button2.h>
#define BUTTON_1_PIN  23
#define BUTTON_2_PIN  34
#define BUTTON_3_PIN  35
#define BUTTON_4_PIN  36
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


// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define GSM_PIN "0000"

// Set serial for AT commands (to SIM7000 module)

#include <TinyGsmClient.h>
#include <FRAM.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

const char apn[]      = "chili";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "0000"; 

const char server[]   = "https://oms.ventil.nl";
const int  port       = 8081;
const int  machinenr  = 1;

static const unsigned long REFRESH_INTERVAL = 10000;
static unsigned long lastRefreshTime = 0;

// I2C for SIM7000G (to keep it running when powered from battery)
// I2C for FRAM sensor
TwoWire I2CModem = TwoWire(0);
TwoWire I2CFRAM = TwoWire(1);
FRAM fram(&I2CFRAM);

#define SerialAT Serial1
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, server, port);


bool setPowerBoostKeepOn(int en){
  I2CModem.beginTransmission(0x75);
  I2CModem.write(0x00);
  if (en) { I2CModem.write(0x37); } // Set bit1: 1 enable 0 disable boost keep on
  else    { I2CModem.write(0x35); } // 0x37 is default reg value
  return I2CModem.endTransmission() == 0;
}

void pressed(Button2& btn) {
  if      (btn == button1)  { fram.write32(10000, (fram.read32(10000))+1); }
  else if (btn == button2)  { fram.write32(11000, (fram.read32(11000))+1); }
  else if (btn == button3)  { fram.write32(12000, (fram.read32(12000))+1); }
  else if (btn == button4)  { fram.write32(13000, (fram.read32(13000))+1); }
  else if (btn == button5)  { fram.write32(14000, (fram.read32(14000))+1); }
  else if (btn == button6)  { fram.write32(15000, (fram.read32(15000))+1); }
  else if (btn == button7)  { fram.write32(16000, (fram.read32(16000))+1); }
  else if (btn == button8)  { fram.write32(17000, (fram.read32(17000))+1); }
  else if (btn == button9)  { fram.write32(18000, (fram.read32(18000))+1); }
  else if (btn == button10) { fram.write32(19000, (fram.read32(19000))+1); }
  else if (btn == button11) { fram.write32(20000, (fram.read32(20000))+1); }
  else if (btn == button12) { fram.write32(21000, (fram.read32(21000))+1); }
  else if (btn == button13) { fram.write32(22000, (fram.read32(22000))+1); }
  else if (btn == button14) { fram.write32(23000, (fram.read32(23000))+1); }
  else if (btn == button15) { fram.write32(24000, (fram.read32(24000))+1); }
  else if (btn == button16) { fram.write32(25000, (fram.read32(25000))+1); }
}

void setup() {
  Serial.begin(115200);
  // Start I2C communication (SDA, SCL, Frequency)
  I2CModem.begin(21, 22, 400000);
  I2CFRAM.begin(16, 17, 100000);

  Serial.println(String("IP5306 KeepOn ") + (bool(setPowerBoostKeepOn(1)) ? "OK" : "FAIL"));

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, 26, 27);

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
  button14.begin(BUTTON_14_PIN);
  button14.setPressedHandler(pressed);
  button15.begin(BUTTON_15_PIN);
  button15.setPressedHandler(pressed);
  button16.begin(BUTTON_16_PIN);
  button16.setPressedHandler(pressed);

  fram.write32(500, machinenr);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // modem.init();

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) { modem.simUnlock(simPIN); }

  Serial.println(int(fram.begin(0x50)), HEX);
}

void sendpulseString() {
  Serial.print("Connecting to APN: ");
  Serial.println(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    String httpRequestData = "?HBBoxNumber=" + String(fram.read32(500)) + "&Channel1=" + String(fram.read32(10000)) + "&Channel2=" + String(fram.read32(11000)) + "&Channel3=" + String(fram.read32(12000)) + "&Channel4=" + String(fram.read32(13000)) + "&Channel5=" + String(fram.read32(14000)) + "&Channel6=" + String(fram.read32(15000)) + "&Channel7=" + String(fram.read32(16000)) + "&Channel8=" + String(fram.read32(17000)) + "&Channel9=" + String(fram.read32(18000)) + "&Channel10=" + String(fram.read32(19000)) + "&Channel11=" + String(fram.read32(20000)) + "&Channel12=" + String(fram.read32(21000)) + "&Channel13=" + String(fram.read32(22000)) + "&Channel14=" + String(fram.read32(23000)) + "&Channel15=" + String(fram.read32(24000)) + "&Channel16=" + String(fram.read32(25000));
    http.get("/VENTIL API ADRESS");
    //int statusCode = http.responseStatusCode();
    String response = http.responseBody();
    //Serial.println("Status code: " + statusCode);
    Serial.println("Response: " + response);
    Serial.println("");
  
    http.stop();
    Serial.println(F("Server disconnected"));
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }
}

void loop() {
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) {
    lastRefreshTime += REFRESH_INTERVAL;
    sendpulseString();
  }
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