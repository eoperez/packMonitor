/*
    LedIndicator.h - Library for flashing Led based on need.
*/
#ifndef __LEDINDICATOR_H__
#define __LEDINDICATOR_H__

#include "Arduino.h"
#define PANIC 150
#define WAITING 350
#define TX 250

class LedIndicator
{
private:
    int _pin;
public:
    LedIndicator(int pin);
    void blink(int mode);
    void off();
    void on();
};
#endif