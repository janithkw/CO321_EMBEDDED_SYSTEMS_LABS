#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 100

int main(void) {
    // Configure PB2 to PB5 as output
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5); // Set PB2 to PB5 as output

    while (1) {
        // Move LED from PB2 to PB5
        for (int i = PB2; i <= PB5; i++) {
            PORTB = (1 << i); // Turn on the current LED
            _delay_ms(BLINK_DELAY_MS);
        }

        // Move LED from PB5 back to PB2
        for (int i = PB5; i >= PB2; i--) {
            PORTB = (1 << i); // Turn on the current LED
            _delay_ms(BLINK_DELAY_MS);
        }
    }

    return 0;
}