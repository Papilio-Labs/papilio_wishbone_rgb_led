// Papilio RGB LED OS Plugin - CLI interface
#ifndef PAPILIO_RGB_LED_OS_H
#define PAPILIO_RGB_LED_OS_H

#ifdef ENABLE_PAPILIO_OS

#include <PapilioOS.h>
#include "RGBLed.h"

class PapilioRgbLedOS {
public:
    PapilioRgbLedOS(PapilioRgbLed* device);
    
private:
    PapilioRgbLed* _device;
    void registerCommands();
    
    // Command handlers
    static void handleTutorial(int argc, char** argv);
    static void handleHelp(int argc, char** argv);
    static void handleStatus(int argc, char** argv);
    static void handleSetColor(int argc, char** argv);
    static void handleSetRGB(int argc, char** argv);
    
    // Tutorial helper
    static bool tutorialStep(int stepNum, const char* description, const char* command);
    
    static PapilioRgbLedOS* _instance;
};

#endif // ENABLE_PAPILIO_OS
#endif // PAPILIO_RGB_LED_OS_H
