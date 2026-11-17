#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"

volatile uint8_t count = 0; // Shared counter variable

const uint8_t segments[10] = {
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

// Timer 1 Compare Match Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
    PORTA = segments[count]; // Update the display
    count++;
    if (count > 9) count = 0; // Reset after 9
}

int main(void) {
    // Initialize UART for debugging
    init_debug_uart0();

    // Set Port A as output for the 7-segment display
    DDRA = 0xFF;

    // Configure Timer 1
    TCCR1B |= (1 << WGM12) | (1 << CS12); // CTC mode, prescaler 256
    OCR1A = 62499;                        // Set compare value for 1s delay
    TIMSK1 |= (1 << OCIE1A);              // Enable Timer Compare Interrupt

    sei(); // Enable global interrupts

    // Main loop
    while (1) {
        // Spin in the loop, all work done in ISR
    }
}
