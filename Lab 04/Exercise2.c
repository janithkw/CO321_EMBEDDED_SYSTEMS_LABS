#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= (1 << PD6);                                    // Set PORTD pin 7 as output
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Non-inverting, Fast PWM
    TCCR0B |= (1 << CS01) | (1 << CS00);                   // Clock prescaler 64 → gives ~976.56Hz

    while (1)
    {
        for (int i = 128; i >= 0; i--)
        {
            OCR0A = i;     // Set duty cycle
            _delay_ms(10); // Delay to observe the change
        }
        _delay_ms(100);
        for (int i = 0; i <= 128; i++)
        {
            OCR0A = i;     // Set duty cycle
            _delay_ms(10); // Delay to observe the change
        } // Clock prescaler 8 → gives ~976.56Hz
    }
}
