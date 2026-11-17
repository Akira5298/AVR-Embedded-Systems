#ifndef AVR_LAB_DEBUG_H
#define AVR_LAB_DEBUG_H

/*
 * Minimal UART0 printf helper for ATmega644P AVR lab examples.
 * Assumes F_CPU = 12 MHz (or a build-provided override).
 * Sends text at 9600 baud, 8 data bits, no parity, 1 stop bit.
 * This is an independently written replacement for the missing course helper.
 */
#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#define DEBUG_UART_BAUD 9600UL
#define DEBUG_UART_UBRR ((F_CPU / (16UL * DEBUG_UART_BAUD)) - 1UL)

static int debug_uart_putchar(char character, FILE *stream)
{
    (void)stream;

    /* Send carriage return before newline for serial terminals. */
    if (character == '\n') {
        debug_uart_putchar('\r', stream);
    }

    while (!(UCSR0A & _BV(UDRE0))) {
        /* Wait for the transmit data register to become empty. */
    }

    UDR0 = (uint8_t)character;
    return 0;
}

static FILE debug_uart_stdout =
    FDEV_SETUP_STREAM(debug_uart_putchar, NULL, _FDEV_SETUP_WRITE);

static void init_debug_uart0(void)
{
    UBRR0H = (uint8_t)(DEBUG_UART_UBRR >> 8);
    UBRR0L = (uint8_t)DEBUG_UART_UBRR;

    UCSR0A = 0;
    UCSR0B = _BV(TXEN0);                  /* Enable UART0 transmitter. */
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8 data bits, no parity, 1 stop bit. */

    stdout = &debug_uart_stdout;
}

#endif /* AVR_LAB_DEBUG_H */
