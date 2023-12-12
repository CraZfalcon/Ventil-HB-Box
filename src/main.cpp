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

#define USE_VENTIL_SSL // Uses Ventil SSL certificate for oms.ventil.nl 
#define LOCATION_ENABLED // Uncomment to enable GPS location
#define TEST //Uncomment to test with test API

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define GSM_PIN "0000"

#include <TinyGsmClient.h>
#include <FRAM.h>
#include <Wire.h>
#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ssl_cert.h>

const int  machinenr  = 1;
const char apn[]      = "chili";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "0000"; 
const char server[]    = "oms.ventil.nl";

// Location variables for GPS & API response global variable
float lat = 0, lon = 0;
String  responseCode = "";

static unsigned long REFRESH_INTERVAL = 100000;
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
  //REFRESH_INTERVAL = REFRESH_INTERVAL * 60 * 60 * 1000; // Convert miliseconds to hours

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
  // button7.begin(BUTTON_7_PIN);
  // button7.setPressedHandler(pressed);
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
  //modem.init();

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) { modem.simUnlock(simPIN); }

  Serial.println(int(fram.begin(0x50)), HEX);
}

void getGPSLocation(){
  Serial.println("\nGPS data:");
  modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    Serial.println("SGPIO=0,4,1,1 false");
  } else Serial.println("SGPIO=0,4,1,1 true");
  modem.enableGPS();

  if(modem.getGsmLocation(&lon, &lat)) {
    Serial.println("GPS raw: " + modem.getGsmLocationRaw());
    Serial.println("Latitude, Longitude");
    Serial.println(String(lat, 7) + ", " + String(lon, 7));
  } else Serial.println("Failed to get GSM location");
  
  modem.sendAT("+SGPIO=0,4,1,0");
  if (modem.waitResponse(10000L) != 1) {
    Serial.println("SGPIO=0,4,1,0 false");
  } else Serial.println("SGPIO=0,4,1,0 true");
  modem.disableGPS();

}

void sendpulseStringSecure(){
  Serial.println("\n\n\nStarting connection to GPRS network...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    Serial.println("Successfully connected to GPRS network");
    Serial.println("Getting GPS data...");

    #ifdef LOCATION_ENABLED
      getGPSLocation();
    #endif

    Serial.println("\nStarting connection to server...");

    #ifndef TEST
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
      + "&Channel16=" + String(fram.read32(25000))
      #ifdef LOCATION_ENABLED
        + "&Latitude="  + String(lat, 7)
        + "&Longitude=" + String(lon, 7)
      #endif    
      );

      responseCode = "NULL";
      responseCode = https.responseBody();

      if(responseCode != "NULL") {
        Serial.println("\nresponse: " + responseCode + "\n");
        lastRefreshTime += REFRESH_INTERVAL;
      }
      else{
        Serial.println("error: " + https.responseStatusCode());
      }

    #endif
    #ifdef TEST
      /*https.get("/api/MachinePartOperations?HBBoxNumber=1"
      "&Channel1=1&Channel2=1&Channel3=1&Channel4=1&Channel5=1&Channel6=1"
      "&Channel7=1&Channel8=1&Channel9=1&Channel10=1&Channel11=1&Channel12=1"
      "&Channel13=1&Channel14=1&Channel15=1&Channel16=1&Latitude=1&Longitude=1");*/

      String testLink = "/api/MachinePartOperations?"
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
      + "&Channel16=" + String(fram.read32(25000))
      #ifdef LOCATION_ENABLED
        + "&Latitude="  + String(lat, 7)
        + "&Longtitude=" + String(lon, 7)
      #endif
      ;

      https.get(testLink);
      Serial.println("Successfully connected to server, response:" + String(https.responseBody()));

      Serial.println("\nAPI request used: https://oms.ventil.nl:8081" + testLink);

      Serial.println("\n\nStored values:\n"
      "https response: " + String(https.responseBody()) + "\n"
      + "HBBoxNumber: " + String(fram.read32(500))   + "\n"
      + "Channel 1: "  + String(fram.read32(10000)) + "\n"
      + "Channel 2: "  + String(fram.read32(11000)) + "\n"
      + "Channel 3: "  + String(fram.read32(12000)) + "\n"
      + "Channel 4: "  + String(fram.read32(13000)) + "\n"
      + "Channel 5: "  + String(fram.read32(14000)) + "\n"
      + "Channel 6: "  + String(fram.read32(15000)) + "\n"
      + "Channel 7: "  + String(fram.read32(16000)) + "\n"
      + "Channel 8: "  + String(fram.read32(17000)) + "\n"
      + "Channel 9: "  + String(fram.read32(18000)) + "\n"
      + "Channel 10: " + String(fram.read32(19000)) + "\n"
      + "Channel 11: " + String(fram.read32(20000)) + "\n"
      + "Channel 12: " + String(fram.read32(21000)) + "\n"
      + "Channel 13: " + String(fram.read32(22000)) + "\n"
      + "Channel 14: " + String(fram.read32(23000)) + "\n"
      + "Channel 15: " + String(fram.read32(24000)) + "\n"
      + "Channel 16: " + String(fram.read32(25000)) + "\n"
      #ifdef LOCATION_ENABLED
        + "Latitude: "  + String(lat, 7) + "\n"
        + "Longitude: " + String(lon, 7) + "\n"
      #endif
      + "\n\n");

      lastRefreshTime += REFRESH_INTERVAL;
    #endif

    https.stop();
    Serial.println("Disconnected from server");
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }

  #ifndef TEST
    if (responseCode == "1") { Serial.println("Responsecode = 1, returning to normal operation"); lastRefreshTime += REFRESH_INTERVAL}
    else if (responseCode = "number / response code") { /*function*/ }
    else if (responseCode = "number / response code") { /*function*/ }
    else if (responseCode = "number / response code") { /*function*/ }
    else if (responseCode = "number / response code") { /*function*/ }
    else if (responseCode = "number / response code") { /*function*/ }
    else if (responseCode = "number / response code") { /*function*/ }
    else { Serial.println("Responsecode not recognized"); } // This should never happen, but it's here just in case
  #endif

}

void loop() {
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) {
    sendpulseStringSecure();
  }
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  button5.loop();
  button6.loop();
  // button7.loop();
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