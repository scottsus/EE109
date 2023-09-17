/********************************************
*
*  Name: Scott Susanto
*  Email: scottsus@usc.edu
*  Section: Wednesday 12:30pm
*  Assignment: Lab 4 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

int isPressed(char);

enum states
{
    UP,
    DOWN
};

int main(void)
{
    // lcd_init();

    // while (1)
    // {
    //     lcd_writedata(0);
    // }

    // Setup DDR and PORT bits for the 2 input buttons as necessary
    DDRB |= 0x03;
    DDRD |= 0xf0;

    // Initialize the LCD
    lcd_init();

    // Setup PORTC as inputs, enable Pull-up Resistors for A2 and A4
    DDRC &= 0x00;
    PORTC |= 0x18;

    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to track what state you
    //     are in by assigning the values UP or DOWN to that variable.
    char count = 0;
    char timer = 0;
    char my_counter_state = UP;

    while (1)
    { // Loop forever

        // Use an outer if statement to select what state you are in
        // Then inside each 'if' or 'else' block, perform the desired
        // output operations and then sample the inputs to update
        // the state for the next iteration

        if (my_counter_state == UP)
        {
            if (timer % 500 == 0)
            {
                if (count == 10)
                {
                    count = 0;
                }
                lcd_writecommand(2);
                lcd_numout(count);
                lcd_writecommand(2);
                count += 1;
            }

            if (isPressed(4) == 1)
            {
                my_counter_state = DOWN;
            }
        }
        else
        {
            if (timer % 500 == 0)
            {
                if (count == -1)
                {
                    count = 9;
                }
                lcd_writecommand(4);
                lcd_numout(count);
                lcd_writecommand(2);
                count -= 1;
            }

            if (isPressed(2) == 1)
            {
                my_counter_state = UP;
            }
        }

        // Delay before we go to the next iteration of the loop
        _delay_ms(10);
        timer += 10;
    }

    return 0; /* never reached */
}

// Checks if a PORTC button is pressed
int isPressed(char bit)
{
    if (!(PINC & (1 << bit)))
    {
        return 1;
    }
    return 0;
}
