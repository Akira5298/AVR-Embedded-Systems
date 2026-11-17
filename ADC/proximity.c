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

// Function to select the ADC channel and perform a single ADC conversion
uint16_t read_adc(uint8_t channel)
{
    // Select ADC channel while preserving reference settings (clear previous selection and set new one)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // Start a single conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion is complete (ADSC bit clears automatically)
    while (ADCSRA & (1 << ADSC));

    // Return the 10-bit ADC result
    return ADC;
}

// Function to initialize PB7 as an output (LED control)
void init_led(void)
{
    DDRB |= (1 << PB7);
}

int main(void)
{
    uint16_t adc_value; // To store the ADC result
    double voltage; // To store calculated voltage

    const uint16_t Va = 700; // Threshold: No object nearby
    const uint16_t Vp = 300; // Threshold: Object detected

    // Initialize peripherals
    init_debug_uart0(); // Initialize UART for debugging
    init_adc(); // Initialize ADC
    init_led(); // Initialize LED

    for (;;)
    {
        // Read ADC value from channel 1 (PA1)
        adc_value = read_adc(1);

        // Convert the ADC reading to voltage (3.3V reference, 10-bit resolution)
        voltage = (adc_value / 1024.0) * 3.3;

        // Print ADC result and voltage through UART
        printf("%4u : %6.5fV\n", adc_value, voltage);

        // Turn LED on when an object is detected (when its nearby)
        if (adc_value < Vp)
        {
            PORTB |= (1 << PB7);
        }
        // Turn LED off when no object is nearby
        else if (adc_value > Va)
        {
            PORTB &= ~(1 << PB7);
        }

        // Sample approximately five times per second
        // Small delay for stability
        _delay_ms(200);
    }
}

