#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100
#define MAX_COUNT 64
#define DEBOUNCE_DELAY_MS 150


volatile uint8_t count = 0; //Counter to count the interrupt

int main() {
    DDRD &= ~(1 << 2);  // Set PD2 (INT0) as input
    PORTD |= (1 << 2);  // Enable internal pull-up resistor on PD2

    DDRB |= 0x3F; // Set PB0 to PB5 as output (for 6 LEDs)
    PORTB = 0; // Ensure all LEDs are off initially


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
// Interrupt Service Routine for INT0
ISR(INT0_vect) {
    _delay_ms(DEBOUNCE_DELAY_MS); // Debounce delay
    if (PIND & (1 << 2)) { // Check if the button is still released (rising edge confirmed)
        count++; // Increment the counter
        if (count >= MAX_COUNT) {
            count = 0; // Reset the counter if it exceeds the maximum value
        }
        PORTB = count & 0x3F; // Display the count on LEDs (6 LSBs)
    }
}