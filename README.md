# Bilateral Awareness System (BAS) — Firmware

> **This is a test bench build. Not production-ready.**  
> Hardware is set up purely to verify sensor behaviour and MCU logic before any real e-bike integration. Expect rough edges.

---

## Table of Contents

- [Overview](#overview)
- [Version](#version)
- [Hardware](#hardware)
- [Pin Mapping](#pin-mapping)
- [File Structure](#file-structure)
- [How It Works](#how-it-works)
- [Alert Logic](#alert-logic)
- [Known Bugs](#known-bugs)
- [Future Updates](#future-updates)
- [Build Instructions](#build-instructions)

---

## Overview

BAS (Bilateral Awareness System) is a proximity detection concept for electric bicycles. Two HC-SR04 ultrasonic sensors are mounted on opposing sides of the bike to detect approaching vehicles. When an object enters a configurable threshold distance, PORTC output pins are driven to signal an alert — which could later drive a buzzer, vibration motor, or LED strip on a real bike.

This repository contains the test bench firmware written for the **PIC18F4580** microcontroller using **MPLAB X IDE** and the **XC8 compiler**. The test bench is a breadboard setup; there is no PCB yet.

The initial codebase was written in **2021** and is now being revisited in **2026**.

---

## Version

| Field | Detail |
|---|---|
| Version | v0.1.0-testbench |
| MCU | PIC18F4580 |
| Toolchain | MPLAB X IDE + XC8 Compiler |
| Clock | Internal 8 MHz RC oscillator |
| Status | Test bench only — under active revision |
| Last updated | March 2026 |

Version history is tracked here manually until a proper changelog is set up.

**v0.1.0-testbench** (March 2026)
- Revisited original 2021 codebase
- Documented known bugs (see below)
- Added `.gitignore` and this README
- No functional changes yet — bugs are being catalogued before fixes begin

---

## Hardware

| Component | Part | Quantity |
|---|---|---|
| Microcontroller | PIC18F4580 (DIP-40) | 1 |
| Ultrasonic sensor | HC-SR04 | 2 |
| Display | 16×2 character LCD (HD44780) | 1 |
| Programmer | PICkit 3 / PICkit 4 | 1 |
| Power supply | 5V regulated (bench supply) | 1 |
| Misc | Breadboard, jumper wires, 10kΩ pot for LCD contrast | — |

---

## Pin Mapping

```
PIC18F4580 Pin    Direction    Connected to
-------------------------------------------------
RD0 (LATD0)       OUTPUT       HC-SR04 #1 — TRIG
RD1 (LATD1)       OUTPUT       HC-SR04 #2 — TRIG
RD2 (LATD2)       OUTPUT       LCD — RS
RD3 (LATD3)       OUTPUT       LCD — EN
RD4–RD7           OUTPUT       LCD — D4, D5, D6, D7  (4-bit mode)
RB0               INPUT        HC-SR04 #1 — ECHO
RB1               INPUT        HC-SR04 #2 — ECHO
RC0–RC7           OUTPUT       Alert signal outputs (PORTC)
```

PORTB internal pull-ups are enabled. PORTB and PORTD share the MCU but serve different roles — be careful with LATD direct writes (see Known Bugs).

---

## File Structure

```
BAS_firmware.X/
├── main.c           — entry point, main loop, alert logic
├── ultrasonic.c     — HC-SR04 distance measurement (both sensors)
├── ultrasonic.h     — declarations and global variables
├── LCD_4bit.c       — 16x2 LCD driver (4-bit mode)
├── LCD_4bit.h       — LCD pin definitions and prototypes
├── include.h        — shared includes, frequency define, pin aliases
├── config.h         — PIC18F4580 configuration bits (#pragma config)
├── Makefile         — build system
├── nbproject/
│   ├── project.xml          — MPLAB X project definition
│   └── configurations.xml   — build configuration
├── .gitignore
├── README.md
└── LICENSE
```

Build output (`build/`, `dist/`) is excluded from version control. See `.gitignore`.

---

## How It Works

1. Both HC-SR04 sensors are triggered with a 10 µs pulse.
2. Timer1 measures the duration of the returning ECHO pulse.
3. Distance is calculated as: `Distance (cm) = Timer1_count / 140.0`  
   *(This constant is derived empirically for 8 MHz internal clock — needs verification.)*
4. Each sensor reading is averaged over 10 samples to reduce noise.
5. Results are printed to the LCD: sensor 1 on the left of line 2, sensor 2 on the right.
6. PORTC is driven based on the detection state of both sensors (see Alert Logic below).

---

## Alert Logic

| Left sensor | Right sensor | PORTC value | Meaning |
|---|---|---|---|
| ≤ 100 cm | ≤ 100 cm | `0xF0` | Both sides — danger |
| > 100 cm | ≤ 100 cm | `0x30` | Right side only |
| ≤ 100 cm | > 100 cm | `0xC0` | Left side only |
| > 100 cm | > 100 cm | `0x00` | Clear — no alert |

The 100 cm threshold is currently hardcoded. It will be made configurable in a future version.

---

## Known Bugs

> These bugs are **documented and acknowledged**. They are being left in place intentionally during this test phase so that sensor and display behaviour can first be validated end-to-end. Fixes will be applied incrementally — see [Future Updates](#future-updates).

---

### Bug 1 — `sprintf` buffer declared as `float[]` instead of `char[]`

**File:** `main.c`  
**Severity:** High

```c
// Current (wrong)
float Total_distance[10];
float Total_distance2[10];

// Should be
char Total_distance[10];
char Total_distance2[10];
```

`sprintf` writes ASCII characters into a `char *` buffer. Using a `float` array is undefined behaviour. It appears to work on this compiler/target by accident due to memory layout, but will be corrected.

---

### Bug 2 — Variable definitions inside a header file

**File:** `ultrasonic.h`  
**Severity:** High

```c
// In ultrasonic.h — this causes multiple definitions
float Distance, Distance2, dist=0.0, dist2=0.0;
int Time, Time2, i=0, j=0, distance, distance2;
```

Variables should be defined once in `ultrasonic.c` and declared as `extern` in `ultrasonic.h`. Currently this works only because the header is included in a limited number of translation units. Will break if the project grows.

---

### Bug 3 — No timeout on the rising-edge echo wait

**File:** `ultrasonic.c`  
**Severity:** High

```c
while(PORTBbits.RB0 == 0);  // hangs forever if no echo arrives
```

If a sensor is disconnected or the object is out of range, the firmware hangs here permanently. The falling-edge wait checks `TMR1IF`, but the rising-edge wait does not. A timeout condition needs to be added to both loops.

---

### Bug 4 — `TMR1IF` never cleared inside the measurement loop

**File:** `ultrasonic.c`  
**Severity:** High

`TMR1IF` is cleared once at startup but never inside the averaging loop. After the first Timer1 overflow, the flag stays set and the echo-wait condition exits immediately on every subsequent iteration, returning garbage distance values. The flag must be cleared at the start of each sample.

---

### Bug 5 — Averaging formula loses precision due to integer truncation order

**File:** `ultrasonic.c`  
**Severity:** Medium

```c
distance = dist / 10 * 10;
```

Due to C operator precedence, this is `(dist / 10) * 10`. Because `distance` is an `int`, the intermediate result is truncated — e.g. `dist = 152.7` → `152.7/10 = 15` (int) → `15 * 10 = 150`. The average is rounded down to the nearest 10 cm. This should be:

```c
distance = (int)(dist / 10);
```

---

### Bug 6 — `sprintf` format specifier may not work on default XC8 printf level

**File:** `main.c`  
**Severity:** Medium

```c
sprintf(Total_distance, "%d", distance);
```

XC8 uses a minimal `printf` by default that may not support `%d`. The project linker settings need to explicitly select the "integer" printf library, otherwise this silently produces empty output.

---

### Bug 7 — Magic number `140.0` in distance formula

**File:** `ultrasonic.c`  
**Severity:** Low

```c
Distance = ((float)Time / 140.00);
```

The constant `140.0` is undocumented. It is a simplified approximation derived from Timer1 tick rate and the speed of sound. It needs to be verified against actual measurements and replaced with a named `#define` with a comment explaining the derivation.

---

## Future Updates

The following is planned but not started yet. No timeline is set — this is a test bench.

- [ ] Fix all bugs listed above
- [ ] Add timeout handling for missing echo pulses (watchdog-style loop counter)
- [ ] Move variable definitions from `ultrasonic.h` to `ultrasonic.c` with proper `extern` declarations
- [ ] Replace magic number `140.0` with a documented `#define DISTANCE_DIVISOR`
- [ ] Make the 100 cm alert threshold configurable via a `#define` in `config.h`
- [ ] Add a third sensor (rear) for complete 3-side coverage
- [ ] Evaluate replacing Timer1 polling with interrupt-driven echo measurement
- [ ] Add UART debug output for logging raw Timer1 counts during calibration
- [ ] Validate the `140.0` constant by cross-checking measured vs actual distances
- [ ] Design a simple PCB once the test bench logic is confirmed working
- [ ] Add power consumption measurement for battery-operated e-bike use case
- [ ] Explore I²C LCD module to free up PORTD pins for future expansion

---

## Build Instructions

1. Clone the repository.
2. Open MPLAB X IDE → File → Open Project → select `BAS_firmware.X`.
3. Connect PICkit 3/4 to the board.
4. Select the correct programmer under Project Properties if needed.
5. Build: `Production` → `Build Main Project` (or press F11).
6. Program: `Production` → `Make and Program Device`.

**Note:** If `sprintf("%d", ...)` produces blank output, go to Project Properties → XC8 Linker → Additional options and set the printf library to `printf-integer` or `printf-float`.

