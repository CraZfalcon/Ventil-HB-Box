[![PlatformIO Registry](https://badges.registry.platformio.org/packages/digitaldragon/library/SSLClient.svg)](https://registry.platformio.org/libraries/digitaldragon/SSLClient)

![arduino-library-badge](https://www.ardu-badge.com/badge/GovoroxSSLClient.svg)   

<!-- [Arduino Libraries Log](https://downloads.arduino.cc/libraries/logs/github.com/govorox/SSLClient/) -->

### Now updated on PlatformIO registry as digitaldragon/SSLClient@1.1.7
### Updated on Arduino Libraries registry to digitaldragon/GovoroxSSLClient@1.1.7

# SSLClient Arduino library using *mbedtls* functions
The SSLClient class implements support for secure connections using TLS (SSL). It Provides a transparent SSL wrapper over existing transport object of a **Client** class.
Based on the [WiFiClientSecure](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiClientSecure) for Arduino/ESP32.
Designed and tested on ESP32, but should be portable to other platforms.

Can be used to provide connectivity to AWS IoT via GSM client (for example **TinyGSM**)

Example usage:

```
TinyGsmClient transport(modem);
SSLClient secure(&transport);

...

// Configure certificates to be used
secure.setCACert(AWS_CERT_CA);
secure.setCertificate(AWS_CERT_CRT);
secure.setPrivateKey(AWS_CERT_PRIVATE);

...

// Establish a generic secure connection
// secure.connect(TLS_ENDPOINT, TLS_PORT);

// or connect to the MQTT broker on the AWS endpoint
MQTTClient mqtt = MQTTClient(256);
mqtt.begin(AWS_IOT_ENDPOINT, 8883, secure);
  
```
