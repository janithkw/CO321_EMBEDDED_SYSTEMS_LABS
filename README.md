# CO321 Embedded Systems Labs

This repository contains lab materials, code, and resources for the CO321 Embedded Systems course conducted by the Department of Computer Engineering, Faculty of Engineering, University of Peradeniya.

## Repository Structure

```
.
├── Lab 01/
│   ├── CO321 Lab 1 - Programming AVR Microcontrollers.pdf

│   ├── Tutorial 1.pdf
│   └── Code/
│       ├── led.c
│       ├── led
│       ├── led_knight_rider.c
│       └── led_knight_rider
├── Lab 02/
│   ├── code/
│   │   ├── Lab2_Task1_G54.c
│   │   ├── Lab2_Task2_G54.c
│   │   ├── Lab2_Task3_G54.c
│   │   ├── Lab2_Task4_G54.c
│   └── Lab 2 materials/
│       └── CO321 Lab 2 - Interrupts - Self Study.pdf
│       └── CO321 Lab 2 - Interrupts.pdf
├── Lab 03/
│   ├── CO321 Lab 3 - Timers..pdf
│   └── Code/
│       └── Lab3/
│           ├── src/
│           │   ├── exercise1.c
│           │   ├── exercise2.c
│           │   ├── exercise3.c
│           │   ├── exercise4.c
│           │   ├── exercise5.c
│           │   ├── exercise6.c
│           │   ├── example1.c
│           │   └── example2.c

├── Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
├── README.md
└── .gitignore
```

## Lab Summaries

### Lab 01: Basic LED Control
- **Objective:** Introduction to AVR microcontroller programming, basic I/O, and LED control.
- **Code:** Simple C programs to blink LEDs and implement a "Knight Rider" LED pattern using delays.

### Lab 02: Interrupts and Button Inputs
- **Objective:** Learn to use external interrupts and button inputs to control LEDs and counters.
- **Code:** 
  - `Lab2_Task1_G54.c`: Button press counter displayed on LEDs.
  - `Lab2_Task2_G54.c`: Toggle LED using external interrupt.
  - `Lab2_Task3_G54.c`: Interrupt-driven counter with debounce.
  - `Lab2_Task4_G54.c`: Binary number entry using two buttons and interrupts.

### Lab 03: Timers and Delays
- **Objective:** Explore AVR timers in normal mode, generate precise delays, and use timer interrupts.
- **Code:** 
  - `exercise1.c` to `exercise6.c`: Timer-based LED blinking, maximum interval calculations, and advanced timer usage.
  - `example1.c`, `example2.c`: Reference timer delay implementations.

### Common Files
- **Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf:** Reference datasheet for the ATmega328P microcontroller.
- **.gitignore:** Ignores build artifacts, binaries, and other non-source files.

## Building and Running

For Labs, PlatformIO is used for building and uploading code:

1. Install [PlatformIO](https://platformio.org/) in VS Code.
2. Open the folder where platformio.ini is located.
3. Select the desired source file in `platformio.ini` using the `build_src_filter` option.
4. Build and upload to your Arduino Uno board.

## Notes

- All code is written for the ATmega328P microcontroller (Arduino Uno).
- Each lab folder contains relevant documentation, code, and binaries.
- For questions, refer to the provided lab PDFs or contact the course instructor.

---

*This repository is for educational purposes as part of the CO321 Embedded Systems course at the University of Peradeniya.*
