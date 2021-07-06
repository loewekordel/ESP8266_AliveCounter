#include <ESP8266WiFi.h>
#include "ThingSpeakWrpr.h"
#include "debug.h"

WiFiClient client;

const long ThingSpeakWrpr_chNr = 1435107;
const char*  ThingSpeakWrpr_apiKey = "I7IVZRKLZP33XAQ0";

void ThingSpeakWrpr_init(void) {
  ThingSpeak.begin(client);
}

int ThingSpeakWrpr_sendData(int data) {
  int rtn = 0;

  // ThingSpeak.begin(client);
  rtn = ThingSpeak.setField(1, data);
  if (rtn != TS_OK_SUCCESS) {
    LOG("Error: ThingSpeak error number: ");
    LOGLN(rtn);    
    return rtn;
  }
  rtn = ThingSpeak.writeFields(ThingSpeakWrpr_chNr, ThingSpeakWrpr_apiKey);
  if (rtn != TS_OK_SUCCESS) {
    LOG("Error: ThingSpeak error number: ");
    LOGLN(rtn);
    return rtn;
  }

  LOGLN("[TS] Data sent");
  return 0;
}