#include <avr/io.h>

void delay_timer0()
{

    TCNT0 = 116; /*Load timer counter register*/
    TIFR0 = 0x01; /*Clear the timer overflow bit (TOvO) for next round*/
    /*strange thing about this flag is that inorder to clear it we should write 1 to it.
    This rule applies to all flags of AVR chip*/
    
    TCCR0A = 0x00; /*Set the Timer0 under normal mode with 1:8 prescaler*/
    TCCR0B = 0x02;

    while ((TIFR0 & 0x01) == 0); /*Wait till timer overflow bit (TOVO) is set*/

    TCCR0B = 0x00;

    
}

int main(void){
    /* configure pin 5 of PORTB for output*/
        DDRB = DDRB | (1 << 5);

    while (1)
    {
        /* toggle pin 5 */
        PORTB = PORTB ^ (1 << 5);
        delay_timer0();
    }
}

