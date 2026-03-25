# Changelog

All notable changes to the BAS firmware will be documented here.

---

## [Unreleased]

### Planned
- Fix all known bugs from v0.1.0-alpha
- Timeout handling for missing echo pulses
- Move variable definitions out of `ultrasonic.h` into `ultrasonic.c`
- Replace magic number `140.0` with documented `#define DISTANCE_DIVISOR`
- Make 100 cm alert threshold configurable via `config.h`
- UART debug output for raw Timer1 count logging
- Interrupt-driven echo measurement (replace polling)
- Validate distance formula constant against real measurements

---

## [0.1.0-alpha]

> ⚠️ Test bench release only. Not production-ready.

### Added
- `main.c` - entry point, main loop, four-state PORTC alert logic
- `ultrasonic.c` / `ultrasonic.h` - dual HC-SR04 distance measurement
  with 10-sample averaging using Timer1
- `LCD_4bit.c` / `LCD_4bit.h` - 16×2 LCD driver in 4-bit mode via PORTD
- `include.h` - shared includes, `_XTAL_FREQ` define, pin aliases
- `config.h` - PIC18F4580 `#pragma config` fuse settings
- `Makefile` - MPLAB X / XC8 build system
- `.gitignore` - excludes all MPLAB X and XC8 generated output
- `README.md` - project overview, hardware, pin mapping, bug register,
  and future update checklist

### Known bugs (documented, unresolved)
- `sprintf` buffer declared as `float[]` instead of `char[]` in `main.c`
- Global variables defined in `ultrasonic.h` - will cause multiple
  definition errors as project scales
- No timeout on rising-edge echo wait in `ultrasonic.c` - firmware
  hangs if no echo is received
- `TMR1IF` not cleared inside the measurement loop - stale flag
  corrupts distance readings after first Timer1 overflow
- Averaging formula `dist / 10 * 10` loses precision due to integer
  truncation order
- `%d` format specifier in `sprintf` may silently fail if XC8 printf
  library is not explicitly set to integer mode
- Distance formula constant `140.0` is undocumented and unverified

### Hardware
- MCU: PIC18F4580 (8 MHz internal oscillator)
- Sensors: HC-SR04 ×2 (left and right)
- Display: 16×2 character LCD, HD44780, 4-bit mode
- Toolchain: MPLAB X IDE + XC8 Compiler
