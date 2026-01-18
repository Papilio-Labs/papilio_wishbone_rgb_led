#include "PapilioRgbLedOS.h"

#ifdef ENABLE_PAPILIO_OS

PapilioRgbLedOS* PapilioRgbLedOS::_instance = nullptr;

PapilioRgbLedOS::PapilioRgbLedOS(PapilioRgbLed* device) : _device(device) {
    _instance = this;
    registerCommands();
}

void PapilioRgbLedOS::registerCommands() {
    PapilioOS.registerCommand("rgbled", "tutorial", handleTutorial, "Interactive RGB LED tutorial");
    PapilioOS.registerCommand("rgbled", "help", handleHelp, "Show all rgbled commands");
    PapilioOS.registerCommand("rgbled", "status", handleStatus, "Show RGB LED controller status");
    PapilioOS.registerCommand("rgbled", "setcolor", handleSetColor, "Set LED color (GRB hex): rgbled setcolor 0x190000");
    PapilioOS.registerCommand("rgbled", "setrgb", handleSetRGB, "Set LED color (R G B): rgbled setrgb 25 0 0");
}

void PapilioRgbLedOS::handleTutorial(int argc, char** argv) {
    Serial.println("\n========================================");
    Serial.println("   RGB LED Interactive Tutorial");
    Serial.println("========================================\n");
    
    Serial.println("This tutorial will guide you through controlling WS2812B RGB LEDs.");
    Serial.println("Type 'exit' at any prompt to quit the tutorial.\n");
    
    delay(1000);
    
    if (!_instance || !_instance->_device) {
        Serial.println("Note: Device not initialized. Tutorial will show commands anyway.");
        Serial.println("In your sketch, create the device in setup():\n");
        Serial.println("  PapilioRgbLed rgbLed(0x2000);");
        Serial.println("  rgbLed.begin();\n");
        delay(1000);
    }
    
    // Step 1: Check status
    if (!tutorialStep(1, "Check RGB LED controller status",
                      "rgbled status")) {
        return;
    }
    
    // Step 2: Set LED to red
    if (!tutorialStep(2, "Set LED to red (25% brightness)",
                      "rgbled setrgb 25 0 0")) {
        return;
    }
    
    // Step 3: Set LED to green
    if (!tutorialStep(3, "Set LED to green",
                      "rgbled setrgb 0 25 0")) {
        return;
    }
    
    // Step 4: Set LED to blue
    if (!tutorialStep(4, "Set LED to blue",
                      "rgbled setrgb 0 0 25")) {
        return;
    }
    
    // Step 5: Set LED using hex color (cyan)
    if (!tutorialStep(5, "Set LED to cyan using hex color (GRB format)",
                      "rgbled setcolor 0x190019")) {
        return;
    }
    
    Serial.println("\n========================================");
    Serial.println("   Tutorial Complete!");
    Serial.println("========================================\n");
    
    Serial.println("You've learned how to:");
    Serial.println("  - Check RGB LED controller status");
    Serial.println("  - Set colors using RGB values");
    Serial.println("  - Set colors using hex (GRB format)");
    Serial.println("\nFor more info, run: rgbled help");
}

