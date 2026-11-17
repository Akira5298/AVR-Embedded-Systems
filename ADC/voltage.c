
#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#include <avr/io.h> // For AVR device-specific registers
#include <util/delay.h> // For delay functions
#include <stdint.h>
#include <stdio.h>
#include "debug.h" // For debug UART functions

// Function to initialize the ADC
void init_adc(void)
{
    // Use external AREF (3.3V), channel 0, 10-bit result
    ADMUX = 0x00;

    // Enable ADC with prescaler 64 (187.5 kHz ADC clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

// Function to perform a single ADC conversion and return the 10-bit result
uint16_t read_adc(void)
{
    // Start a single conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete (ADSC bit clears automatically)
    while (ADCSRA & (1 << ADSC));

    // Return the 10-bit ADC result
    return ADC;
}

int main(void)
{
    uint16_t result; // To hold the raw ADC result
    double voltage; // To hold the converted voltage value

    // Initialize UART for debugging and ADC
    init_debug_uart0();
    init_adc();

    for (;;)
    {
        // Read the ADC result
        result = read_adc();

        // Convert the raw ADC result to voltage
        voltage = (result / 1024.0) * 3.3; // Using 3.3V as the reference voltage

        // Print ADC result and measured voltage
        printf("%4u : %6.5fV\n", result, voltage);

        // Wait for 1 second
        _delay_ms(1000);
    }
}
