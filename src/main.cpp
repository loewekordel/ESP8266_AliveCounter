#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "types.h"
#include "debug.h"
#include "ThingSpeakWrpr.h"

#define INTERVAL  (60000)   // Time to sleep (in milliseconds):

#define WIFI_SSID "A1-B5B769"
#define WIFI_PASSWORD "0BC30981E6"
#define WIFI_MAXRECONATTEMPTS (5)

bool enaSendData = false;
static uint8_t cntrWifiRecon = 0u;

Ticker wifiReconnectTimer;
WiFiEventHandler wifiConnectedHandler;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
void connectToWifi();
static void onWifiConnected(const WiFiEventStationModeConnected& event);
void onWifiGotIp(const WiFiEventStationModeGotIP& event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);

void connectToWifi() {
  LOG("[WiFi] Connecting to Wi-Fi ");
  LOGLN(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

static void onWifiConnected(const WiFiEventStationModeConnected& event) {
  LOGLN("[WiFi] Wifi connected");
}

void onWifiGotIp(const WiFiEventStationModeGotIP& event) {
  LOG("[WiFi] Got IP: ");
  LOGLN(WiFi.localIP());
  enaSendData = true;
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  LOG("[WiFi] Wifi disconnected. Reason: ");
  LOGLN(event.reason);

  switch (event.reason) {
    case WIFI_DISCONNECT_REASON_ASSOC_LEAVE:
      break;
    default:
      if (cntrWifiRecon < WIFI_MAXRECONATTEMPTS) {
        LOGLN("[WiFi] Try to reconnect");
        cntrWifiRecon++;
      } else {
        LOGLN("[WiFi] Max. number of reconnect attempts reached. Shutdown and try later");
        ESP.deepSleep((INTERVAL * 10e5), WAKE_RF_DEFAULT);    // Sleep for INTERVAL seconds
      }
      break;
  };
}

void setup() {
  Serial.begin(115200);
  LOGLN();
  LOGLN();

  wifiConnectedHandler = WiFi.onStationModeConnected(onWifiConnected);
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiGotIp);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  ThingSpeakWrpr_init();
  connectToWifi();
}

void loop() {
  static unsigned long currentMillis = 0;
  static unsigned long previousMillis = 0;

  if (enaSendData) {
    currentMillis = millis();
    if(currentMillis - previousMillis > INTERVAL) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;  
      ThingSpeakWrpr_sendData(1);
    }
  }
}