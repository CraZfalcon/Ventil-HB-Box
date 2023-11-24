#include <Button2.h>
#define BUTTON_1_PIN  23
#define BUTTON_2_PIN  34
#define BUTTON_3_PIN  35
#define BUTTON_4_PIN  36
Button2 button1, button2, button3, button4;

#define USE_VENTIL_SSL
//#define TEST //Uncomment to test with test API

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define GSM_PIN "0000"

#include <TinyGsmClient.h>
#include <FRAM.h>
#include <Wire.h>
#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ssl_cert.h>

const char apn[]      = "chili";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "0000"; 
const char server[]    = "oms.ventil.nl";

const int  machinenr  = 1;

static const unsigned long REFRESH_INTERVAL = 100000;
static unsigned long lastRefreshTime = 0;

// I2C for SIM7000G(0) & FRAM(1) (to keep it running when powered from battery)
TwoWire I2CModem = TwoWire(0);
TwoWire I2CFRAM = TwoWire(1);
FRAM fram(&I2CFRAM);

#define SerialAT Serial1
TinyGsm modem(SerialAT);
TinyGsmClient sslclient(modem);
SSLClient secure_layer(&sslclient);
HttpClient https = HttpClient(secure_layer, server, 8081);


bool setPowerBoostKeepOn(int en){
  I2CModem.beginTransmission(0x75);
  I2CModem.write(0x00);
  if (en) { I2CModem.write(0x37); } // Set bit1: 1 enable 0 disable boost keep on
  else    { I2CModem.write(0x35); } // 0x37 is default reg value
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

  secure_layer.setCACert(ssl_cert);

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
  //modem.init();

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) { modem.simUnlock(simPIN); }

  Serial.println(int(fram.begin(0x50)), HEX);

}

void sendpulseStringSecure(){
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    Serial.println("\n\n\nSuccessfully connected to GPRS network");
    Serial.println("Starting connection to server...");


  #ifndef TEST
    https.get("/api/MachinePartOperations?"
    "HBBoxNumber="  + String(fram.read32(500))
    + "&Channel1="  + String(fram.read32(10000))
    + "&Channel2="  + String(fram.read32(11000))
    + "&Channel3="  + String(fram.read32(12000))
    + "&Channel4="  + String(fram.read32(13000)));
  #endif
  #ifdef TEST
      https.get("/api/MachinePartOperations?HBBoxNumber=1&Channel=1");
  #endif
    Serial.println("\nresponse: " + https.responseBody() + "\n");

    https.stop();
    Serial.println("Disconnected from server");
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }

}

void loop() {
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) {
    lastRefreshTime += REFRESH_INTERVAL;
    sendpulseStringSecure();
  }
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
}