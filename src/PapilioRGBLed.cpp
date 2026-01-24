#include "PapilioRGBLed.h"

// Color constants (GRB format)
const uint32_t PapilioRGBLed::COLOR_OFF     = 0x000000;
const uint32_t PapilioRGBLed::COLOR_RED     = 0x001900; // G=0, R=25, B=0
const uint32_t PapilioRGBLed::COLOR_GREEN   = 0x190000; // G=25, R=0, B=0
const uint32_t PapilioRGBLed::COLOR_BLUE    = 0x000019; // G=0, R=0, B=25
const uint32_t PapilioRGBLed::COLOR_YELLOW  = 0x191900;
const uint32_t PapilioRGBLed::COLOR_CYAN    = 0x190019;
const uint32_t PapilioRGBLed::COLOR_MAGENTA = 0x001919;
const uint32_t PapilioRGBLed::COLOR_WHITE   = 0x191919;
const uint32_t PapilioRGBLed::COLOR_ORANGE  = 0x0C1900;
const uint32_t PapilioRGBLed::COLOR_PURPLE  = 0x000C0C;

PapilioRGBLed::PapilioRGBLed(uint16_t baseAddress) : _baseAddress(baseAddress) {
}

bool PapilioRGBLed::begin() {
    // No specific initialization needed - relies on WishboneSPI being initialized
    return true;
}

void PapilioRGBLed::setColor(uint32_t grbColor) {
    uint8_t g = (grbColor >> 16) & 0xFF;
    uint8_t r = (grbColor >> 8) & 0xFF;
    uint8_t b = grbColor & 0xFF;

    wishboneWrite8(_baseAddress + REG_LED_GREEN, g);
    wishboneWrite8(_baseAddress + REG_LED_RED, r);
    wishboneWrite8(_baseAddress + REG_LED_BLUE, b);
}

void PapilioRGBLed::setColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = ((uint32_t)green << 16) | ((uint32_t)red << 8) | blue;
    setColor(color);
}

bool PapilioRGBLed::isBusy() {
    uint8_t status = wishboneRead8(_baseAddress + REG_LED_CTRL);
    return (status & 0x01) != 0;
}
