#ifndef F_CPU
#define F_CPU 12000000UL // Clock frequency of 12 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

#define FREQ 262  // Desired tone frequency (262 Hz)

unsigned long toneClock;  /* Set at timer initialisation and used
                           to calculate TOP when setting frequency */

void init_tone(void);
void tone(uint16_t frequency);

int main(void) {
    init_tone();  // Fixed missing semicolon

    tone(FREQ);   // Set the frequency once

    for (;;) {
        // Timer1 generates the tone automatically
    }
}

/* init_tone ---
   Setup timer1 as a tone frequency oscillator.
*/

void init_tone(void)
{
    DDRD |= _BV(PD5);  // Set PD5 (OC1A) as output

    // Configure Timer1 for Phase and Frequency Correct PWM (Mode 9)
    TCCR1A = _BV(COM1A0) | _BV(WGM10); // Toggle OC1A on compare match, WGM10 = 1

    // Prescaler = 8
    TCCR1B = _BV(WGM13) | _BV(CS11); // WGM13 = 1, Prescaler = 8
}

/* tone ---
   Set oscillator output to desired frequency
*/
void tone(uint16_t frequency)
{
    // Timer clock
    toneClock = 1500000; //F_CPU divided by prescaler = 12 MHz / 8 = 1.5 MHz

    // Calculate TOP for the desired frequency
    uint16_t topValue = toneClock / (4 * frequency);

    // Set TOP value in OCR1A (Mode 9 uses OCR1A as TOP)
    OCR1A = topValue;
}

