#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t value = 0; // 8-bit value

ISR(INT0_vect) {
    if (value > 0) value--; // Decrement on INT0
}

ISR(INT1_vect) {
    if (value < 255) value++; // Increment on INT1
}

int main(void) {
    // Set Port C as output for the 8-bit value
    DDRC = 0xFF;

    // Configure external interrupts
    EIMSK |= (1 << INT0) | (1 << INT1); // Enable INT0 and INT1
    EICRA |= (1 << ISC01) | (1 << ISC11); // Falling edge for INT0 and INT1

    sei(); // Enable global interrupts

    while (1) {
        PORTC = value; // Output value to Port C
    }
}
