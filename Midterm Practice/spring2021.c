#include <avr/io.h>
#include <util/delay.h> // allows for _delay_ms() function
#define ALL_LEDS ((1 << 4) | (1 << 3) | (1 << 2))
#define BTNB (1 << 1)
#define BTNA (1 << 5)

enum States
{
    S0,
    SBP,
    SWA,
    SAP
};

int main()
{
    char st = S0;
    char count = 0;
    char countB = 0;
    /* ---- ADD ANY NECESSARY VARIABLE DECLARATIONS OR INIT CODE HERE ---- */

    while (1)
    {
        char btns = PINB; // SAMPLE BUTTONS
        char a = btns & BTNA;
        char b = btns & BTNB;

        // Add your logic for the state machine and outputs here
        // You may not alter this 'if' statement structure
        if (st == S0)
        {
            if (!a && b)
            {
                st = SBP;
            }
        }
        else if (st == SBP)
        {
            if (b)
            {
                countB++;
            }
            if (!a && countB >= 4)
            {
                st = SWA;
            }
        }
        else if (st == SWA)
        {
            if (a)
            {
                st = SAP;
            }
        }
        else
        {
            if (!a)
            {
                st = S0;
            }
        }
        // Delay before next iteration
        count++;
        countB++;
        _delay_ms(50); // choose your delay value
    }                  /* end while */
    return 0;
} /* end main */