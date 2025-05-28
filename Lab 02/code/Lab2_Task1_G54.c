#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 100

int main() {
    DDRD &= ~(1 << 7);  // Set PD7 as input
    PORTD |= (1 << 7);  // Enable internal pull-up resistor on PD7

    // Set PORTB for 6 LEDs PB0 to PB5
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5); // Set PB0 to PB5 as output
    PORTB = 0; // Ensure all LEDs are off initially

    uint8_t count = 0; // Counter to track button presses
    uint8_t button_pressed = 0; // Flag to track button state

    while (1) {
        if (!(PIND & (1 << 7))) { // Check if button is pressed (active low)
            if (!button_pressed) { // Check if this is a new press
                
                if (!(PIND & (1 << 7))) { // Confirm button is still pressed
                    count++; // Increment the counter
                    PORTB = count & 0x3F; // Display the count on LEDs (6 LSBs) 0x3F means 111111
                    button_pressed = 1; // Set the button pressed flag
                }
            }
        } else {
            button_pressed = 0; // Reset the button pressed flag when button is released
        }
    }
}