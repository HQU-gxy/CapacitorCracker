#include <relay.h>
#include <Arduino.h>
#include "config.h"
uint32 Relay::_timeLength = 2000;
Relay *Relay::_relaysList[10];
uint8 Relay::_relaysCount = 0;

Relay::Relay(u8 pin, u8 num)
{
    _pin = pin;
    _num = num;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, 0);
    _isOn = false;
    _relaysList[_relaysCount] = this;
    _relaysCount++;
}

u8 Relay::getRelaysCount()
{
    return _relaysCount;
}

void Relay::fuck()
{
    if (!this->_isOn)
    {

#ifdef DEBUG
        Serial.print("Relay@pin ");
        Serial.print(this->_pin);
        Serial.println("switched on");
#endif

        digitalWrite(this->_pin, 1);
        this->_isOn = true;
        this->_onTime = millis();
    }
}

bool Relay::isOn()
{
    return this->_isOn;
}

void Relay::setTimeLength(u32 timeLength)
{
    Relay::_timeLength = timeLength;
}

u32 Relay::getTimeLength()
{
    return Relay::_timeLength;
}

void Relay::checkExpire()
{
    for (u8 i = 0; i < _relaysCount; i++)
    {
        if (_relaysList[i]->_isOn)
            if (_relaysList[i]->_onTime + _timeLength < millis())
            {

#ifdef DEBUG
                Serial.print("Relay@pin ");
                Serial.print(_relaysList[i]->_pin);
                Serial.println("switched off");
#endif

                digitalWrite(_relaysList[i]->_pin, 0);
                _relaysList[i]->_isOn = false;
            }
    }
}

Relay *Relay::getRelay(u8 num)
{
    for (u8 i = 0; i < _relaysCount; i++)
    {
        if (_relaysList[i]->_num == num)
            return _relaysList[i];
    }
    return NULL;
}