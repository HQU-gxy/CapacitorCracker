#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WS_Server.h>
#include "config.h"
#include "relay.h"
#include <ArduinoJson.h>

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
}
u32 t0 = 0;
void loop()
{

  // put your main code here, to run repeatedly:
  ws_server.loop();
  if (millis() - t0 >= 500)
  {
    
#ifdef DEBUG
    Serial.print("Voltage: ");
    Serial.println(analogRead(A0));
#endif
    digitalWrite(LED_LOW_BAT, (analogRead(A0) <= 652)); // about 7V

    sendInfo(255, VOLTAGE);
    //sendInfo(255, RELAY);
    sendInfo(255, TIMELENGTH);

    Relay::checkExpire();
    t0 = millis();
  }
}