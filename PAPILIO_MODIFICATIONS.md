# Local Modifications to papilio_wishbone_rgb_led

This is a local copy of the papilio_wishbone_rgb_led library with modifications to make it compatible with the Papilio OS API pattern used by other local libraries.

## Changes Made

### File: `src/PapilioRgbLedOS.cpp`

**Issue:** The original library expected `PapilioOSClass` to have `println()`, `print()`, and `waitForCommand()` methods that don't exist in the current PapilioOS implementation.

**Solution:** Replaced all PapilioOS output methods with direct `Serial` calls to match the pattern used by local libraries (papilio_wb_register, papilio_wb_bram).

**Specific Changes:**
1. Replaced all `PapilioOS.println()` → `Serial.println()`
2. Replaced all `PapilioOS.print()` → `Serial.print()`
3. Simplified `handleTutorial()` to show command list instead of interactive steps (removed `PapilioOS.waitForCommand()` calls)

## Why Local Copy?

The upstream library was written for a different PapilioOS API. Until the upstream is updated or a unified API pattern is established across all Papilio libraries, this local copy ensures compatibility with our build system.

## Upstream

Original source: https://github.com/Papilio-Labs/papilio_wishbone_rgb_led.git

## Future

Consider either:
1. Updating PapilioOSClass to include the missing methods (println, print, waitForCommand)
2. Submitting a PR to upstream to use Serial directly like other libraries
3. Establishing a unified API standard across all Papilio libraries
