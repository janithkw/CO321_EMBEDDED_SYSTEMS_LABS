#include <avr/io.h>
#include <avr/interrupt.h>


// TIMER0: 50ms blink (prescaler 1024, 1 overflow = 256*64us = 16.384ms, need 3 overflows for ~50ms)
//Using 12.5 ms delay, we can achieve 50ms by toggling the LED every 4 overflows
//Starter count = 61 (256 - 195) to achieve ~12.5ms delay as one increment takes 64us, so 64us * 195 = 12.48ms
#define TIMER0_INIT 61   // Start from 61 to acheive ~50ms delay (4*12.5ms = 50ms)
volatile uint8_t t0_count = 0; 

// TIMER1: 500ms blink (prescaler 256, 1 overflow = 65536*64us = 4.194s, need to preload)
/*
XTAL = 16MHz -> Tclock = 1/16MHz = 62.5 ns
Prescaler = 1:256 → Tcounter_clock = 256 × 1/16 MHz = 16 μs


Counter increments needed = 500000μs / 16μs = 31250 

Maximum possible counter increment = 65536
Initial counter value = 1 + 65535 - 31250 = 34387 (rounded to nearest integer, which is 34387)

*/
#define TIMER1_INIT 34387 // Start from 34387 to achieve ~500ms delay

void setup_timer0() {
    TCCR0A = 0x00; // Normal mode
    TCCR0B = 0x05; // Prescaler 1024
    TCNT0 = TIMER0_INIT;
    TIMSK0 = 0x01; // Enable TIMER0 overflow interrupt
}

void setup_timer1() {
    TCCR1A = 0x00; // Normal mode
    TCCR1B = 0x04; // Prescaler 1024
    TCNT1 = TIMER1_INIT;
    TIMSK1 = 0x01; // Enable TIMER1 overflow interrupt
}

ISR(TIMER0_OVF_vect) {
    TCNT0 = TIMER0_INIT; // Reload which will give us ~12.5ms
    t0_count++;
    if (t0_count >= 4) { // ~50ms (4 * 12.5ms)
        // Toggle LED every 50ms
        PORTB ^= (1 << PB5); // Toggle LED
        t0_count = 0;
    }
}

ISR(TIMER1_OVF_vect) {
    TCNT1 = TIMER1_INIT; // Reload for 500ms
    PORTB ^= (1 << PB4); // Toggle LED
}

int main(void) {
    // Set PB4 and PB5 as output
    DDRB |= (1 << PB5) | (1 << PB4);
    PORTB &= ~((1 << PB5) | (1 << PB4)); // LEDs off

    setup_timer0();
    setup_timer1();
    sei(); // Enable global interrupts

    while (1) {
        // Main loop does nothing, all handled by interrupts
    }
}