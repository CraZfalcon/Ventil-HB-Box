//const char server[] = "enisadesign.nl";
//const char resource[] = "/ventil/post-data.php";
//const int  port = 80;


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

//uint32_t start;
//uint32_t stop;
//uint32_t sizeInBytes = 0;

//const uint32_t I2C_Freq = 400000;
//const uint32_t I2C_Freq_2 = 100000;

/*#define uS_TO_S_FACTOR 1000000UL   // Conversion factor for micro seconds to seconds
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
*/




/*
void sendpulseStringWiFi() {
  Serial.println("Connecting to WiFi");
  if (!WiFi.begin("Ventil_Wireless", "Vc25Srv!!")) {
    Serial.println("Failed to connect to network");
  }
  else {
    //https://oms.ventil.nl:8081/api/MachinePartOperations?HBBoxNumber=1&Channnel=1
    String httpRequestData = "?HBBoxNumber=" + String(fram.read32(500)) + "&Channnel=1";
    Serial.print("");
    Serial.println(httpRequestData);
    Serial.println("");
    http.get("/" + httpRequestData);
    //int statusCode = http.responseStatusCode();
    String response = http.responseBody();
    //Serial.println("Status code: " + statusCode);
    Serial.println("Response: " + response);
    Serial.println("");
  
    http.stop();
    Serial.println(F("Server disconnected"));
    WiFi.disconnect();
    Serial.println(F("Network disconnected"));
  }
}
*/



/*
void loop2(){
  while (millis() - lastRefreshTime < REFRESH_INTERVAL)
  {
    if      (btn == button1) { fram.write32(10000, (fram.read32(10000))+1); }
    else if (btn == button2) { fram.write32(11000, (fram.read32(11000))+1); }
    else if (btn == button3) { fram.write32(12000, (fram.read32(12000))+1); }
    else if (btn == button4) { fram.write32(13000, (fram.read32(13000))+1); }
  }
  Serial.println("Connecting to WiFi");
  if (!WiFi.begin("Ventil_Wireless", "Vc25Srv!!")) {
    Serial.println("Failed to connect to network");
  }
  else {
    String httpRequestData = "?HBBoxNumber=" + String(fram.read32(500)) + "&Channnel=1";// + String(fram.read32(10000)) + "&value3=" + String(fram.read32(11000)) + "&value4=" + String(fram.read32(12000)) + "&value5=" + String(fram.read32(13000)) + "";
    Serial.print("");
    Serial.println(httpRequestData);
    Serial.println("");
    http.get("/" + httpRequestData);
    //int statusCode = http.responseStatusCode();
    String response = http.responseBody();
    //Serial.println("Status code: " + statusCode);
    Serial.println("Response: " + response);
    Serial.println("");
  
    http.stop();
    Serial.println(F("Server disconnected"));
    WiFi.disconnect();
    Serial.println(F("Network disconnected"));
  }
}
*/






























/*
// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, Serial);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif
*/

/*
void sendpulseString() {
  Serial.print("Connecting to APN: ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS network");
  }
  else {
    Serial.print("Connecting to ");
    Serial.print(server);
    if (!client.connect(server, port)) {
      Serial.println("Failed to connect to server");
    }
    else {
      //  Serial print counter values
      long machinenr = fram.read32(500);
      long sensor_1 = fram.read32(10000);
      long sensor_2 = fram.read32(11000);
      long sensor_3 = fram.read32(12000);
      long sensor_4 = fram.read32(13000);
      Serial.print(machinenr);
      Serial.println(String(fram.read32(10000)) + "," + String(fram.read32(11000)) + "," + String(fram.read32(12000)) + "," + String(fram.read32(13000)));
      // Making an HTTP POST request
      Serial.println("Performing HTTP POST request...");
      String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(fram.read32(500)) + "&value2=" + String(fram.read32(10000)) + "&value3=" + String(fram.read32(11000)) + "&value4=" + String(fram.read32(12000)) + "&value5=" + String(fram.read32(13000)) + "";
      Serial.println("API Data: " + httpRequestData);
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
          Serial.print(c);
          timeout = millis();
        }
      }
      Serial.println("");
      
      // Close client and disconnect
      client.stop();
      Serial.println(F("Server disconnected"));
      modem.gprsDisconnect();
      Serial.println(F("GPRS disconnected"));
      httpRequestData = "";
    }
  }
}
*/

/*
void pressed(Button2& btn) {
  if (btn == button1) {
    long sensor_1 = fram.read32(10000);
    fram.write32(10000, sensor_1+1);
  } else if (btn == button2) {
    long sensor_2 = fram.read32(11000);
    fram.write32(11000, sensor_2+1);
  } else if (btn == button3) {
    long sensor_3 = fram.read32(12000);
    fram.write32(12000, sensor_3+1); 
  } else if (btn == button4) {
    long sensor_4 = fram.read32(13000);
    fram.write32(13000, sensor_4+1);
  }
}
*/