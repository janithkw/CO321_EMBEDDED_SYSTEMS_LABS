#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// ==== EEPROM functions ====
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

#define EEPROM_KEY_ADDR 0x03FF

// ==== Caesar Cipher Logic ====
char caesar_encrypt(char c, int key)
{
    if (c >= 'A' && c <= 'Z')
        return ((c - 'A' + key) % 26) + 'A';
    else if (c >= 'a' && c <= 'z')
        return ((c - 'a' + key) % 26) + 'a';
    else
        return c; // Leave symbols/numbers unchanged
}

// ==== LCD (4-bit Mode, PORTD) ====

#define LCD_Dir DDRD
#define LCD_Port PORTD
#define RS PD0
#define EN PD1

void lcd_command(unsigned char cmd)
{
    LCD_Port = (LCD_Port & 0x0F) | (cmd & 0xF0);
    LCD_Port &= ~(1 << RS); // RS = 0 for command
    LCD_Port |= (1 << EN);
    _delay_us(1);
    LCD_Port &= ~(1 << EN);
    _delay_us(200);

    LCD_Port = (LCD_Port & 0x0F) | (cmd << 4);
    LCD_Port |= (1 << EN);
    _delay_us(1);
    LCD_Port &= ~(1 << EN);
    _delay_ms(2);
}

void lcd_char(unsigned char data)
{
    LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);
    LCD_Port |= (1 << RS); // RS = 1 for data
    LCD_Port |= (1 << EN);
    _delay_us(1);
    LCD_Port &= ~(1 << EN);
    _delay_us(200);

    LCD_Port = (LCD_Port & 0x0F) | (data << 4);
    LCD_Port |= (1 << EN);
    _delay_us(1);
    LCD_Port &= ~(1 << EN);
    _delay_ms(2);
}

void lcd_init()
{
    LCD_Dir = 0xFF;
    _delay_ms(20);
    lcd_command(0x02); // 4-bit mode
    lcd_command(0x28); // 2-line, 5x8 font
    lcd_command(0x0C); // Display ON, Cursor OFF
    lcd_command(0x06); // Increment cursor
    lcd_command(0x01); // Clear display
    _delay_ms(2);
}

void lcd_string(const char *str)
{
    while (*str)
        lcd_char(*str++);
}

void lcd_clear()
{
    lcd_command(0x01);
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    lcd_command(0x80 + (row * 0x40) + col);
}

// ==== Keypad (PORTC: PC0-PC3 = rows, PC4-PC7 = cols) ====

char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

void keypad_init()
{
    DDRC = 0x0F;  // PC0-3 intput (rows), PC4-7 input (cols)
    PORTC = 0xF0; // Enable pull-up on columns
}

char keypad_get_key()
{
    for (int row = 0; row < 4; row++)
    {
        PORTC = ~(1 << row); // Pull one row LOW
        _delay_us(5);
        for (int col = 0; col < 4; col++)
        {
            if (!(PINC & (1 << (col + 4))))
            {
                _delay_ms(50); // Debounce
                while (!(PINC & (1 << (col + 4))))
                    ;
                return keypad[row][col];
            }
        }
    }
    return 0;
}

// ==== Main ====

int main(void)
{
    lcd_init();
    keypad_init();
    _delay_ms(100);

    // Load Caesar key from EEPROM (or default to 3)
    char key = EEPROMread(EEPROM_KEY_ADDR);
    if (key == 0xFF || key > 25)
        key = 3;

    while (1)
    {
        lcd_clear();
        lcd_string("1:Encrypt 2:Key");
        char option = 0;

        // Wait for user input
        while (!(option = keypad_get_key()))
            ;

        // Option 1: Encrypt message
        if (option == '1')
        {
            lcd_clear();
            lcd_string("Enter text:");
            lcd_set_cursor(1, 0);

            char input[33];
            char output[33];
            int idx = 0;

            while (idx < 32)
            {
                char k = keypad_get_key();
                if (k == '#')
                    break;

                // Only encrypt letters
                input[idx] = k;
                char encrypted = caesar_encrypt(k, key);
                output[idx++] = encrypted;
                lcd_char(encrypted);
            }

            input[idx] = '\0';
            output[idx] = '\0';

            _delay_ms(2000);
            lcd_clear();
        }

        // Option 2: Change Caesar key
        else if (option == '2')
        {
            lcd_clear();
            lcd_string("New key (1-9):");

            char k = 0;
            while (!(k = keypad_get_key()))
                ;

            if (k >= '1' && k <= '9')
            {
                key = k - '0';
                EEPROMwrite(EEPROM_KEY_ADDR, key);
                lcd_clear();
                lcd_string("Key saved: ");
                lcd_char(k);
                _delay_ms(1500);
            }
            else
            {
                lcd_clear();
                lcd_string("Invalid key!");
                _delay_ms(1500);
            }
        }

        // Optional: Any other key press
        else
        {
            lcd_clear();
            lcd_string("Invalid Option");
            _delay_ms(1500);
        }
    }
}
