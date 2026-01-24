// Papilio RGB LED Library - WS2812B controller via Wishbone SPI
#ifndef PAPILIO_RGB_LED_H
#define PAPILIO_RGB_LED_H

#include <Arduino.h>
#include <WishboneSPI.h>

class PapilioRGBLed {
public:
    PapilioRGBLed(uint16_t baseAddress = 0x2000);
    
    bool begin();
    
    // Set LED color using 24-bit GRB value
    void setColor(uint32_t grbColor);
    
    // Set LED color using separate RGB components (0-255)
    void setColorRGB(uint8_t red, uint8_t green, uint8_t blue);
    
    // Check LED controller busy flag
    bool isBusy();
    
    // Get base address
    uint16_t getBaseAddress() const { return _baseAddress; }
    
    // Common colors in GRB format (dimmed to ~10% for examples)
    static const uint32_t COLOR_OFF;
    static const uint32_t COLOR_RED;
    static const uint32_t COLOR_GREEN;
    static const uint32_t COLOR_BLUE;
    static const uint32_t COLOR_YELLOW;
    static const uint32_t COLOR_CYAN;
    static const uint32_t COLOR_MAGENTA;
    static const uint32_t COLOR_WHITE;
    static const uint32_t COLOR_ORANGE;
    static const uint32_t COLOR_PURPLE;

private:
    uint16_t _baseAddress;
    
    // Wishbone register offsets (relative to base address)
    static const uint8_t REG_LED_GREEN = 0x00;
    static const uint8_t REG_LED_RED   = 0x01;
    static const uint8_t REG_LED_BLUE  = 0x02;
    static const uint8_t REG_LED_CTRL  = 0x03;
};

#endif // PAPILIO_RGB_LED_H
