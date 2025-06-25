#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void ADC_init()
{
    // ADMUX = 0x00
    // → REFS1 = 0, REFS0 = 0 → External AREF
    // → ADLAR = 0 → Right-adjusted result
    // → MUX[3:0] = 0000 → ADC0 (pin A0 / PC0)
    ADMUX = 0x00;

    // ADCSRA = 0x86
    // → ADEN = 1 (ADC enable)
    // → ADSC = 0 (no conversion yet)
    // → ADATE = 0 (no auto trigger)
    // → ADIF = 0 (no flag)
    // → ADIE = 0 (no interrupt)
    // → ADPS2 = 1, ADPS1 = 1, ADPS0 = 0 → Prescaler = 128 (16MHz/64 = 250kHz)
    ADCSRA = 0x86;
}

uint16_t ADC_read()
{
    ADCSRA |= 0x40; // Start conversion (set ADSC)
    while (ADCSRA & 0x40)
        ;       // Wait until ADSC becomes 0 (conversion done)
    return ADC; // Return 10-bit result from ADCL + ADCH
}

int main(void)
{
    // DDRD = 0x01 → Set PD0 as output (LED)
    DDRD = 0x01;

    ADC_init();

    while (1)
    {
        uint16_t light = ADC_read();

        if (light > 100)
        {
            PORTD = 0x01; // Turn ON LED (PD0 = 1)
        }
        else
        {
            PORTD = 0x00; // Turn OFF LED (PD0 = 0)
        }

        _delay_ms(200); // Delay for stability
    }
}
