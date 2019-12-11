#include "Arduino.h"
#include "LedIndicator.h"

LedIndicator::LedIndicator(int pin){
    _pin = pin;
    pinMode(pin, OUTPUT);
}

void LedIndicator::blink(int mode){
    int times;
    switch (mode)
    {
    case PANIC:
        times = 10;
        break;
    case TX:
        times = 3;
        break;
    case WAITING:
        times = 1;
        break;
    
    default:
        times = 0;
        break;
    }
    
    for (size_t i = 0; i < times; i++)
    {
        digitalWrite(_pin, HIGH);
        delay(mode);
        digitalWrite(_pin, LOW);
        delay(mode);
    }
}
void LedIndicator::off(){
    digitalWrite(_pin, LOW);
}
void LedIndicator::on(){
    digitalWrite(_pin, HIGH);
}