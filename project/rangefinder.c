#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "rangefinder.h"

#define ACQUIRE (1 << PB4)
#define TRIGGER (1 << PD2)
#define STARTT1 TCCR1B |= ((1 << CS11) | (1 << CS10)) // TIMER1
#define STOPT1 TCCR1B &= ~((1 << CS11) | (1 << CS10))

volatile unsigned char startedISR = 0;
volatile unsigned char distanceChanged = 0;
volatile unsigned char error = 0;
volatile unsigned char ignoreEcho = 0;

/**
 * Check for button press.
 * Send the signal to begin the echo process.
 */
void checkAcquire()
{
    if ((PINB & ACQUIRE))
    {
        while (PINB & ACQUIRE)
        {
            // debouncing
        }
        TCNT1 = 0;
        PORTD |= TRIGGER;
        _delay_us(10);
        PORTD &= ~TRIGGER;
    }
}

/**
 * Returns the distance in millimeters.
 * Scaled frequency = 16,000,000 / 64
 * Period = 1 / frequency
 * Duration = number of pulses * period
 * Distance = duration * speed of sound in air / 2 (due to echo)
 * Distance = pulses * 1 / (16,000,000 / 64) * 330 / 2 * 1000 (m to mm)
 * Distance = pulses * 33 / 50 (simplified)
 */
int calcDistance(int pulses)
{
    if (pulses > 1000)
    {
        long intermediate = pulses * 10 / 50; // anything more than 10 potentially
        return intermediate * 33 / 10;        // causes an integer overflow
    }
    else
    {
        return pulses * 33 / 50; // if pulses * 33 < 65,536, no risk of overflow
    }
}

/**
 * ISR for Pin Change Interrupt on Port D.
 */
ISR(PCINT2_vect)
{
    if (!startedISR) // Detects start signal
    {
        STARTT1;
        startedISR = 1;
    }
    else // Detects stop signal
    {
        if (!ignoreEcho) // if a measurement is too far, abandon echo
        {
            STOPT1;
            startedISR = 0;
            distanceChanged = 1;
        }
        ignoreEcho = 0;
    }
}

/**
 * ISR for TIMER1.
 * Comparison with OCR1A = 6060;
 */
ISR(TIMER1_COMPA_vect)
{
    error = 1;
    ignoreEcho = 1;
}