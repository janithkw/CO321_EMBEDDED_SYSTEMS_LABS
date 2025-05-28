// Write a C program to toggle an LED connected to pin 5 of PORTB register (PB5) every 2ms. Use TIMER0, normal mode, and a suitable pre-scalar to create the delay. Assume XTAL= 16 MHz. What is the selected prescaler? What is the initial counter value? Explain the output. What is the reason for it?

#include <avr/io.h>

void delay_timer0()
{

    TCNT0 = 225;  /*Load timer counter register*/
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
        delay_timer0();
    }
}


/*
XTAL = 16MHz -> Tclock = 1/16MHz = 62.5 ns
Prescaler = 1:1024 → Tcounter_clock = 1024 × 1/16 MHz = 64 μs


Counter increments needed = 2000μs / 64μs = 31.25
increments Initial counter value = 1+255 – 31.25  = 225 (rounded to nearest integer)
*/