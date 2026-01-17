#include "PapilioRgbLedOS.h"

#ifdef ENABLE_PAPILIO_OS

PapilioRgbLedOS* PapilioRgbLedOS::_instance = nullptr;

PapilioRgbLedOS::PapilioRgbLedOS(PapilioRgbLed* device) : _device(device) {
    _instance = this;
    registerCommands();
}

void PapilioRgbLedOS::registerCommands() {
    PapilioOS::registerCommand("rgbled", "tutorial", handleTutorial, "Interactive RGB LED tutorial");
    PapilioOS::registerCommand("rgbled", "help", handleHelp, "Show all rgbled commands");
    PapilioOS::registerCommand("rgbled", "status", handleStatus, "Show RGB LED controller status");
    PapilioOS::registerCommand("rgbled", "setcolor", handleSetColor, "Set LED color (GRB hex): rgbled setcolor 0x190000");
    PapilioOS::registerCommand("rgbled", "setrgb", handleSetRGB, "Set LED color (R G B): rgbled setrgb 25 0 0");
}

void PapilioRgbLedOS::handleTutorial(int argc, char** argv) {
    PapilioOS::println("\n=== RGB LED Tutorial ===\n");
    PapilioOS::println("This tutorial will guide you through using the WS2812B RGB LED controller.");
    PapilioOS::println("Type 'exit' or 'quit' at any time to leave the tutorial.\n");
    
    // Step 1: Status
    PapilioOS::println("Step 1: Check controller status");
    PapilioOS::println("Type: rgbled status");
    String cmd = PapilioOS::waitForCommand();
    if (cmd == "exit" || cmd == "quit") return;
    if (cmd == "rgbled status") {
        handleStatus(0, nullptr);
    }
    
    // Step 2: Set color to red
    PapilioOS::println("\nStep 2: Set LED to red");
    PapilioOS::println("Type: rgbled setrgb 25 0 0");
    cmd = PapilioOS::waitForCommand();
    if (cmd == "exit" || cmd == "quit") return;
    if (cmd.startsWith("rgbled setrgb")) {
        if (_instance && _instance->_device) {
            _instance->_device->setColorRGB(25, 0, 0);
            PapilioOS::println("LED set to red!");
        }
    }
    
    // Step 3: Set color to green
    PapilioOS::println("\nStep 3: Set LED to green");
    PapilioOS::println("Type: rgbled setrgb 0 25 0");
    cmd = PapilioOS::waitForCommand();
    if (cmd == "exit" || cmd == "quit") return;
    if (cmd.startsWith("rgbled setrgb")) {
        if (_instance && _instance->_device) {
            _instance->_device->setColorRGB(0, 25, 0);
            PapilioOS::println("LED set to green!");
        }
    }
    
    // Step 4: Set color to blue
    PapilioOS::println("\nStep 4: Set LED to blue");
    PapilioOS::println("Type: rgbled setrgb 0 0 25");
    cmd = PapilioOS::waitForCommand();
    if (cmd == "exit" || cmd == "quit") return;
    if (cmd.startsWith("rgbled setrgb")) {
        if (_instance && _instance->_device) {
            _instance->_device->setColorRGB(0, 0, 25);
            PapilioOS::println("LED set to blue!");
        }
    }
    
    // Step 5: Using hex color
    PapilioOS::println("\nStep 5: Set color using hex (cyan = green + blue)");
    PapilioOS::println("Type: rgbled setcolor 0x190019");
    cmd = PapilioOS::waitForCommand();
    if (cmd == "exit" || cmd == "quit") return;
    if (cmd.startsWith("rgbled setcolor")) {
        if (_instance && _instance->_device) {
            _instance->_device->setColor(0x190019);
            PapilioOS::println("LED set to cyan!");
        }
    }
    
    PapilioOS::println("\nTutorial complete! Try 'rgbled help' for more commands.");
}

void PapilioRgbLedOS::handleHelp(int argc, char** argv) {
    PapilioOS::println("\nRGB LED Commands:");
    PapilioOS::println("  rgbled tutorial        - Interactive tutorial");
    PapilioOS::println("  rgbled status          - Show controller status");
    PapilioOS::println("  rgbled setcolor <hex>  - Set color (GRB format, e.g., 0x190000 for green)");
    PapilioOS::println("  rgbled setrgb <R> <G> <B> - Set color (R/G/B 0-255)");
    PapilioOS::println("\nColor format: GRB (Green-Red-Blue) for WS2812B LEDs");
    PapilioOS::println("Common colors:");
    PapilioOS::println("  Red:     rgbled setrgb 25 0 0   or  rgbled setcolor 0x001900");
    PapilioOS::println("  Green:   rgbled setrgb 0 25 0   or  rgbled setcolor 0x190000");
    PapilioOS::println("  Blue:    rgbled setrgb 0 0 25   or  rgbled setcolor 0x000019");
    PapilioOS::println("  Yellow:  rgbled setrgb 25 25 0  or  rgbled setcolor 0x191900");
    PapilioOS::println("  Cyan:    rgbled setrgb 0 25 25  or  rgbled setcolor 0x190019");
    PapilioOS::println("  Magenta: rgbled setrgb 25 0 25  or  rgbled setcolor 0x001919");
    PapilioOS::println("  White:   rgbled setrgb 25 25 25 or  rgbled setcolor 0x191919");
}

void PapilioRgbLedOS::handleStatus(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        PapilioOS::println("Error: RGB LED device not initialized");
        return;
    }
    
    PapilioOS::print("RGB LED Controller at address 0x");
    PapilioOS::println(String(_instance->_device->getBaseAddress(), HEX));
    PapilioOS::print("Busy: ");
    PapilioOS::println(_instance->_device->isBusy() ? "Yes" : "No");
}

void PapilioRgbLedOS::handleSetColor(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        PapilioOS::println("Error: RGB LED device not initialized");
        return;
    }
    
    if (argc < 3) {
        PapilioOS::println("Usage: rgbled setcolor <hex_color>");
        PapilioOS::println("Example: rgbled setcolor 0x190000 (green)");
        return;
    }
    
    uint32_t color = strtoul(argv[2], nullptr, 16);
    _instance->_device->setColor(color);
    PapilioOS::print("LED color set to 0x");
    PapilioOS::println(String(color, HEX));
}

void PapilioRgbLedOS::handleSetRGB(int argc, char** argv) {
    if (!_instance || !_instance->_device) {
        PapilioOS::println("Error: RGB LED device not initialized");
        return;
    }
    
    if (argc < 5) {
        PapilioOS::println("Usage: rgbled setrgb <red> <green> <blue>");
        PapilioOS::println("Example: rgbled setrgb 25 0 0 (red)");
        return;
    }
    
    uint8_t red = atoi(argv[2]);
    uint8_t green = atoi(argv[3]);
    uint8_t blue = atoi(argv[4]);
    
    _instance->_device->setColorRGB(red, green, blue);
    PapilioOS::print("LED color set to RGB(");
    PapilioOS::print(String(red));
    PapilioOS::print(", ");
    PapilioOS::print(String(green));
    PapilioOS::print(", ");
    PapilioOS::print(String(blue));
    PapilioOS::println(")");
}

#endif // ENABLE_PAPILIO_OS
