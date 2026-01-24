# Papilio RGB LED Library - AI Assistant Skill Guide

## Overview

This library provides control of WS2812B RGB LEDs through an FPGA-based Wishbone interface. It includes both programmatic API and optional CLI commands when integrated with `papilio_os`.

For general Papilio development skills, see `papilio_dev_tools/AI_SKILL.md`.

## Library Architecture

- **Firmware**: `PapilioRGBLed` class provides high-level API
- **OS Plugin**: `PapilioRGBLedOS` adds CLI commands (when `ENABLE_PAPILIO_OS` defined)
- **Gateware**: `wb_simple_rgb_led` Verilog module interfaces with WS2812B LED

## Wishbone Register Map

Base Address: Configurable (default 0x2000), auto-assigned by builder

| Offset | Name | Access | Description |
|--------|------|--------|-------------|
| +0x00 | LED_GREEN | R/W | Green component (0-255) |
| +0x01 | LED_RED | R/W | Red component (0-255) |
| +0x02 | LED_BLUE | R/W | Blue component (0-255) |
| +0x03 | LED_CTRL | R | Status register (bit 0: busy) |

**Note**: WS2812B LEDs use GRB color ordering, not RGB!

## Programmatic API

### Initialization
```cpp
#include <PapilioRGBLed.h>

// Create instance with base address
PapilioRGBLed rgbLed(0x2000);  // Or use auto-assigned address

void setup() {
    // Initialize (relies on WishboneSPI being initialized first)
    rgbLed.begin();
}
```

### Setting Colors
```cpp
// Method 1: Using RGB components (0-255)
rgbLed.setColorRGB(25, 0, 0);  // Red

// Method 2: Using GRB hex value
rgbLed.setColor(0x190000);  // Green (GRB format!)

// Method 3: Using color constants
rgbLed.setColor(PapilioRGBLed::COLOR_BLUE);
```

### Color Constants
```cpp
PapilioRGBLed::COLOR_OFF      // 0x000000
PapilioRGBLed::COLOR_RED      // 0x001900
PapilioRGBLed::COLOR_GREEN    // 0x190000
PapilioRGBLed::COLOR_BLUE     // 0x000019
PapilioRGBLed::COLOR_YELLOW   // 0x191900
PapilioRGBLed::COLOR_CYAN     // 0x190019
PapilioRGBLed::COLOR_MAGENTA  // 0x001919
PapilioRGBLed::COLOR_WHITE    // 0x191919
PapilioRGBLed::COLOR_ORANGE   // 0x0C1900
PapilioRGBLed::COLOR_PURPLE   // 0x000C0C
```

### Status Checking
```cpp
// Check if LED transmission is in progress
if (rgbLed.isBusy()) {
    // Wait for transmission to complete
}
```

## CLI Commands (PapilioOS Integration)

When `ENABLE_PAPILIO_OS` is defined and library is auto-integrated:

```bash
# Interactive tutorial
rgbled tutorial

# Show help
rgbled help

# Check status
rgbled status

# Set color using RGB (0-255)
rgbled setrgb 25 0 0        # Red
rgbled setrgb 0 25 0        # Green
rgbled setrgb 0 0 25        # Blue

# Set color using GRB hex
rgbled setcolor 0x190000    # Green
rgbled setcolor 0x001900    # Red
rgbled setcolor 0x000019    # Blue
```

## Gateware Module

### Module: wb_simple_rgb_led

**File**: `gateware/wb_simple_rgb_led.v`

**Ports**:
- `clk` - System clock (27 MHz)
- `rst` - Reset signal
- `wb_adr_i[7:0]` - Wishbone address
- `wb_dat_i[7:0]` - Wishbone data in
- `wb_dat_o[7:0]` - Wishbone data out
- `wb_we_i` - Write enable
- `wb_cyc_i` - Cycle signal
- `wb_stb_i` - Strobe signal
- `wb_ack_o` - Acknowledge
- `led_out` - WS2812B data signal

**Instantiation Template**:
```verilog
wb_simple_rgb_led rgb_led_inst (
    .clk(clk),
    .rst(rst),
    .wb_adr_i(wb_adr[7:0]),
    .wb_dat_i(wb_dat_w[7:0]),
    .wb_dat_o(rgb_led_wb_dat),
    .wb_we_i(wb_we),
    .wb_cyc_i(wb_cyc),
    .wb_stb_i(rgb_led_wb_stb),
    .wb_ack_o(rgb_led_wb_ack),
    .led_out(led_out)
);
```

## Automatic Library Builder Integration

This library follows Papilio Library Standards v1.0 with complete metadata in `library.json`:

**Key Features**:
- Auto-discovery from `lib_deps` or `lib_extra_dirs`
- Auto-allocation of Wishbone address space (256 bytes)
- Auto-generation of FPGA interconnect
- Auto-generation of ESP32 initialization code
- Auto-registration of CLI commands

**Builder Requirements**:
1. Add to platformio.ini:
```ini
lib_deps = 
    papilio_wishbone_rgb_led
```

