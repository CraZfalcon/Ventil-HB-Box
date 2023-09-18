
#include <Button2.h>
#define BUTTON_1_PIN  23
#define BUTTON_2_PIN  34
#define BUTTON_3_PIN  35
#define BUTTON_4_PIN  36
Button2 button1, button2, button3, button4;


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
  if (en) {
    I2CModem.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CModem.write(0x35); // 0x37 is default reg value
  }
  return I2CModem.endTransmission() == 0;
}

void pressed(Button2& btn) {
  if      (btn == button1) { fram.write32(10000, (fram.read32(10000))+1); }
  else if (btn == button2) { fram.write32(11000, (fram.read32(11000))+1); }
  else if (btn == button3) { fram.write32(12000, (fram.read32(12000))+1); }
  else if (btn == button4) { fram.write32(13000, (fram.read32(13000))+1); }
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

  fram.write32(500, machinenr);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // modem.init();

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  Serial.println(int(fram.begin(0x50)), HEX);
}

void sendpulseString() {
  Serial.print("Connecting to APN: ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    String httpRequestData = "?HBBoxNumber=" + String(fram.read32(500)) + "&Channel=1";// + String(fram.read32(10000)) + "&value3=" + String(fram.read32(11000)) + "&value4=" + String(fram.read32(12000)) + "&value5=" + String(fram.read32(13000)) + "";
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
}
