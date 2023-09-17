#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.h"
#include "encoder.h"

#define BUZZER (1 << PC5)
#define STARTT0 TCCR0B |= ((1 << CS01) | (1 << CS00)) // TIMER0
#define STOPT0 TCCR0B &= ~((1 << CS01) | (1 << CS00))

volatile unsigned char isOn = 0;
volatile unsigned char isDone = 0;
volatile unsigned char isPlaying = 0;
volatile unsigned int buzzerTime = 0;

/**
 * Play a tone at the frequency specified for one second.
 */
void checkBuzzer()
{
    if (isPlaying && (buzzerTime < 450))
    {
        if (!isOn)
        {
            isOn = 1;
            PORTC |= BUZZER;
            TCNT0 = 0;
            STARTT0;
        }
        else
        {
            if (isDone)
            {
                PORTC &= ~BUZZER;
                STOPT0;
                isDone = 0;
                isOn = 0;
                buzzerTime++;
            }
        }

        if (buzzerTime >= 450)
        {
            isPlaying = 0;
        }
    }
}

/**
 * Activates the buzzer if distance is inside the near threshold.
 */
void activateBuzzer(int distance)
{
    if (distance <= nearThreshold)
    {
        isPlaying = 1;
        buzzerTime = 0;
    }
}

/**
 * ISR using TIMER0.
 * Comparison with OCR0A = 0.5 * 1000000 / 330;
 */
ISR(TIMER0_COMPA_vect)
{
    isDone = 1;
}