bool PapilioRgbLedOS::tutorialStep(int stepNum, const char* description, 
                                    const char* command) {
    Serial.printf("\nStep %d: %s\n", stepNum, description);
    Serial.printf("Try the command: %s\n", command);
    Serial.print("\nPress Enter when ready (or type 'exit' to quit): ");
    
    // Wait for user input
    while (!Serial.available()) {
        delay(10);
    }
    
    String input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println();
    
    if (input.equalsIgnoreCase("exit") || input.equalsIgnoreCase("quit")) {
        Serial.println("Tutorial exited.");
        return false;
    }
    
    // Execute the command
    Serial.printf("> %s\n", command);
    
    // Parse and execute
    char cmdCopy[128];
    strncpy(cmdCopy, command, sizeof(cmdCopy) - 1);
    cmdCopy[sizeof(cmdCopy) - 1] = '\0';
    
    char* argv[10];
    int argc = 0;
    char* token = strtok(cmdCopy, " ");
    while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(nullptr, " ");
    }
    
    // Route to handler (skip module name "rgbled")
    if (argc >= 2) {
        if (strcmp(argv[1], "status") == 0) {
            handleStatus(argc - 1, &argv[1]);
        } else if (strcmp(argv[1], "setrgb") == 0) {
            handleSetRGB(argc - 1, &argv[1]);
        } else if (strcmp(argv[1], "setcolor") == 0) {
            handleSetColor(argc - 1, &argv[1]);
        }
    }
    
    delay(1000);
    return true;
}

void PapilioRgbLedOS::handleHelp(int argc, char** argv) {
    Serial.println("\nRGB LED Commands:");
    Serial.println("  rgbled tutorial        - Interactive tutorial");
    Serial.println("  rgbled status          - Show controller status");
    Serial.println("  rgbled setcolor <hex>  - Set color (GRB format, e.g., 0x190000 for green)");
    Serial.println("  rgbled setrgb <R> <G> <B> - Set color (R/G/B 0-255)");
    Serial.println("\nColor format: GRB (Green-Red-Blue) for WS2812B LEDs");
    Serial.println("Common colors:");
    Serial.println("  Red:     rgbled setrgb 25 0 0   or  rgbled setcolor 0x001900");
    Serial.println("  Green:   rgbled setrgb 0 25 0   or  rgbled setcolor 0x190000");
    Serial.println("  Blue:    rgbled setrgb 0 0 25   or  rgbled setcolor 0x000019");
    Serial.println("  Yellow:  rgbled setrgb 25 25 0  or  rgbled setcolor 0x191900");
    Serial.println("  Cyan:    rgbled setrgb 0 25 25  or  rgbled setcolor 0x190019");
    Serial.println("  Magenta: rgbled setrgb 25 0 25  or  rgbled setcolor 0x001919");
    Serial.println("  White:   rgbled setrgb 25 25 25 or  rgbled setcolor 0x191919");
}

void PapilioRgbLedOS::handleStatus(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        Serial.println("Error: RGB LED device not initialized");
        return;
    }
    
    Serial.print("RGB LED Controller at address 0x");
    Serial.println(String(_instance->_device->getBaseAddress(), HEX));
    Serial.print("Busy: ");
    Serial.println(_instance->_device->isBusy() ? "Yes" : "No");
}

void PapilioRgbLedOS::handleSetColor(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        Serial.println("Error: RGB LED device not initialized");
        return;
    }
    
    if (argc < 3) {
        Serial.println("Usage: rgbled setcolor <hex_color>");
        Serial.println("Example: rgbled setcolor 0x190000 (green)");
        return;
    }
    
    uint32_t color = strtoul(argv[2], nullptr, 16);
    _instance->_device->setColor(color);
    Serial.print("LED color set to 0x");
    Serial.println(String(color, HEX));
}

void PapilioRgbLedOS::handleSetRGB(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        Serial.println("Error: RGB LED device not initialized");
        return;
    }
    
    if (argc < 5) {
        Serial.println("Usage: rgbled setrgb <red> <green> <blue>");
        Serial.println("Example: rgbled setrgb 25 0 0 (red)");
        return;
    }
    
    uint8_t red = atoi(argv[2]);
    uint8_t green = atoi(argv[3]);
    uint8_t blue = atoi(argv[4]);
    
    _instance->_device->setColorRGB(red, green, blue);
    Serial.print("LED color set to RGB(");
    Serial.print(String(red));
    Serial.print(", ");
    Serial.print(String(green));
    Serial.print(", ");
    Serial.print(String(blue));
    Serial.println(")");
}

#endif // ENABLE_PAPILIO_OS
