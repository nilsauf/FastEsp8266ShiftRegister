#ifndef FAST_ESP_SHIFT_REGISTER
#define FAST_ESP_SHIFT_REGISTER

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

class FastEsp8266ShiftRegister
{
public:
    FastEsp8266ShiftRegister(const uint8_t dataPin, const uint8_t clockPin, const uint8_t latchPin, const uint8_t shiftRegisterCount = 1)
    {
        this->_dataPinMask = (uint16_t)1 << dataPin;
        this->_clockPinMask = (uint16_t)1 << clockPin;
        this->_latchPinMask = (uint16_t)1 << latchPin;
        this->_shiftRegisterCount = shiftRegisterCount;

        this->_dataPin = dataPin;
        this->_clockPin = clockPin;
        this->_latchPin = latchPin;

        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
    }

    void shiftAllOutAndUpdate(uint8_t *newValues)
    {
        for (uint8_t shiftRegisterIndex = this->_shiftRegisterCount - 1; shiftRegisterIndex >= 0; shiftRegisterIndex--)
        {
            this->shiftOut(newValues[shiftRegisterIndex]);
        }
        this->update();
    }

    void shiftOut(uint8_t newValue)
    {
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
        {
            SET_PERI_REG_MASK(!!(newValue & (1 << (7 - bitIndex))) ? 0x60000304 : 0x60000308, this->_dataPinMask);
            SET_PERI_REG_MASK(0x60000304, this->_clockPinMask);
            SET_PERI_REG_MASK(0x60000308, this->_clockPinMask);
        }
    }

    void update()
    {
        SET_PERI_REG_MASK(0x60000304, this->_latchPinMask);
        SET_PERI_REG_MASK(0x60000308, this->_latchPinMask);
    }

    uint8_t *getAll()
    {
        uint8_t dataIn[this->_shiftRegisterCount];
        for (uint8_t valueIndex = 0; valueIndex < this->_shiftRegisterCount; valueIndex++)
        {
            dataIn[valueIndex] = shiftIn(this->_dataPin, this->_clockPin, MSBFIRST);
        }
        return dataIn;
    }

private:
    uint16_t _dataPinMask;
    uint16_t _clockPinMask;
    uint16_t _latchPinMask;
    uint8_t _shiftRegisterCount;

    uint8_t _dataPin;
    uint8_t _clockPin;
    uint8_t _latchPin;
};

#endif