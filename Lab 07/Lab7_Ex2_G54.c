#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// EEPROM functions
void EEPROMwrite(unsigned int address, unsigned char data)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = address;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

unsigned char EEPROMread(unsigned int address)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = address;
    EECR |= (1 << EERE);
    return EEDR;
}

// UART functions
void UART_init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_send(char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

char UART_receive()
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

void UART_send_string(const char *str)
{
    while (*str)
    {
        UART_send(*str++);
    }
}

// EEPROM utilities
void store_sentence_to_EEPROM()
{
    unsigned int addr = 0;
    char c;

    UART_send_string("Enter sentence (ends with \\r):\r\n");

    while (1)
    {
        c = UART_receive();
        EEPROMwrite(addr++, c);
        if (c == '\r')
            break;
    }

    UART_send_string("Sentence stored in EEPROM.\r\n");
}

void read_EEPROM_to_serial()
{
    UART_send_string("Reading EEPROM:\r\n");

    for (unsigned int i = 0; i < 1024; i++)
    {
        char c = EEPROMread(i);
        if (c == '\r')
            break; // Stop reading after sentence ends
        UART_send(c);
    }

    UART_send_string("\r\nDone.\r\n");
}

// Main
int main(void)
{
    UART_init(103); // 9600 baud rate for 16MHz clock

    _delay_ms(100); // Wait for UART ready

    store_sentence_to_EEPROM();
    _delay_ms(1000); // Delay before reading
    read_EEPROM_to_serial();

    while (1)
        ; // Loop forever
}
