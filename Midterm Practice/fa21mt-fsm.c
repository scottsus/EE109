#include <avr/io.h>
#include <util/delay.h> // allows for _delay_ms() function
#define LED (1 << 1)
#define BTNB (1 << 2)
#define BTNA (1 << 5)

enum States
{
    S0,
    S1,
    S2
};

int main()
{
    char st = S0;
    /* ---- ADD ANY NECESSARY VARIABLE DECLARATIONS OR INIT CODE HERE ---- */
    int count = 0;
    int blink = 0;
    DDRD |= LED;
    PORTD |= BTNA | BTNB;

    while (1)
    {
        char btns = PIND; // SAMPLE BUTTONS
        char a = btns & BTNA;
        char b = btns & BTNB;

        if (st == S0)
        {
            // Output logic for state S0
            PORTD &= ~LED1;

            // Transition logic for state S0
            if (a && !b)
            {
                if (count == 20) // if A and not B has been pressed for 2s
                {
                    st = S1; // move to state S1
                    count = 0;
                }
                else
                {
                    count++; // otherwise continue the count while A is pressed
                }
            }
            else // if at any point A is not pressed
            {
                count = 0; // reset the count
                if (b)     // additionally, if b is pressed,
                {
                    st = S1; // move to state S1
                }
            }
        }
        else if (st == S1)
        {
            // Output logic for state S1
            if (blink % 5 == 0)
            {
                PORTD |= LED1;
            }
            else
            {
                PORTD &= ~LED1;
            }

            // Transition logic state S1
            if (b)
            {
                st = S2;
            }
        }
        else
        { /* S2 */
            // Output logic for state S2
            PORTD |= LED1;

            // Transition logic for state S2
            if (!a && !b)
            {
                st = S0;
            }
        }
        // Delay before next iteration
        count++;
        blink++;
        _delay_ms(100);
    } /* end while */
    return 0;
} /* end main */