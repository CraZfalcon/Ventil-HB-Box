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

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define GSM_PIN "0000"

#include <TinyGsmClient.h>
#include <FRAM.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <SSLClient.h>
#include <ArduinoHttpClient.h>

const char apn[]      = "chili";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "0000"; 
const char server[]    = "oms.ventil.nl";

const int  machinenr  = 1;

static const unsigned long REFRESH_INTERVAL = 10000;
static unsigned long lastRefreshTime = 0;

const char ssl_cert[] PROGMEM =
"-----BEGIN CERTIFICATE-----\n" \
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n" \
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n" \
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n" \
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n" \
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n" \
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n" \
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n" \
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n" \
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n" \
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n" \
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n" \
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n" \
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n" \
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n" \
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n" \
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n" \
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n" \
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n" \
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n" \
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n" \
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n" \
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n" \
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n" \
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n" \
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n" \
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n" \
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n" \
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n" \
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n" \
"jjxDah2nGN59PRbxYvnKkKj9\n" \
"-----END CERTIFICATE-----\n";

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
  //modem.restart();
  modem.init();

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

    https.get("/api/MachinePartOperations?"
    "HBBoxNumber="  + String(fram.read32(500))
    + "&Channel1="  + String(fram.read32(10000))
    + "&Channel2="  + String(fram.read32(11000))
    + "&Channel3="  + String(fram.read32(12000))
    + "&Channel4="  + String(fram.read32(13000))
    + "&Channel5="  + String(fram.read32(14000))
    + "&Channel6="  + String(fram.read32(15000))
    + "&Channel7="  + String(fram.read32(16000))
    + "&Channel8="  + String(fram.read32(17000))
    + "&Channel9="  + String(fram.read32(18000))
    + "&Channel10=" + String(fram.read32(19000))
    + "&Channel11=" + String(fram.read32(20000))
    + "&Channel12=" + String(fram.read32(21000))
    + "&Channel13=" + String(fram.read32(22000))
    + "&Channel14=" + String(fram.read32(23000))
    + "&Channel15=" + String(fram.read32(24000))
    + "&Channel16=" + String(fram.read32(25000)));

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