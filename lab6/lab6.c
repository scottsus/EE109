/********************************************
 *
 *  Name: Scott Susanto
 *  Email: scottsus@usc.edu
 *  Section: Wednesday 12:30pm
 *  Assignment: Lab 6 - Analog-to-digitaXl conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{

    // Initialize the LCD
    lcd_init();
    PORTD |= (1 << PD0);

    // Initialize the ADC
    adc_init();

    // Initialize the random number function
    rand_init();

    // Write splash screen
    lcd_moveto(0, 4);
    lcd_stringout("Lab 06");
    lcd_moveto(1, 0);
    lcd_stringout("Scott Susanto");
    _delay_ms(1000);
    lcd_writecommand(1);

    // Find a random initial position of the 'X'
    int posX = rand();
    posX %= 15;
    lcd_moveto(0, posX);

    // Display the 'X' on the screen
    lcd_stringout("X");

    int posC = 0;
    int time = 0;
    int timeEqual = 0;
    while (1)
    { // Loop forever

        char pressed = 0;

        // Check for button pressed
        unsigned char dataX = adc_sample(0);          // 0 = Right
        if (150 <= dataX && dataX <= 160 && posX > 0) // 51 = Up
        {                                             // 102 = Down
            posX -= 1;                                // 156 = Left
            pressed = 1;                              // 206 = Select
        }                                             // 255 = Nothing
        if (0 <= dataX && dataX <= 10 && posX < 15)
        {
            posX += 1;
            pressed = 1;
        }

        // If pressed, move 'X' to new position
        if (pressed == 1)
        {
            lcd_writecommand(1);
            lcd_moveto(0, posX);
            lcd_stringout("X");
        }

        // Do a conversion of potentiometer input
        unsigned char dataC = adc_sample(3);

        // Clear old location
        lcd_moveto(1, posC);
        lcd_stringout(" ");

        // Move '^' to new position
        posC = dataC / 17;
        lcd_moveto(1, posC);
        lcd_stringout("^");

        // Delay
        time += 100;
        _delay_ms(100);

        // Check if '^' is aligned with 'X'
        if (posX == posC)
        {
            timeEqual += 100;
        }

        if (timeEqual >= 2000)
        {
            lcd_writecommand(1);
            char result[15];
            snprintf(result, 15, "Time: %dms", time);
            lcd_moveto(0, 0);
            lcd_stringout("VICTORY");
            lcd_moveto(1, 0);
            lcd_stringout(result);
            break;
        }

        if (posX != posC)
        {
            timeEqual = 0;
        }
    }

    return 0; /* never reached */
}

void rand_init()
{
    int seed = 0;
    unsigned char i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++)
    {
        for (j = 1; j < 6; j++)
        {
            x = adc_sample(j);
            x &= 1;                 // Get the LSB of the result
            seed = (seed << 1) + x; // Build up the 15-bit result
        }
    }
    srand(seed); // Seed the rand function
}
