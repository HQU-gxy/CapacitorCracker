#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
class Relay
{
public:
    Relay(u8 pin,u8 num);
    void fuck();
    bool isOn();

    static void setTimeLength(u32 timeLength);
    static u32 getTimeLength();
    static void checkExpire();
    static Relay *getRelay(u8 num);
    static u8 getRelaysCount();
    u8 _num;
private:
    static u32 _timeLength;
    static Relay *_relaysList[10];
    static u8 _relaysCount;

    u8 _pin;
    
    bool _isOn = false;
    unsigned long _onTime = 0;
};

#endif // !RELAY_H