#include <WS_Server.h>
#include <ArduinoJson.h>
#include "config.h"
#include "relay.h"
void wsEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;

    case WStype_CONNECTED:
    {
        IPAddress ip = ws_server.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
        // send message to client
        ws_server.sendTXT(num, "Connected");
        break;
    }

    case WStype_TEXT:
    {
#ifdef DEBUG
        Serial.printf("[%u] get Text: %s\n", num, payload);
#endif
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        parseCmd(doc, num);
        break;
    }

    default:
        break;
    }
}

void parseCmd(JsonDocument &doc, u8 num)
{
    String cmd = doc["cmd"];
    DynamicJsonDocument responseJson(1024);
    char response[1024];

    if (cmd.equals("get_voltage"))
    {
        sendInfo(num, VOLTAGE);
        return;
    }

    if (cmd.equals("fuck_relay"))
    {
        int relayNum = doc["args"]["relay_num"];
        Relay *relay = Relay::getRelay(relayNum);
        if (relay == NULL)
        {
            responseJson["status"] = 1;
            serializeJson(responseJson, response);
            ws_server.sendTXT(num, response);
            return;
        }
        if (relay->isOn())
        {
            responseJson["status"] = 2;
            serializeJson(responseJson, response);
            ws_server.sendTXT(num, response);
            return;
        }

        relay->fuck();
        responseJson["status"] = 0;
        serializeJson(responseJson, response);
        ws_server.sendTXT(num, response);
        return;
    }

    if (cmd.equals("get_relays_status"))
    {
        sendInfo(num, RELAY);
        return;
    }

    if (cmd.equals("set_time_length"))
    {
        int timeLength = doc["args"]["time_length"];
        Relay::setTimeLength(timeLength);
        responseJson["status"] = 0;
        serializeJson(responseJson, response);
        ws_server.sendTXT(num, response);
        return;
    }

    if (cmd.equals("get_time_length"))
    {
        sendInfo(num, TIMELENGTH);
        return;
    }
}

void sendInfo(u8 num, InfoCategory category)
{
    DynamicJsonDocument responseJson(1024);
    char response[1024];
    switch (category)
    {
    case InfoCategory::VOLTAGE:
    {
        responseJson["voltage"] = analogRead(A0);
        break;
    }
    case InfoCategory::RELAY:
    {
        StaticJsonDocument<JSON_ARRAY_SIZE(16)> relaysStatus;
        for (int i = 0; i < Relay::getRelaysCount(); i++)
        {
            relaysStatus.add((int)(Relay::getRelay(i)->isOn()));
        }
        responseJson["relays_status"] = relaysStatus;
        break;
    }

    case InfoCategory::TIMELENGTH:
    {
        responseJson["time_length"] = Relay::getTimeLength();
        break;
    }

    default:
        break;
    }
    serializeJson(responseJson, response);
    if (num == 255)
    {
        ws_server.broadcastTXT(response);
        return;
    }
    ws_server.sendTXT(num, response);
}