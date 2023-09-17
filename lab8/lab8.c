/********************************************
 *
 *  Name: Scott Susanto
 *  Email: scottsus@usc.edu
 *  Section: Wednesday 12:30pm
 *  Assignment: Lab 8 - Stopwatch
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "adc.h"

#define start TCCR1B |= (1 << CS12);
#define stop TCCR1B &= ~(1 << CS12);

void toZero();

// Initialize time variables
volatile unsigned char tens = 0;
volatile unsigned char ones = 0;
volatile unsigned char tenths = 0;
volatile unsigned char updateTens = 1;
volatile unsigned char updateOnes = 1;
volatile unsigned char updateTenths = 1;

enum states
{
    stopped,
    started,
    lapped
};

unsigned char state = stopped;
unsigned char updateDisplay = 1;

int main(void)
{
    // Initialize the LCD
    lcd_init();

    // Initialize the ADC
    adc_init();

    // Initialize the timer module
    TCCR1B |= (1 << WGM12);  // Clear Timer on Compare
    TIMSK1 |= (1 << OCIE1A); // Enable local interrupt
    sei();                   // Enable global interrupt
    OCR1A = 6250;            // Set clock cycle

    // Splash screen
    lcd_moveto(0, 4);
    lcd_stringout("Lab 08");
    lcd_moveto(1, 0);
    lcd_stringout("Scott Susanto");
    _delay_ms(1000);
    lcd_writecommand(1);

    while (1)
    {
        // Timer past 60s
        if (tens == 6)
        {
            toZero();
        }

        // Polling for button press
        unsigned char btnVal = adc_sample(0);
        if (btnVal != 255)
        {
            _delay_ms(100); // Debouncing
        }

        // Start_Stop button
        if (40 < btnVal && btnVal < 60)
        {
            if (state == stopped)
            {
                start;
                state = started;
            }
            else if (state == started)
            {
                stop;
                state = stopped;
            }
            else if (state == lapped)
            {
                updateDisplay = 1;
            }
        }

        // Lap_Reset button
        if (90 < btnVal && btnVal < 110)
        {
            if (state == stopped)
            {
                toZero();
            }
            else if (state == started)
            {
                updateDisplay = 0;
                state = lapped;
            }
            else if (state == lapped)
            {
                updateDisplay = 1;
                state = started;
            }
        }

        // Resetting button value
        btnVal = 255;

        // Updating the display
        if (updateDisplay)
        {
            if (updateTenths) // Updating the tenths value
            {
                char tenthsPrint[2];
                snprintf(tenthsPrint, 2, "%d", tenths);
                lcd_moveto(0, 3);
                lcd_stringout(tenthsPrint);
                updateTenths = 0;
            }

            if (updateOnes) // Updating the ones value
            {
                char onesPrint[3];
                snprintf(onesPrint, 3, "%d.", ones);
                lcd_moveto(0, 1);
                lcd_stringout(onesPrint);
                updateOnes = 0;
            }

            if (updateTens) // Updating the tens value
            {
                char tensPrint[2];
                snprintf(tensPrint, 2, "%d", tens);
                lcd_moveto(0, 0);
                lcd_stringout(tensPrint);
                updateTens = 0;
            }
        }
    }
}

// Helper function to reset the timer to 00.0
void toZero()
{
    tens = 0;
    ones = 0;
    tenths = 0;
    lcd_writecommand(1);

    char out[5];
    snprintf(out, 5, "%d%d.%d", tens, ones, tenths);
    lcd_moveto(0, 0);
    lcd_stringout(out);
}

// Interrupt
ISR(TIMER1_COMPA_vect)
{
    tenths++;
    updateTenths = 1;

    if (tenths >= 10)
    {
        ones++;
        tenths = 0;
        updateOnes = 1;
    }

    if (ones >= 10)
    {
        tens++;
        ones = 0;
        updateTens = 1;
    }
}
