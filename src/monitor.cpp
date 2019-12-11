/*
    Pack monitor intended to be Attiny85 dev board
*/
#include <Arduino.h>
#include <LedIndicator.h>
#include <SoftwareSerial.h>

#include <avr/power.h>

#include "configSlave.h"
#include "SerialComm.h"
#include "Protocol.h"

SoftwareSerial serial(RX_PIN, TX_PIN);
LedIndicator led(LEDPIN);
SerialComm comm(serial);
Protocol protocol(&comm);

long readVcc()
{
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
    #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);
    #else
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #endif

    delay(2);            // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC))
        ; // measuring

    uint8_t low = ADCL;  // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    long result;
    result = (high << 8) | low;
    result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result;              // Vcc in millivolts
}

long readTemp()
{
    float tempReading = analogRead(TMP_PIN); // Get the reading from the sensor
    // Converts reading from sensor into millivolts using reference voltage
    float voltageRead = tempReading * (1100/1024); 
    float temperatureC = (voltageRead - 500)/10; // offset and conversion 10mv to 1 degree C
    return temperatureC;
}

void setup()
{
    if (F_CPU != 8000000)
    {
        led.blink(PANIC);
        abort();
    }
    serial.begin(9600);
    led.blink(WAITING);
    pinMode(TMP_PIN,INPUT);
    analogReference(INTERNAL);
}

void loop()
{
    uint32_t now = millis();
    if (comm.update(now))
    {
        uint8_t *data = comm.data();
        packet_t packet;
        if (protocol.handle(data, &packet))
        {
            if (packet.reg == REG_VOLTAGE)
            {
                uint16_t voltage = readVcc();
                protocol.respond(&packet, voltage);
            }
            else
            {
                uint16_t temp = readTemp();
                protocol.respond(&packet, temp);
            }
            
            /*
            switch (packet.reg)
            {
            case REG_VOLTAGE:
                // Get VCC for now mocking it up
                uint16_t voltage = readVcc();
                protocol.respond(&packet, voltage);
                break;

            case REG_TEMP:
                //Read Temp
                uint16_t temp = 36;
                protocol.respond(&packet, temp);
                break;
            }
            */
        }
    }
}