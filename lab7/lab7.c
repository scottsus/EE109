/********************************************
 *
 *  Name: Scott Susanto
 *  Email: scottsus@usc.edu
 *  Section: Wednesday 12:30pm
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include <avr/interrupt.h>

void variable_delay_us(int);
void play_note(unsigned short);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
unsigned short frequency[8] =
	{262, 294, 330, 349, 392, 440, 494, 523};

volatile unsigned char new_state, old_state;
volatile unsigned char changed = 1; // Flag for state change
volatile int count = 0;				// Count to display
volatile unsigned char a, b, regC;

int main(void)
{
	// Initialize DDR and PORT registers and LCD
	lcd_init();
	DDRB |= (1 << PB4);
	DDRC &= 0x00;
	PORTC |= (1 << PC1) | (1 << PC5);
	PORTB |= (1 << PB4);

	// Enabling Interrupts
	PCICR |= (1 << PCINT1);
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);
	sei();

	// Write a spash screen to the LCD
	lcd_moveto(0, 4);
	lcd_stringout("Lab 07");
	lcd_moveto(1, 0);
	lcd_stringout("Scott Susanto");
	_delay_ms(1000);
	lcd_writecommand(1);

	// Read the A and B inputs to determine the intial state.
	// In the state number, B is the MSB and A is the LSB.
	regC = PINC;
	a = regC & (1 << PC1);
	b = regC & (1 << PC5);

	if (!b && !a)
		old_state = 0;
	else if (!b && a)
		old_state = 1;
	else if (b && !a)
		old_state = 2;
	else
		old_state = 3;

	new_state = old_state;

	while (1)
	{ // Loop forever
		if (changed)
		{
			changed = 0; // Reset changed flag

			// Output count to LCD
			char output[5];
			snprintf(output, 5, "%2d", count);
			lcd_writecommand(1);
			lcd_moveto(0, 0);
			lcd_stringout(output);

			// Do we play a note?
			if ((count % 8) == 0)
			{
				play_note(frequency[(abs(count) % 64) / 8]);
			}
		}
	}
}

ISR(PCINT1_vect)
{
	// Read inputs
	regC = PINC;
	a = regC & (1 << PC1);
	b = regC & (1 << PC5);

	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	if (old_state == 0)
	{
		if (!(a == 0))
		{
			new_state = 1;
			count++;
		}
		if (!(b == 0))
		{
			new_state = 2;
			count--;
		}
		// Handle A and B inputs for state 0
	}
	else if (old_state == 1)
	{
		if (a == 0)
		{
			new_state = 0;
			count--;
		}
		if (!(b == 0))
		{
			new_state = 3;
			count++;
		}
		// Handle A and B inputs for state 1
	}
	else if (old_state == 2)
	{
		if (!(a == 0))
		{
			new_state = 3;
			count--;
		}
		if (b == 0)
		{
			new_state = 0;
			count++;
		}
		// Handle A and B inputs for state 2
	}
	else
	{ // old_state = 3
		if (a == 0)
		{
			new_state = 2;
			count++;
		}
		if (b == 0)
		{
			new_state = 1;
			count--;
		}
		// Handle A and B inputs for state 3
	}

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state)
	{
		changed = 1;
		old_state = new_state;
	}
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(unsigned short freq)
{
	unsigned long period;
	period = 1000000 / freq; // Period of note in microseconds
	while (freq--)
	{
		PORTB |= (1 << PB4);
		variable_delay_us(0.5 * period); // Delay for half the period
		PORTB &= ~(1 << PB4);
		variable_delay_us(0.5 * period); // Delay for half the period again
	}
}

/*
	variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
	int i = (delay + 5) / 10;

	while (i--)
		_delay_us(10);
}
