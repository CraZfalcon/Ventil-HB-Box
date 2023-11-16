#include <Button2.h>
#define BUTTON_1_PIN  23
#define BUTTON_2_PIN  34
#define BUTTON_3_PIN  35
#define BUTTON_4_PIN  36
Button2 button1, button2, button3, button4;

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define GSM_PIN "0000"

#include <TinyGsmClient.h>
#include <FRAM.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <WiFi.h>
//#include <HttpClient.h>
#include <WifiClientSecure.h>

const char apn[]      = "chili";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "0000"; 
const char* server    = "oms.ventil.nl";
const char* ssid      = "Ventil_Wireless";
const char* password  = "Vc25Srv!!";


const int  machinenr  = 1;

static const unsigned long REFRESH_INTERVAL = 10000;
static unsigned long lastRefreshTime = 0;

const char* ssl_cert =
"-----BEGIN CERTIFICATE-----\n"
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n"
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n"
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n"
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n"
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n"
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n"
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n"
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n"
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n"
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n"
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n"
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n"
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n"
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n"
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n"
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n"
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n"
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n"
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n"
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n"
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n"
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n"
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n"
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n"
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n"
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n"
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n"
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n"
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n"
"jjxDah2nGN59PRbxYvnKkKj9\n"
"-----END CERTIFICATE-----\n";

// I2C for SIM7000G(0) & FRAM(1) (to keep it running when powered from battery)
TwoWire I2CModem = TwoWire(0);
TwoWire I2CFRAM = TwoWire(1);
FRAM fram(&I2CFRAM);

#define SerialAT Serial1
TinyGsm modem(SerialAT);
//HTTPClient http;
WiFiClientSecure client;


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

  // WiFi.begin("Ventil_Wireless", "Vc25Srv!!");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Start I2C communication (SDA, SCL, Frequency)
  I2CModem.begin(21, 22, 400000);
  I2CFRAM.begin(16, 17, 100000);

  Serial.println(String("IP5306 KeepOn ") + (bool(setPowerBoostKeepOn(1)) ? "OK" : "FAIL"));

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);

  client.setCACert(ssl_cert);

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
  esp_netif_init();

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) { modem.simUnlock(simPIN); }

  Serial.println(int(fram.begin(0x50)), HEX);

}

/*void sendpulseString() {
  Serial.println("Connecting to APN: " + String(apn));
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    String serverPath = "https://oms.ventil.nl:8081/api/MachinePartOperations?HBBoxNumber=" + String(fram.read32(500)) + "&Channel=1";
    //https://oms.ventil.nl/api/MachinePartOperations?HBBoxNumber= + String(fram.read32(500)) + "&Channel1=" + String(fram.read32(10000)) + "&Channel2=" + String(fram.read32(11000)) + "&Channel3=" + String(fram.read32(12000)) + "&Channel4=" + String(fram.read32(13000));
    // API adress will need to be changed. Current path is only for testing purposes.

    http.begin(serverPath.c_str());
    http.GET();
    if (http.getString() == "1") Serial.println(F("API request success"));
    // option to add more actions by adding "if (http.getString() == "{responsecode}") {responsecode action}"
    else Serial.println(F("API request failed"));

    http.end();
    Serial.println(F("Server disconnected"));
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }
}*/

void sendpulseStringSecure(){
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    Serial.println("\nSuccessfully connected to GPRS network\nStarting connection to server...");
    if (!client.connect(server, 8081))
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      String httpsPath = "https://oms.ventil.nl/api/MachinePartOperations?HBBoxNumber=1&Channel=1";
      client.println("GET " + httpsPath + " HTTP/1.0");
      client.println("Host: oms.ventil.nl");
      client.println("Connection: close");
      client.println();

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("headers received");
          break;
        }
      }
      // if there are incoming bytes available
      // from the server, read them and print them:
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }

      client.stop();
      Serial.println(F("\nHTTPS connection closed"));
    }
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }
}

void serverCall(){
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 8081))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    client.println("GET https://oms.ventil.nl/api/MachinePartOperations?HBBoxNumber=1&Channel=1 HTTP/1.0");
    client.println("Host: oms.ventil.nl");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void sendpulseStringSecureV2(){
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    Serial.println("\nSuccessfully connected to GPRS network");
    serverCall();
  }
  modem.gprsDisconnect();
  Serial.println(F("\nGPRS disconnected"));
}




void loop() {
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) {
    lastRefreshTime += REFRESH_INTERVAL;
    //sendpulseString();
    
    Serial.println("Sending request via GPRS:\n");
    sendpulseStringSecureV2();
    Serial.println("\n\n\n\n\n\n");
    Serial.println("Sending request via Wifi:\n");
    //serverCall();
    Serial.println("\n\n\n\n\n\n");
  }
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
}