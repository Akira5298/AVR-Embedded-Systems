#ifndef F_CPU
#define F_CPU 12000000UL  // ATmega644P clock frequency: 12 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Seven-segment display patterns for digits 0–9
static const uint8_t segments[10] = {
    0xFC, // 0
    0x60, // 1
    0xDA, // 2
    0xF2, // 3
    0x66, // 4
    0xB6, // 5
    0xBE, // 6
    0xE0, // 7
    0xFE, // 8
    0xF6  // 9
};

int main(void) {
    // Configure Port A as output
    DDRA = 0xFF;

    uint8_t count = 0;

    while (1) {
        // Display the current digit
        PORTA = segments[count];

        // Wait for one second
        _delay_ms(1000);

        // Increment and wrap around from 9 to 0
        count = (count + 1) % 10;
    }

    return 0;
}
