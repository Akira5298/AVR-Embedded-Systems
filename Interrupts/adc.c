
#include <avr/io.h>
#include <avr/interrupt.h>

// Reserved for storing ADC readings; currently unused.
volatile uint16_t adc_value = 0; // Store ADC result

// Alternative ADC initialization function from the lab.
// Not called; main() configures the ADC directly in free-running mode.
void init_adc(void) {
    ADMUX = (1 << REFS0); // Reference voltage AVcc
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, interrupt, prescaler 64
    ADCSRA |= (1 << ADSC); // Start first conversion
}

// Optional LED initialization function from the lab.
// Not called; main() configures PB7 directly.
void init_led(void) {
    DDRB |= (1 << PB7); // Set PB7 as output
}

ISR(ADC_vect) {
    if(ADC > 650)
        PORTB |= _BV(PB7);
    else
        PORTB &= ~ _BV(PB7);
    PINB = _BV(PB3); // Changed from |= to =
}

int main(void) {
    DDRB |= _BV(PB3);
    DDRB |= _BV(PB7);
    PORTB &= ~_BV(PB7);
    /* Set ADC prescaler to 64 - 187.5kHz ADC clock */
    ADCSRA = _BV(ADPS2) | _BV(ADPS1);
    /* AREF reference, channel 0, right shift result */
    ADMUX = 0x00;
    /* Free-running mode and enable ADC */
    ADCSRA |= _BV(ADATE) | _BV(ADEN);
    ADCSRB = 0x00;
    /* Enable Interrupts */
    sei();
    ADCSRA |= _BV(ADIE);
    /* Start Conversions */
    ADCSRA |= _BV(ADSC);
    while(1);
}
