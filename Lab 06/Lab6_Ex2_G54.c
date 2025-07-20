//Write a C function called usart_init to initialize the USART to work at 9600 baud, 8 bit data, no parity, 1 stop bit.
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>

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

void usart_send_string(const char *str)
{
    // Send each character in the string until null terminator
    while (*str)
    {
        usart_send(*str);
        str++;
    }
}
int main(void)
{
    // Initialize USART
    usart_init(MYUBRR);

    // Small delay to ensure USART is ready
    _delay_ms(100);

    // Send initial information
    usart_send_string("=== USART Echo Test ===\r\n");
    usart_send_string("1. Samadhi Sakunika       - E/20/419\r\n");
    usart_send_string("2. Janith Wanasinghe      - E/20/420\r\n");
    usart_send_string("=======================\r\n");
    usart_send_string("Type anything and press Enter. Type 'exit' to quit.\r\n");
    usart_send_string(">> ");

    char received_char; // Variable to store received character
    char buffer[100];   // Buffer to store received string
    uint8_t buffer_index = 0;

    // Main loop - echo back any received character and handle commands
    while (1)
    {
        // Receive a character
        received_char = usart_receive();

        // Handle different characters
        if (received_char == '\r' || received_char == '\n') 
        {
            // End of line - process the received string
            buffer[buffer_index] = '\0'; // Null terminate the string
            
            usart_send_string("\r\nYou typed: ");
            usart_send_string(buffer);
            usart_send_string("\r\n");
            
            // Check for exit command
            if (strcmp(buffer, "exit") == 0) 
            {
                usart_send_string("Goodbye!\r\n");
                break;
            }
            
            // Reset buffer and show prompt
            buffer_index = 0;
            usart_send_string(">> ");
        }
        else if (received_char == '\b' || received_char == 127) 
        {
            // Backspace handling
            if (buffer_index > 0) 
            {
                buffer_index--;
                usart_send_string("\b \b"); // Move back, space, move back again
            }
        }
        else if (buffer_index < sizeof(buffer) - 1) 
        {
            // Regular character - add to buffer and echo
            buffer[buffer_index++] = received_char;
            usart_send(received_char); // Echo the character back
        }
    }

    return 0;
}