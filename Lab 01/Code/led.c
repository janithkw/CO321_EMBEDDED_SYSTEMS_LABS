#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

int main(void) {
    // Configure PB2 to PB5 as output
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);

    while (1) {
        // Set PB2 to PB5 high to turn LEDs on
        PORTB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
        _delay_ms(BLINK_DELAY_MS);

        // Set PB2 to PB5 low to turn LEDs off
        PORTB &= ~((1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
        _delay_ms(BLINK_DELAY_MS);
    }

    return 0;
}