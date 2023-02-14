#ifndef WS_H
#define WS_H
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

typedef enum
{
    RELAY,
    VOLTAGE,
    TIMELENGTH
} InfoCategory;

extern WebSocketsServer ws_server;
void wsEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void parseCmd(JsonDocument &doc,u8 num);
void sendInfo(u8 num,InfoCategory category);
#endif // !WS_H
