/********************************************
 *
 *  Name: Scott Susanto
 *  Email: scottsus@usc.edu
 *  Section: Wednesday 12:30pm
 *  Assignment: Final Project - Rangefinder
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "project.h"
#include "lcd.h"
#include "rangefinder.h"
#include "encoder.h"
#include "led.h"
#include "buzzer.h"

#define GREEN (1 << PB3)
#define ACQUIRE (1 << PB4)
#define LENCODER (1 << PC1)
#define RENCODER (1 << PC2)
#define TOGGLE (1 << PC3)
#define BLUE (1 << PC4)
#define BUZZER (1 << PC5)
#define TRIGGER (1 << PD2)
#define ECHO (1 << PD3)

/**
 * Main function.
 */
int main(void)
{
	// Initialize LCD and other Ports
	lcd_init();
	port_init();
	encoder_init();
	threshold_init();

	// Enabling Pin Change Interrupts on PIN C
	PCICR |= (1 << PCINT1);					  // Enable interrupts in PIN C
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT13); // Enable interrupts in Pin C1 and C2

	// Enabling Pin Change Interrupts on PIN D
	PCICR |= (1 << PCINT2);	  // Enable interrupts in PIN D
	PCMSK2 |= (1 << PCINT19); // Enable interrupts in Pin D3

	// Configuring TIMER0
	TIMSK0 |= (1 << OCIE0A);	 // Enable local interrupt
	OCR0A = 0.5 * 1000000 / 330; // 0.5 * period with f = 330 Hz

	// Configuring TIMER1
	TIMSK1 |= (1 << OCIE1A); // Enable local interrupt
	OCR1A = 6060;			 // Setting upper threshold

	sei(); // Enable global interrupts

	// Write a spash screen to the LCD
	lcd_moveto(0, 1);
	lcd_stringout("Final Project");
	lcd_moveto(1, 1);
	lcd_stringout("Scott Susanto");
	_delay_ms(1000);
	lcd_writecommand(1);

	// Printing data on screen
	lcd_moveto(0, 0);
	lcd_stringout("Near");
	lcd_moveto(1, 0);
	lcd_stringout("Far");
	lcd_intout_coord(0, 5, nearThreshold);
	lcd_intout_coord(1, 5, farThreshold);
	lcd_moveto(isFar, 4);
	lcd_stringout(":");

	while (1)
	{
		// Checking for button presses
		checkAcquire();
		checkBuzzer();
		checkToggle();

		if (encoderChanged) // Updating values from encoder
		{
			lcd_moveto(0, 5);
			lcd_stringout("   ");
			lcd_intout_coord(0, 5, nearThreshold);
			lcd_moveto(1, 5);
			lcd_stringout("   ");
			lcd_intout_coord(1, 5, farThreshold);
			encoderChanged = 0;
		}

		if (distanceChanged) // Updating values from rangefinder
		{
			// Obtaining pieces of information
			int pulses = TCNT1;
			int distance = calcDistance(pulses);
			int integer = distance / 10;
			int decimal = distance % 10;

			// Printing data on screen
			char out[15];
			snprintf(out, 15, "%d.%d", integer, decimal);
			lcd_moveto(0, 10);
			lcd_stringout("    ");
			moveToPosition(integer);
			lcd_stringout(out);

			// Activate LED and Buzzer
			int distanceInCenti = distance / 10; // change from mm to cm
			activateLED(distanceInCenti);
			activateBuzzer(distanceInCenti);

			// Resetting flag
			distanceChanged = 0;
		}

		if (error) // Out of range from rangefinder
		{
			// Printing error message
			lcd_moveto(0, 10);
			lcd_stringout(">400.0");
			activateLED(-1);
			error = 0;
		}
	}
}

/**
 * Port initialization.
 */
void port_init()
{
	DDRB |= GREEN;
	DDRC |= BLUE | BUZZER;
	DDRD |= TRIGGER;
	PORTB |= GREEN | ACQUIRE;
	PORTC |= TOGGLE | LENCODER | RENCODER;
	PORTD |= ECHO;
}

/**
 * Check for button press.
 * Toggles the setting between near and far thresholds
 */
void checkToggle()
{
	if (PINC & TOGGLE)
	{
		while (PINC & TOGGLE)
		{
			// debouncing
		}

		lcd_moveto(isFar, 4);
		lcd_stringout(" ");

		isFar = 1 - isFar;

		lcd_moveto(isFar, 4);
		lcd_stringout(":");
	}
}

/**
 * Helper function to move to correct position.
 */
void moveToPosition(int integer)
{
	if (integer < 10)
	{
		lcd_moveto(0, 13);
	}
	else if (integer < 100)
	{
		lcd_moveto(0, 12);
	}
	else
	{
		lcd_moveto(0, 11);
	}
}
