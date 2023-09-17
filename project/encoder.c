#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "encoder.h"

#define LENCODER (1 << PC1)
#define RENCODER (1 << PC2)

volatile unsigned char new_state;
volatile unsigned char old_state;
volatile unsigned char encoderChanged = 1; // Flag for state change
volatile unsigned char a, b, regC;

volatile unsigned int nearThreshold;
volatile unsigned int farThreshold;
volatile unsigned char isFar;

unsigned int nearAddress = 808;
unsigned int farAddress = 909;

/**
 * Encoder initialization.
 */
void encoder_init()
{
    regC = PINC;
    a = regC & LENCODER;
    b = regC & RENCODER;

    if (!a && !b)
        old_state = 0;
    else if (!a && b)
        old_state = 1;
    else if (!b && a)
        old_state = 2;
    else
        old_state = 3;

    new_state = old_state;
}

/**
 * Threshold initialization.
 */
void threshold_init()
{
    short near = eeprom_read_word(nearAddress);
    short far = eeprom_read_word(farAddress);

    if (near == -1)
    {
        nearThreshold = 100;
    }
    else
    {
        nearThreshold = near;
    }

    if (far == -1)
    {
        farThreshold = 200;
    }
    else
    {
        farThreshold = far;
    }
}

/**
 * Helper function to increment thresholds
 */
void increment(unsigned char threshold)
{
    if (threshold == 0) // near threshold
    {
        if ((farThreshold - nearThreshold) > 5)
        {
            nearThreshold++;
            eeprom_update_word(nearAddress, nearThreshold);
        }
    }
    else if (threshold == 1)
    {
        if (farThreshold < 400)
        {
            farThreshold++;
            eeprom_update_word(farAddress, farThreshold);
        }
    }
}

/**
 * Helper function to decrement thresholds
 */
void decrement(unsigned char threshold)
{
    if (threshold == 0) // near threshold
    {
        if (nearThreshold > 2)
        {
            nearThreshold--;
            eeprom_update_word(nearThreshold, nearAddress);
        }
    }
    else if (threshold == 1) // far threshold
    {
        if ((farThreshold - nearThreshold) > 5)
        {
            farThreshold--;
            eeprom_update_word(farThreshold, farAddress);
        }
    }
}

/**
 * ISR for Pin Change Interrupt on Port C.
 */
ISR(PCINT1_vect)
{
    // Read inputs
    regC = PINC;
    a = regC & LENCODER;
    b = regC & RENCODER;

    // For each state, examine the two input bits to see if state
    // has changed, and if so set "new_state" to the new state,
    // and adjust the count value.
    if (old_state == 0)
    {
        if (!(a == 0))
        {
            new_state = 1;
            if (isFar)
            {
                increment(1);
            }
            else
            {
                increment(0);
            }
        }
        if (!(b == 0))
        {
            new_state = 2;
            if (isFar)
            {
                decrement(1);
            }
            else
            {
                decrement(0);
            }
        }
        // Handle A and B inputs for state 0
    }
    else if (old_state == 1)
    {
        if (a == 0)
        {
            new_state = 0;
            if (isFar)
            {
                decrement(1);
            }
            else
            {
                decrement(0);
            }
        }
        if (!(b == 0))
        {
            new_state = 3;
            if (isFar)
            {
                increment(1);
            }
            else
            {
                increment(0);
            }
        }
        // Handle A and B inputs for state 1
    }
    else if (old_state == 2)
    {
        if (!(a == 0))
        {
            new_state = 3;
            if (isFar)
            {
                decrement(1);
            }
            else
            {
                decrement(0);
            }
        }
        if (b == 0)
        {
            new_state = 0;
            if (isFar)
            {
                increment(1);
            }
            else
            {
                increment(0);
            }
        }
        // Handle A and B inputs for state 2
    }
    else
    { // old_state = 3
        if (a == 0)
        {
            new_state = 2;
            if (isFar)
            {
                increment(1);
            }
            else
            {
                increment(0);
            }
        }
        if (b == 0)
        {
            new_state = 1;
            if (isFar)
            {
                decrement(1);
            }
            else
            {
                decrement(0);
            }
        }
        // Handle A and B inputs for state 3
    }

    // If state changed, update the value of old_state,
    // and set a flag that the state has changed.
    if (new_state != old_state)
    {
        encoderChanged = 1;
        old_state = new_state;
    }
}