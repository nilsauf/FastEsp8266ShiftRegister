#ifndef FAST_ESP_SHIFT_REGISTER
#define FAST_ESP_SHIFT_REGISTER

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

class FastEsp8266ShiftRegister
{
public:
    /**
     * @brief Construct a new FastEsp8266ShiftRegister object
     * 
     * @param dataPin The pin number of the data pin
     * @param clockPin The pin number of the clock pin
     * @param latchPin The pin number of the latch pin
     * @param shiftRegisterCount the count of chained hardware registers
     */
    FastEsp8266ShiftRegister(const uint8_t dataPin, const uint8_t clockPin, const uint8_t latchPin, const uint8_t shiftRegisterCount = 1)
    {
        // Generating the bit masks for the pins
        this->_dataPinMask = digitalPinToBitMask(dataPin);
        this->_clockPinMask = digitalPinToBitMask(clockPin);
        this->_latchPinMask = digitalPinToBitMask(latchPin);

        // Storing the count of chained shift registers
        this->_shiftRegisterCount = shiftRegisterCount;

        // Setting the pins to output mode
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(latchPin, OUTPUT);
    }

    /**
     * @brief Shifts all values out and triggers the latch pin to output all bits
     * 
     * @param newValues An array of register values, needs to have a length of shiftRegisterCount (see constructor)
     */
    void shiftAllOutAndUpdate(uint8_t *newValues)
    {
        for (uint8_t shiftRegisterIndex = this->_shiftRegisterCount; shiftRegisterIndex > 0; shiftRegisterIndex--)
        {
            this->shiftOut(newValues[shiftRegisterIndex - 1]);
        }
        this->update();
    }

    /**
     * @brief Shifts out the given value
     * 
     * @param newValue the value to be shifted out
     */
    void shiftOut(uint8_t newValue)
    {
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
        {
            // Setting the direct output mask to be faster than digitalWrite
            SET_PERI_REG_MASK(!!(newValue & (1 << (7 - bitIndex))) ? 0x60000304 : 0x60000308, this->_dataPinMask);
            SET_PERI_REG_MASK(0x60000304, this->_clockPinMask);
            SET_PERI_REG_MASK(0x60000308, this->_clockPinMask);
        }
    }

    /**
     * @brief Triggers the latch pin to output all current bits
     */
    void update()
    {
        // Setting the direct output mask to be faster than digitalWrite
        SET_PERI_REG_MASK(0x60000304, this->_latchPinMask);
        SET_PERI_REG_MASK(0x60000308, this->_latchPinMask);
    }

    /**
     * @brief Get the count of chained shift registers
     */
    uint8_t GetRegisterCount()
    {
        return this->_shiftRegisterCount;
    }

private:
    uint16_t _dataPinMask;
    uint16_t _clockPinMask;
    uint16_t _latchPinMask;
    uint8_t _shiftRegisterCount;
};

#endif