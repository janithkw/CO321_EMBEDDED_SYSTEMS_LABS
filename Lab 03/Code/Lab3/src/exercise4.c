/*
XTAL = 16MHz -> Tclock = 1/16MHz = 62.5 ns
Prescaler = 1:256 → Tcounter_clock = 256 × 1/16 MHz = 16 μs


Counter increments needed = 1000000μs / 16μs = 62500

Maximum possible counter increment = 65536
Initial counter value = 1 + 65535 - 62500 = 3036 (rounded to nearest integer)


*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 100
#define BITMASK (1 << PB5) // Bitmask for PB5

volatile uint8_t k = 0;

void knight_rider(){

    // Move LED from PB1 to PB4
    for (int i = PB1; i <= PB4; i++)
    {
        PORTB = (PORTB & BITMASK) | (1 << i); // Turn on the current LED
        _delay_ms(BLINK_DELAY_MS);
    }

    // Move LED from PB4 back to PB1
    for (int i = PB4; i >= PB1; i--)
    {
        PORTB = (PORTB & BITMASK) | (1 << i); // Turn on the current LED
        _delay_ms(BLINK_DELAY_MS);
    }
}


void setup_timer1()
{
    TCNT1 = 3036; // Initial value for Timer1 to achieve approximately 100ms delay
    TCCR1A = 0x00;  // Normal mode
    TCCR1B = 0x04;  // prescaler = 256
    TIMSK1 = 0x01; // enable overflow interrupt
   
}

ISR(TIMER1_OVF_vect)
{
    DDRB |= (1 << PB5); // Set PB5 as output
    TCNT1 = 3036;        // reload
    PORTB ^= (1 << PB5); // toggle LED
}

int main(void)
{
    // Configure PB1 to PB4 as output
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5); // Set PB1 to PB5 as output
    sei();                                                                  // global interrupt enable
    setup_timer1(); // Setup Timer1 for overflow interrupt
    PORTB = 0; // Initialize PORTB to 0
    while (1)
    {
        knight_rider(); // Call the knight rider function to blink LEDs
        _delay_ms(BLINK_DELAY_MS); // Delay between cycles
    }

    return 0;
}

