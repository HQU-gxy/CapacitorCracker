#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WS_Server.h>
#include "config.h"
#include "relay.h"
#include <ArduinoJson.h>
#include <EEPROM.h>

Relay relay1(RELAY1, 1);
Relay relay2(RELAY2, 2);
Relay relay3(RELAY3, 3);
Relay relay4(RELAY4, 4);
Relay relay5(RELAY5, 5);
Relay relay6(RELAY6, 6);

WebSocketsServer ws_server(11451);

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_LOW_BAT, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
  ws_server.begin();
  ws_server.onEvent(wsEventHandler);
  EEPROM.begin(2);
  Relay::setTimeLength(EEPROM.read(0) + (EEPROM.read(1) << 8));
}
u32 t0 = 0;
void loop()
{

  // put your main code here, to run repeatedly:
  ws_server.loop();
  Relay::checkExpire();
  if (millis() - t0 >= 1000)
  {

#ifdef DEBUG
    Serial.print("Voltage: ");
    Serial.println(analogRead(A0));
#endif
    digitalWrite(LED_LOW_BAT, (analogRead(A0) <= 652)); // about 7V

    sendInfo(255, VOLTAGE);
    delay(200);
    sendInfo(255, RELAY);
    delay(200);
    sendInfo(255, TIMELENGTH);

    t0 = millis();
  }
}