2. Mark regions in FPGA top.v:
```verilog
//# PAPILIO_AUTO_WIRES
// Auto-generated wires appear here
//# PAPILIO_AUTO_WIRES_END

//# PAPILIO_AUTO_MODULE_INST
// Auto-generated module instantiations
//# PAPILIO_AUTO_MODULE_INST_END

//# PAPILIO_AUTO_WISHBONE
// Auto-generated Wishbone interconnect
//# PAPILIO_AUTO_WISHBONE_END
```

3. Mark regions in ESP32 main.cpp:
```cpp
//# PAPILIO_AUTO_INCLUDES
// Auto-generated includes
//# PAPILIO_AUTO_INCLUDES_END

//# PAPILIO_AUTO_GLOBALS
// Auto-generated global variables
//# PAPILIO_AUTO_GLOBALS_END

void setup() {
    //# PAPILIO_AUTO_INIT
    // Auto-generated initialization
    //# PAPILIO_AUTO_INIT_END
}

//# PAPILIO_AUTO_CLI
// Auto-generated CLI setup (if ENABLE_PAPILIO_OS defined)
//# PAPILIO_AUTO_CLI_END
```

## Pin Assignments

### Papilio RetroCade
See `gateware/constraints/rgb_led_papilio_retrocade.cst`:
- LED_OUT: Pin P9 (WS2812B data signal)

**Integration**: Copy the library's constraint file to your project:
```bash
cp .pio/libdeps/fpga/papilio_wishbone_rgb_led/gateware/constraints/rgb_led_papilio_retrocade.cst fpga/constraints/
```

Then add to `fpga/project.gprj`:
```xml
<File path="constraints/rgb_led_papilio_retrocade.cst" type="file.cst" enable="1" />
```

**Important**: Do NOT merge library constraints into your base constraint file. Keep them as separate files for better organization and to avoid pin conflicts.

### Papilio Synth
See `gateware/constraints/rgb_led_papilio_synth.cst`:
- LED_OUT: Pin TBD

## Common Operations

### Setting LED to specific color
```cpp
// API
rgbLed.setColorRGB(255, 128, 0);  // Orange at full brightness

// CLI
rgbled setrgb 255 128 0

// Direct register (via wishboneWrite8)
wishboneWrite8(BASE_ADDR + 0, 128);  // Green
wishboneWrite8(BASE_ADDR + 1, 255);  // Red
wishboneWrite8(BASE_ADDR + 2, 0);    // Blue
```

### Creating color animation
```cpp
for (int i = 0; i < 256; i++) {
    rgbLed.setColorRGB(i, 0, 255 - i);  // Red to Blue fade
    delay(10);
}
```

### Checking transmission status
```cpp
// Set color
rgbLed.setColor(PapilioRGBLed::COLOR_GREEN);

// Wait for transmission to complete
while (rgbLed.isBusy()) {
    delay(1);
}
```

## Adding Features

### New Color Presets
Add to `PapilioRGBLed.h`:
```cpp
static const uint32_t COLOR_LIME = 0x646400;  // GRB format
```

Add to `PapilioRGBLed.cpp`:
```cpp
const uint32_t PapilioRGBLed::COLOR_LIME = 0x646400;
```

### Brightness Control
Extend API:
```cpp
void setBrightness(float factor);  // 0.0-1.0
```

Implementation:
```cpp
void PapilioRGBLed::setBrightness(float factor) {
    // Scale all future colors by factor
    // Or add brightness register to gateware
}
```

### Multi-LED Support
Modify gateware to support LED chains:
- Add NUM_LEDS parameter
- Extend register map (3 bytes per LED)
- Update WS2812B controller

## Troubleshooting

### LED not responding
1. Check Wishbone bus initialization:
   ```cpp
   // Ensure WishboneSPI is initialized before rgbLed.begin()
   ```

2. Verify FPGA bitstream loaded:
   ```cpp
   // Check if reads return expected values
   uint8_t status = wishboneRead8(BASE_ADDR + 3);
   ```

3. Check pin assignment in constraint file

### Wrong colors displayed
1. Remember GRB ordering:
   ```cpp
   // NOT RGB! Use GRB format
   rgbLed.setColor(0xGGRRBB);  // Green-Red-Blue
   ```

2. Verify voltage levels (3.3V logic for WS2812B)

### Builder not discovering library
1. Check `library.json` has `papilio` section
2. Verify library is in `lib/`, `lib_deps`, or `lib_extra_dirs`
3. Check builder output for validation errors

## Testing

### Hardware Test
Location: `tests/hw/`

Run with PlatformIO:
```bash
cd tests/hw
pio test
```

Tests verify:
- API initialization
- Register reads/writes
- Color setting
- Status checking

### Simulation Test
Location: `tests/sim/`

Run with:
```bash
cd tests/sim
python run_all_sims.py
```

Tests verify:
- Wishbone timing
- Register operations
- WS2812B timing

## Resources

- WS2812B Datasheet: [https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
- Wishbone Specification: [https://opencores.org/howto/wishbone](https://opencores.org/howto/wishbone)
- Papilio Library Standards: `@/openspec/specs/papilio-library-standards/spec.md`

## Version History

- **0.1.0** - Initial release with Papilio Library Standards compliance
  - PapilioRgbLed class with base address support
  - PapilioRgbLedOS CLI plugin
  - WS2812B controller gateware
  - Automatic builder integration
