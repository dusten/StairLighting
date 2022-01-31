#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <WidgetRTC.h>

#include <Wire.h>
#include "vl53l0x.h"

VL53L0X sensor;
#define HIGH_ACCURACY
int myLed = 5;
int intPin = 14;

bool newData = false;

uint32_t delt_t = 0, count = 0, sumCount = 0;  // used to control display output rate
float deltat = 0.0f, sum = 0.0f;          // integration interval for both filter schemes
uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;                         // used to calculate integration interval


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

#define HOSTNAME "Stairs"

const char* ssid = STASSID;
const char* pass = STAPSK;

BlynkTimer timer;
WidgetRTC rtc;

WidgetLED statusLED(V10);
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#E6E600"
#define BLYNK_ORANGE    "#E68A00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

#define stair01
#define stair02
#define stair03
#define stair04
#define stair05
#define stair06
#define stair07
#define stair08
#define stair09
#define stair10
#define stair11
#define stair13
#define stair14
#define stair15
#define stair16


void ledOn(int stairNo, int brightness, int color)
{

}

void ledOff(int stairNo)
{
  
}

void fadeLedOn(int stairNo, int brightness, int color)
{
  
}

void fadeLedOff(int stairNo)
{
  
}

void requestTime()
{
  Blynk.sendInternal("rtc", "sync");
}
void setup() {

  #ifndef BLYNK_PRINT
  Serial.begin(115200); //Not really needed, Sets 0 the serial debug port.
  #endif
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,160), 8080);  //connects to the blynk server using the credentals from above.

  rtc.begin();
  Blynk.syncAll();

  ArduinoOTA.setHostname(HOSTNAME);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  #ifndef BLYNK_PRINT
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
  timer.setInterval(3600000L, requestTime);
}
void loop()
{
  Blynk.run();

  timer.run();
  
  ArduinoOTA.handle();
}

void myinthandler()
{
  newData = true; // set the new data ready flag to true on interrupt
}

// I2C scan function
void I2Cscan()
{
// scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}
