/**
 * Papilio RGB LED Example with CLI
 * 
 * This example demonstrates using the PapilioRgbLed library
 * with both programmatic API and CLI commands.
 * 
 * Hardware: Papilio RetroCade or compatible board with WS2812B LED
 * 
 * Usage:
 * 1. Upload FPGA bitstream with wb_simple_rgb_led module
 * 2. Upload this sketch to ESP32
 * 3. Open serial monitor
 * 4. Try CLI commands or watch color animation
 */

#include <Arduino.h>
#include <WishboneSPI.h>
#include <RGBLed.h>

// Uncomment to enable CLI interface
// #define ENABLE_PAPILIO_OS

#ifdef ENABLE_PAPILIO_OS
#include <PapilioOS.h>
#include <PapilioRgbLedOS.h>
#endif

// SPI Configuration
#define SPI_CLK  18
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_CS   5

SPIClass* fpgaSPI;

// RGB LED Controller (base address will be auto-assigned in automatic builder)
// For manual setup, use the address from your FPGA design
PapilioRgbLed rgbLed(0x2000);

#ifdef ENABLE_PAPILIO_OS
PapilioRgbLedOS rgbLedOS(&rgbLed);
#endif

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n=== Papilio RGB LED Example ===");
    
    // Initialize SPI
    fpgaSPI = new SPIClass(HSPI);
    fpgaSPI->begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_CS);
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);
    
    // Initialize Wishbone SPI
    wishboneInit(fpgaSPI, SPI_CS);
    Serial.println("Wishbone SPI initialized");
    
    // Initialize RGB LED
    if (rgbLed.begin()) {
        Serial.println("RGB LED initialized");
    } else {
        Serial.println("Failed to initialize RGB LED!");
    }
    
#ifdef ENABLE_PAPILIO_OS
    Serial.println("\nCLI enabled. Type 'rgbled tutorial' to get started.");
    Serial.println("Type 'rgbled help' for available commands.\n");
    PapilioOS::begin();
#else
    Serial.println("\nRunning color animation...");
    Serial.println("To enable CLI, uncomment #define ENABLE_PAPILIO_OS\n");
#endif
}

void loop() {
#ifdef ENABLE_PAPILIO_OS
    // CLI mode - handle commands
    PapilioOS::loop();
#else
    // Demo mode - cycle through colors
    static unsigned long lastChange = 0;
    static int colorIndex = 0;
    
    if (millis() - lastChange >= 2000) {
        lastChange = millis();
        
        switch (colorIndex) {
            case 0:
                Serial.println("Setting LED to RED");
                rgbLed.setColor(PapilioRgbLed::COLOR_RED);
                break;
            case 1:
                Serial.println("Setting LED to GREEN");
                rgbLed.setColor(PapilioRgbLed::COLOR_GREEN);
                break;
            case 2:
                Serial.println("Setting LED to BLUE");
                rgbLed.setColor(PapilioRgbLed::COLOR_BLUE);
                break;
            case 3:
                Serial.println("Setting LED to YELLOW");
                rgbLed.setColor(PapilioRgbLed::COLOR_YELLOW);
                break;
            case 4:
                Serial.println("Setting LED to CYAN");
                rgbLed.setColor(PapilioRgbLed::COLOR_CYAN);
                break;
            case 5:
                Serial.println("Setting LED to MAGENTA");
                rgbLed.setColor(PapilioRgbLed::COLOR_MAGENTA);
                break;
            case 6:
                Serial.println("Setting LED to WHITE");
                rgbLed.setColor(PapilioRgbLed::COLOR_WHITE);
                break;
        }
        
        colorIndex = (colorIndex + 1) % 7;
    }
#endif
}
