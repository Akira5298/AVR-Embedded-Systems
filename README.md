# AVR Embedded Systems — C Lab Projects

A collection of **seven standalone embedded C programs** developed from university microcontroller laboratory exercises, covering digital I/O, PWM tone generation, analogue-to-digital conversion (ADC), and interrupts.

The examples target the **ATmega644P** on an **Il Matto** development board, assuming a **12 MHz** processor clock. They are separate experiments, **not** one combined firmware application. The code and accompanying build tools have not yet been collectively verified on physical hardware.

## Projects

| Folder | File | Description |
| --- | --- | --- |
| `Digital-IO` | `counter.c` | Counts 0–9 on a seven-segment display connected to Port A, with a software delay. |
| `PWM` | `tone.c` | Generates an approximately 262 Hz tone on OC1A (PD5) using Timer1 Phase and Frequency Correct PWM (Mode 9). |
| `ADC` | `voltage.c` | Measures ADC channel 0 and prints the raw 10-bit reading and calculated voltage via UART0. |
| `ADC` | `proximity.c` | Reads ADC channel 1 and controls an LED on PB7 using two thresholds (300 and 700), with UART output. |
| `Interrupts` | `timer.c` | Updates a seven-segment counter using Timer1 compare-match interrupts at approximately 1 Hz. |
| `Interrupts` | `adc.c` | Samples the ADC in free-running mode, controls PB7 at threshold 650, and toggles PB3 on each ADC interrupt. |
| `Interrupts` | `external.c` | Increments/decrements an 8-bit value using INT1/INT0 and outputs it on Port C. |

## Repository layout

```text
avr-embedded-systems/
├── Digital-IO/
│   └── counter.c
├── PWM/
│   └── tone.c
├── ADC/
│   ├── voltage.c
│   └── proximity.c
├── Interrupts/
│   ├── timer.c
│   ├── adc.c
│   └── external.c
├── common/
│   └── debug.h
├── Makefile
├── .gitignore
└── README.md
```

## Requirements

- AVR toolchain: `avr-gcc`, `avr-libc`, `avr-objcopy`, and GNU Make.
- ATmega644P development hardware and suitable connections for the selected exercise.
- For UART-based ADC examples: a **3.3 V-compatible** UART-to-USB interface or the board's compatible serial interface and a serial terminal configured to **9600 baud, 8-N-1**.

## Build

Run commands from the repository root:

```sh
make                    # Build all seven separate .elf and .hex programs
make list               # Display expected .hex output paths
make build/ADC/voltage.hex  # Build just one example
make clean              # Remove generated files
```

Outputs appear under `build/`, preserving each source folder name. The Makefile **does not flash** the microcontroller; use an appropriate programmer and board-specific upload procedure.

Default build settings are `MCU=atmega644p` and `F_CPU=12000000UL`. Override them only if your target actually differs, for example:

```sh
make F_CPU=12000000UL
```

Changing `F_CPU` alone will **not** automatically update every original lab calculation: `PWM/tone.c` contains a hardcoded 1.5 MHz timer clock, `Interrupts/timer.c` has a compare value calculated for 12 MHz, and ADC voltage conversion assumes a 3.3 V reference.

## UART debugging

`common/debug.h` is a **minimal replacement helper written for this repository**, not the original university-provided library. It supplies `init_debug_uart0()` and routes `printf()` to UART0 transmit (TX only). It uses the build clock definition and a 9600-baud UART setting.

The two ADC examples include `"debug.h"`; the Makefile supplies `-Icommon`. Their `%f` voltage output requires AVR-libc floating-point `printf`, enabled by the relevant Makefile target-specific linker flags.

To observe values, connect a compatible serial receiver to **UART0 TX (PD1)** and share ground. Check the board's electrical levels and any onboard UART routing before connecting external equipment. An ordinary RS-232 voltage-level interface must **not** be connected directly to the MCU pins.

## Hardware notes and limitations

- All seven sources contain their own `main()` function and must be built/flashed **individually**.
- Seven-segment patterns assume the laboratory's particular Port A segment wiring and current-limiting resistors.
- ADC programs assume a **3.3 V reference supplied to AREF** and use right-adjusted 10-bit results; confirm reference wiring on the actual board.
- `Interrupts/external.c` assumes **active-low** external switches on INT0/PD2 and INT1/PD3. Mechanical switch bounce is not filtered.
- Full Port C operation may require disabling JTAG, depending on microcontroller fuse settings.
- `Interrupts/adc.c` retains unused initialisation functions from the original laboratory submission. The ADC configuration actually used is in `main()`.
- The original timer and PWM programs should initialise their compare/TOP registers **before enabling the timer clock** to avoid startup transients.
- ADC proximity sampling using `_delay_ms(200)` is only **approximately** 5 samples/s because ADC conversion and UART output also take time.
- Physical wiring, compiler compatibility, and measured output frequencies have **not yet been verified** as part of this repository preparation.

## Hardware References

- **Microcontroller:** ATmega644P
- **Development Board:** Il Matto
- **Seven-Segment Display:** Kingbright SC08-11GWA
- **Display Datasheet:** [SC08-11GWA Datasheet](https://www.kingbrightusa.com/images/catalog/SPEC/sc08-11gwa.pdf)
