// Group D5 and D0 = A and B button inputs, respectively
// Group D3 = LED1 and D2 = LED2
//
// Desired Operation: See PDF of test.
//  Remember you must sample the inputs every 50 ms

// Start by completing the function below to flip the output voltage
//  on the specified (in the argument) of PORTD (i.e. calling flipLED(1)
//  should flip PortD bit 1 to its opposite value)
#include <avr/io.h>
#include <util/delay.h>

// These are just convenience and not necessary
#define LED1 1
#define LED2 2
#define A 5
#define B 0

enum states
{
    s0,
    s1,
    s2
};

void flipLED(unsigned char bitpos)
{
    // flip bit 'bitpos' of PORTD to its opposite value
    PORTD ^= (1 << bitpos);
}

int main()
{
    // Declare necessary variables here
    char count = 0;
    char state = S0;

    // Provide register initialization code here
    DDRD |= (1 << LED1) | (1 << LED2);
    PORTD |= (1 << A) | (1 << B);

    // Loop forever
    while (1)
    {
        char btns = PIND;
        // Update the code below by filling int he blanks
        char a = btns & (1 << A); // Isolate the button A's value
        char b = btns & (1 << B); // Isolate the button B's value

        // Implement state machine and LED blinking code below.
        // Do not re-read the inputs a and b from PIND but use
        //    the a and b variables you set above.
        if (state == s0)
        {
            if (!(a == 0) && !(b == 0))
            {
                state = s1;
            }
        }
        else if (state == s1)
        {
            if (count % 3 == 0)
            {
                flipLED(LED1);
            }
            if (!(a == 0) && (b == 0))
            {
                state = s2;
                PORTD &= ~(1 << LED1);
            }
            else if ((a == 0) && !(b == 0))
            {
                state = s0;
                PORTD &= ~(1 << LED1);
            }
        }
        else if (state == s2)
        {
            if (count % 6 == 0)
            {
                flipLED(LED2);
            }
            if ((a == 0) && (b == 0))
            {
                state = s1;
                PORTD &= ~(1 << LED2);
            }
        }

        // This is the only allowed DELAY in the program
        // You may NOT alter it
        count++;
        _delay_ms(50);
    } // end while
    return 0;
} // end main