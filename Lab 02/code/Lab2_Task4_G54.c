#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAX_BITS 6          // Maximum number of bits for the binary number
#define DEBOUNCE_DELAY_MS 150 // Debounce delay in milliseconds

volatile uint8_t binary_number = 0; // Variable to store the binary number
volatile uint8_t bit_position = 0;  // Current bit position (0 to 5)

int main() {
    // Configure PD2 (INT0) and PD3 (INT1) as inputs for buttons A and B
    DDRD &= ~((1 << PD2) | (1 << PD3)); // Set PD2 and PD3 as inputs
    PORTD |= (1 << PD2) | (1 << PD3);   // Enable internal pull-up resistors on PD2 and PD3

    // Configure PB0 to PB5 as outputs for LEDs
    DDRB |= 0x3F; // Set PB0 to PB5 as outputs

    PORTB = 0;    // Ensure all LEDs are off initially

    // Configure INT0 (PD2) and INT1 (PD3) for falling edge detection
    EICRA |= (1 << ISC01); // Falling edge for INT0 (button A)
    EICRA &= ~(1 << ISC00);

    EICRA |= (1 << ISC11); // Falling edge for INT1 (button B)
    EICRA &= ~(1 << ISC10);

    // Enable external interrupts INT0 and INT1
    EIMSK |= (1 << INT0) | (1 << INT1);

    sei(); // Enable global interrupts

    while (1) {
        // Main loop does nothing, waiting for interrupts
    }

    return 0;
}

// Interrupt Service Routine for INT0 (Button A pressed)
ISR(INT0_vect) {
    _delay_ms(DEBOUNCE_DELAY_MS); // Debounce delay
    if (!(PIND & (1 << PD2))) {   // Check if the button is still pressed
        if (bit_position < MAX_BITS) { // Check if within the 6-bit limit
            binary_number &= ~(1 << bit_position); // Set the current bit to 0
            bit_position++; // Move to the next bit position
            PORTB = binary_number; // Update LEDs
        }
    }
}

// Interrupt Service Routine for INT1 (Button B pressed)
ISR(INT1_vect) {
    _delay_ms(DEBOUNCE_DELAY_MS); // Debounce delay
    if (!(PIND & (1 << PD3))) {   // Check if the button is still pressed
        if (bit_position < MAX_BITS) { // Check if within the 6-bit limit
            binary_number |= (1 << bit_position); // Set the current bit to 1
            bit_position++; // Move to the next bit position
            PORTB = binary_number; // Update LEDs
        }
    }
}