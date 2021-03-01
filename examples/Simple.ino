#include <Arduino.h>
#include <FastEsp8266ShiftRegister.hpp>

#define SHIFT_COUNT 1

FastEsp8266ShiftRegister shiftRegister(13, 14, 15, SHIFT_COUNT);

uint8_t firstValue = 170;
uint8_t secondValue = 85;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:
    shiftRegister.shiftAllOutAndUpdate(&firstValue);
    Serial.print(".");
    delay(500);
    shiftRegister.shiftAllOutAndUpdate(&secondValue);
    Serial.print(".");
    delay(500);
}