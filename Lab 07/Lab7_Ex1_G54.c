#include <avr/io.h>

void EEPROMwrite(unsigned int address, unsigned char data)
{
    // Wait for previous write to complete
    while (EECR & (1 << EEPE));

    // Set up address and data
    EEAR = address;
    EEDR = data;

    // Write logical one to EEMPE
    EECR |= (1 << EEMPE);

    // Start eeprom write by setting EEPE
    EECR |= (1 << EEPE);
}

unsigned char EEPROMread(unsigned int address)
{
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set up address
    EEAR = address;

    // Start eeprom read by writing EERE
    EECR |= (1 << EERE);

    // Return data from Data Register
    return EEDR;
}