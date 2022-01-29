#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <WidgetRTC.h>

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
