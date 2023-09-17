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
#define CYCLE 2000

enum states
{
    mainG,
    leftG,
    smallG,
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

    /* Initialize state machine variables */
    char thisState = mainG;
    char nextState = mainG;
    char internalThisState = mainG;
    char internalNextState = mainG;
    char allG = 0;
    char L_1 = ' ';
    char L_2 = ' ';
    char S_1 = ' ';
    char S_2 = ' ';
    char mainst = 'G';
    char left = 'R';
    char small = 'R';
    char lineOne[16];
    char lineTwo[16];
    int timer = 0;

    while (1)
    {
        /* Capturing sensor values */
        if (L1 == 0)
        {
            L_1 = 'L';
        }
        if (L2 == 0)
        {
            L_2 = 'L';
        }
        if (S1 == 0)
        {
            S_1 = 'S';
        }
        if (S2 == 0)
        {
            S_2 = 'S';
        }

        if (timer % 500 == 0)
        {
            char codes[5];
            lcd_moveto(1, 8);
            snprintf(codes, 5, "%c%c%c%c", L_1, L_2, S_1, S_2);
            lcd_stringout(codes);
            lcd_moveto(1, 8);
        }

        /* Capture input variables and assigning states */
        if (LEFT && SMALL)
        {
            thisState = leftG;
            nextState = smallG;
            allG = 1;
        }
        else if (LEFT)
        {
            if (thisState == smallG)
            {
                nextState = leftG;
            }
            else
            {
                thisState = leftG;
            }
        }
        else if (SMALL)
        {
            if (thisState == leftG)
            {
                nextState = smallG;
            }
            else
            {
                thisState = smallG;
            }
        }
        else
        {
            if (allG <= 0)
            {
                if (timer % (2 * CYCLE) == 0)
                {
                    nextState = mainG;
                }
            }
            else
            {
                if (timer % CYCLE == 0)
                {
                    allG -= 1;
                }
            }
        }

        /* Show outputs based on states and printing values */
        if (timer % CYCLE == 0)
        {
            internalThisState = internalNextState;
            switch (internalThisState)
            {
            case mainG:
                mainst = 'G';
                left = 'R';
                small = 'R';
                break;
            case leftG:
                mainst = 'R';
                left = 'G';
                small = 'R';
                break;
            case smallG:
                mainst = 'R';
                left = 'R';
                small = 'G';
                break;
            }
            snprintf(lineOne, 16, "Main=%c  Small=%c", mainst, small);
            snprintf(lineTwo, 16, "Left=%c", left);
            lcd_moveto(0, 0);
            lcd_stringout(lineOne);
            lcd_moveto(1, 0);
            lcd_stringout(lineTwo);
            lcd_moveto(1, 8);

            /* Resetting variables for next iteration */
            internalNextState = thisState;
            thisState = nextState;
            L_1 = ' ';
            L_2 = ' ';
            S_1 = ' ';
            S_2 = ' ';
        }

        /* Delay function and keeping track of time */
        _delay_ms(10);
        if (timer == 10000)
        {
            timer = 0;
        }
        timer += 10;
    }

    return 0;
}