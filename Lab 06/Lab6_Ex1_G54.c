//Write a C function called usart_init to initialize the USART to work at 9600 baud, 8 bit data, no parity, 1 stop bit.
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL             // 16MHz clock frequency
#define BAUD 9600                    // Desired baud rate
#define MYUBRR F_CPU / 16 / BAUD - 1 // Calculate UBRR value (= 103)

// Function to initialize USART
void usart_init(unsigned int ubrr)
{
    // Set baud rate registers
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

//usart_send to transmit a character given as an argument
void usart_send(unsigned char data)
{
    // Wait for empty transmit buffer (UDRE0 flag)
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Put data into buffer
    UDR0 = data;
}

// usart_receive to receive a character
unsigned char usart_receive(void)
{
    // Wait for data to be received (RXC0 flag)
    while (!(UCSR0A & (1 << RXC0)))
        ;

    // Get and return received data from buffer
    return UDR0;
}
