// You found out the maximum interval you can implement using TIMER0 in exercise 2. However, by using an 8-bit counter variable and interrupts, you can make this interval much longer. Develop a program to blink an LED with a 100ms interval using TIMER0 .

/*
Implement a delay of 12.5 miliseconds using TIMER0 in normal mode and a suitable pre-scaler. Assume XTAL= 16MHz. What is the selected prescaler? What is the initial counter value? Explain the output. What is the reason for it?
XTAL = 16MHz -> Tclock = 1/16MHz = 62.5 ns
Prescaler = 1:1024 → Tcounter_clock = 1024 × 1/16 MHz = 64 μs


Counter increments needed = 12500 μs / 64μs = 195.3125 

Maximum possible counter increment = 256
Initial counter value = 1 + 255 - 195.3125 = 60.6875 (rounded to nearest integer, which is 61)

Therefore, such counters are went on 8 times to achieve 100ms delay.

*/

#include <avr/io.h>

void delay_timer0()
{

    TCNT0 = 61;  /*Load timer counter register*/
    TIFR0 = 0x01; /*Clear the timer overflow bit (TOvO) for next round*/
    /*strange thing about this flag is that inorder to clear it we should write 1 to it.
    This rule applies to all flags of AVR chip*/

    TCCR0A = 0x00; /*Set the Timer0 under normal mode with 1:1024 prescaler*/
    TCCR0B = 0x05; /*Prescaler set to 1024 (1:1024)*/

    while ((TIFR0 & 0x01) == 0)
        ; /*Wait till timer overflow bit (TOVO) is set*/

    TCCR0B = 0x00;
}

int main(void)
{
    /* configure pin 5 of PORTB for output*/
    DDRB = DDRB | (1 << 5);

    while (1)
    {
        /* toggle pin 5 */
        PORTB = PORTB ^ (1 << 5);
        
        for (int i = 0; i < 8; i++) // Repeat the delay 8times to achieve approximately 100ms delay
        {
            delay_timer0();
        }
    }
}