#include <avr/io.h>
#include "led.h"
#include "encoder.h"

#define GREEN (1 << PB3)
#define BLUE (1 << PC4)

/**
 * Activate the LED based on the distance.
 */
void activateLED(int distance)
{
    if (distance == -1)
    {
        PORTB &= ~GREEN;
        PORTC |= BLUE;
    }
    else
    {
        PORTC &= ~BLUE;
        PORTB |= GREEN;
        if (distance < nearThreshold)
        {
            setDuty(0); // Green 0% Duty Cycle, Turn on Red
        }
        else if (distance > farThreshold)
        {
            setDuty(255); // Green 100% Duty Cycle, Turn off Red
        }
        else
        {
            setDuty(128); // 50% Duty Cycle
        }
    }
}

/**
 * Helper function to set duty value.
 */
void setDuty(unsigned char val)
{
    TCCR2A |= (1 << WGM21) | (1 << WGM20);             // Fast PWM mode for Pin B3
    TCCR2A |= (1 << COM2A1);                           // Set Pin on 0, Clear Pin on Threshold
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS00); // Set Prescalar to Clk / 256
    OCR2A = val;                                       // Set threshold to duty value
}