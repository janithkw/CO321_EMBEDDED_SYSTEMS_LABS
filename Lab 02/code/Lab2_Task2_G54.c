#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100

int main() {
    DDRD &= ~(1 << 2);  // Set PD2 (INT0) as input
    PORTD |= (1 << 2);  // Enable internal pull-up resistor on PD2

    DDRB |= (1 << PB0); // Set PB0 as output

    // Configure INT0 to trigger on rising edge (button release)
    EICRA |= (1 << ISC01) | (1 << ISC00); // Set ISC01 and ISC00 to 1 (rising edge)

    sei(); // Enable global interrupts

    EIMSK |= (1 << INT0); // Enable INT0 interrupt

    while (1) {
        // Main loop does nothing, waiting for interrupts
    }

    return 0;
}

// Interrupt Service Routine for INT0
ISR(INT0_vect) {
    PORTB ^= (1 << PB0); // Toggle PB0
}