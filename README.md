# ESP8266 Alive Counter
Power failure monitoring using an ESP8266, Thingspeak, RaspberryPi and Simplepush

## Description
The ESP8266 creates a new entry in the Thingspeak channel every x seconds (alive counter).
The RaspberryPi checks every y minutes if the last entry in the Thingspeak channel is not older than z seconds.
If the entry is older, a push message is sent using Simplepush.