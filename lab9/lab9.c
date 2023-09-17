/********************************************
 *
 *  Name: Scott Susanto
 *  Email: scottsus@usc.edu
 *  Section: Wednesday 12pm
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

#define FOSC 16000000                 // Clock frequency
#define BAUD 9600                     // Baud rate used
#define MYUBRR (FOSC / 16 / BAUD - 1) // Value for UBRR0 register

char *messages[] = {
    "Hello           ",
    "How are you?    ",
    "EE109 is NOT FUN",
    "Redekopp BEST   ",
    "Custom Message 5",
    "Custom Message 6",
    "Custom Message 7",
    "Custom Message 8",
    "Hi Wilson       ",
    "Custom Message t",
};

// Initialize global variables
volatile char buffer[17];
volatile unsigned char index = 0;
volatile unsigned char msgRdy = 0;

int main(void)
{
    // Initialize the LCD, ADC and serial modules
    lcd_init();
    adc_init();
    serial_init(MYUBRR);

    // Enable interrupts
    UCSR0B |= (1 << RXCIE0);
    sei();

    // Show the splash screen
    lcd_moveto(0, 4);
    lcd_stringout("Lab 09");
    lcd_moveto(1, 0);
    lcd_stringout("Scott Susanto");
    _delay_ms(1000);
    lcd_writecommand(1);

    // Initialize local variables
    unsigned char selected = 0;

    while (1)
    { // Loop forever

        // Get an ADC sample to read buttons
        unsigned char val = adc_sample(0);

        // Up button pressed?
        if (40 < val && val < 60)
        {
            if (selected == 0)
            {
                selected = 10;
            }
            selected--;
            _delay_ms(250);
            lcd_writecommand(1);
        }

        // Down button pressed?
        if (90 < val && val < 110)
        {
            selected++;
            selected %= 10;
            _delay_ms(250);
            lcd_writecommand(1);
        }

        // Select button pressed?
        if (190 < val && val < 210)
        {
            // Transmit selected message
            serial_stringout(messages[selected]);
        }

        lcd_moveto(0, 0);
        lcd_stringout(messages[selected]);

        // Message received from remote device?
        if (msgRdy)
        {
            lcd_moveto(1, 0);
            lcd_stringout(buffer);
            msgRdy = 0;
            index = 0;
        }
    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{
    // Set up USART0 registers
    UBRR0 = ubrr_value;
    UCSR0C = (3 << UCSZ00);
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);

    // Enable tri-state buffer
    DDRD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
        // Busy looping
    }
    UDR0 = ch;
}

void serial_stringout(char *s)
{
    // Call serial_txchar in loop to send a string
    // (Very similar to lcd_stringout in lcd.c)
    unsigned char i;
    for (i = 0; i < 16; i++)
    {
        serial_txchar(s[i]);
    }
}

ISR(USART_RX_vect)
{
    // Handle received character
    char ch;
    ch = UDR0;
    buffer[index++] = ch;
    if (index == 16)
    {
        msgRdy = 1;
    }
}
