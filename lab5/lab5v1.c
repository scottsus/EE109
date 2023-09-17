// /********************************************
// *
// *  Name: Scott Susanto
// *  Email: scottsus@usc.edu
// *  Section: Wednesday 12:30pm
// *  Assignment: Lab 5 - Traffic light
// *
// ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"

#define L1 (PINC & (1 << 1))
#define L2 (PINC & (1 << 2))
#define S1 (PINC & (1 << 3))
#define S2 (PINC & (1 << 4))
#define LEFT ((L1 == 0) || (L2 == 0))
#define SMALL ((S1 == 0) || (S2 == 0))

enum states
{
    mainG,
    smallG,
    leftG,
    smallLeftG,
    leftSmallG,
    allG
};

int main(void)
{

    /* Call lcd_init to set up the LCD */
    lcd_init();
    lcd_writecommand(1);

    /* Set up I/O ports for buttons */
    PORTC |= (1 << 1);
    PORTC |= (1 << 2);
    PORTC |= (1 << 3);
    PORTC |= (1 << 4);

    /* Call lcd_stringout to print out your name */
    lcd_stringout("SCOTT SUSANTO");

    /* Use snprintf to create a string with your birthdate */
    char birthdate[16];
    int day = 05;
    int year = 1998;
    char *month = "OCT";
    snprintf(birthdate, 16, "%s %02d, %d", month, day, year);

    /* Use lcd_moveto to start at an appropriate column
       in the bottom row to appear centered */
    lcd_moveto(1, 2);

    /* Use lcd_stringout to print the birthdate string */
    lcd_stringout(birthdate);

    /* Delay 1 second */
    _delay_ms(1000);
    lcd_writecommand(1);

    /* Initialize State machine variables */
    char timer = 0;
    // char changed = 1;

    char lineOne[16];
    char lineTwo[16];
    char mainst = 'G';
    char small = 'R';
    char left = 'R';

    char state = mainG;
    char prevState = mainG;

    while (1)
    {
        /* Read the sensors */
        if (LEFT && SMALL)
        {
            prevState = state;
            state = allG;
        }
        else if (LEFT)
        {
            if (state == smallG)
            {
                state = smallLeftG;
            }
            else
            {
                state = leftG;
            }
        }
        else if (SMALL)
        {
            if (state = leftG)
            {
                state = leftSmallG;
            }
            else
            {
                state = smallG;
            }
        }
        else
        {
            state = mainG;
        }

        /* Determine new state */

        /* Show outputs */
        if (timer % 1000 == 0)
        {
            switch (state)
            {
            case mainG:
                mainst = 'G';
                small = 'R';
                left = 'R';
                state = mainG;
                // changed = 1;
                break;
            case leftG:
                mainst = 'R';
                small = 'R';
                left = 'G';
                state = mainG;
                // changed = 1;
                break;
            case smallG:
                mainst = 'R';
                small = 'G';
                left = 'R';
                state = mainG;
                // changed = 1;
                break;
            case leftSmallG:
                mainst = 'R';
                small = 'R';
                left = 'G';
                state = smallG;
                // changed = 1;
                break;
            case smallLeftG:
                mainst = 'R';
                small = 'G';
                left = 'R';
                state = leftG;
                // changed = 1;
                break;
            case allG:
                mainst = 'R';
                small = 'R';
                left = 'G';
                state = smallG;
                // changed = 1;
                break;
            }
        }

        snprintf(lineOne, 16, "Main=%c  Small=%c", mainst, small);
        snprintf(lineTwo, 16, "Left=%c  LLSS", left);

        // if (changed)
        // {
        lcd_moveto(0, 0);
        lcd_stringout(lineOne);
        lcd_moveto(1, 0);
        lcd_stringout(lineTwo);
        // }

        _delay_ms(10);
        if (timer == 10000)
        {
            timer = 0;
        }
        timer += 10;
    }

    return 0; /* never reached */
}
