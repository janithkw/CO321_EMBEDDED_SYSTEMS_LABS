#include <avr/io.h>

void delay_timer0() {
    TCNT0 = 0x00;      // Reset timer count
    TIFR0 = 0x01;      // Clear overflow flag before starting
    TCCR0A = 0x00;     // Normal mode
    TCCR0B = 0x05;     // No prescaler, start timer

    while ((TIFR0 & 0x01) == 0) {
        // Wait for overflow
    }

    TCCR0B = 0x00;     // Stop timer
}

int main(void) {
    DDRB = 0x1F; // Set PB0–PB4 as output (0b00011111)

    while (1) {
        PORTB ^= 0x1F; // Toggle all bits of PORTB
        delay_timer0();
    }
}