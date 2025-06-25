#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>

void ADC_init()
{
    // ADMUX : REFS0=1(external capacitor as the reference), ADLAR=1(for left justification), MUX0=1 → 0b01100001 = 0x61
    ADMUX = 0x61; // 0b01100001 = 0x61

    // ADCSRA : ADEN=1(ADC enabeling), ADPS2=1, ADPS1=1(setting the prescaller = 128 - 125kHz) → 0b10000110 = 0x86
    ADCSRA = 0x87; // 0b10000111 = 0x87
}

uint8_t ADC_read()
{
    ADCSRA |= 0x40; // ADSC = 1 (for start the conversion) (0b01000000 = 0x40)
    while (ADCSRA & 0x40)
        ;        // Wait until ADSC = 0 (conversion complete)
    return ADCH; // Read upper 8 bits (left-adjusted)
}

int main(void)
{
    DDRD = 0xFF; // Set PORTD as output
    ADC_init();  // Initialize ADC

    while (1)
    {
        uint8_t adc_value = ADC_read(); // Read ADC value
        PORTD = adc_value;              // Output value to LEDs
        _delay_ms(100);                 // Delay for visibility
    }
}
