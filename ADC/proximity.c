#includ#include <avr/io.h>       // For AVR device-specific registers

#include <util/delay.h>   // For delay functions

#include "debug.h"        // For debug UART functions

 

// Function to initialize the ADC

void init_adc(void)

{

    // Set reference voltage to AVcc (3.3V) and left adjust result

    ADMUX = (1 << REFS0);  // REFS0 = 1 for AVcc as reference, ADLAR = 0 for 10-bit result

 

    // Enable ADC and set pre-scaler to 64 for a 200kHz ADC clock

    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

}

 

// Function to select the ADC channel and perform a single ADC conversion

uint16_t read_adc(uint8_t channel)

{

    // Select ADC channel (clear previous selection and set new one)

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

 

    // Start a single conversion

    ADCSRA |= (1 << ADSC);

 

    // Wait for the conversion to complete (ADSC bit clears automatically)

    while (ADCSRA & (1 << ADSC));

 

    // Return the 10-bit ADC result

    return ADC;

}

 

// Function to initialize PB7 as an output (LED control)

void init_led(void)

{

    DDRB |= (1 << PB7); // Set PB7 as output

}

 

int main(void)

{

    uint16_t adc_value;    // To store the ADC result

    double voltage;        // To store calculated voltage

    const uint16_t Va = 700; // Threshold: No object nearby

    const uint16_t Vp = 300; // Threshold: Object detected

 

    // Initialize peripherals

    init_debug_uart0(); // Initialize UART for debugging

    init_adc();         // Initialize ADC

    init_led();         // Initialize LED

 

    for (;;)

    {

        // Read the ADC value from channel 1 (PA1)

        adc_value = read_adc(1);

 

        // Convert the ADC result to a voltage (3.3V reference, 10-bit resolution)

        voltage = (adc_value / 1024.0) * 3.3;

 

        // Print the ADC result and corresponding voltage to UART

        printf("%4d : %6.5fV\n", adc_value, voltage);

 

        // Check if the value indicates an object nearby and control the LED

        if (adc_value < Vp)

        {

            PORTB |= (1 << PB7); // Turn ON LED

        }

        else if (adc_value > Va)

        {

            PORTB &= ~(1 << PB7); // Turn OFF LED

        }

 

        // Small delay for stability

        _delay_ms(1000);

    }

}
