/********************************************
*
*  Name:  Scott Susanto
*  Email: scottsus@usc.edu
*  Lab section: Wednesday 12:30pm
*  Assignment:  Lab3
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH 250 /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);
void U();
void S();
void C();
void space();

int main(void)
{

  // Initialize appropriate DDR registers
  DDRD |= 0x02;

  // Initialize the LED output to 0
  PORTD &= ~(0x02);

  // Enable the pull-up resistors for the
  // 3 button inputs
  PORTB |= 0x1C;

  // Loop forever
  while (1)
  {

    //  Use if statements and the checkInput()
    //  function to determine if a button
    //  is being pressed and then output
    //  the correct dot/dash sequence by
    //  calling the dot(), dash(), and
    //  using appropriate delay functions

    if (!checkInput(4))
    {
      U();
    }
    else if (!checkInput(3))
    {
      S();
    }
    else if (!checkInput(2))
    {
      C();
    }
  }

  return 0; /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".
*/
void dot()
{
  makeOutput(1);
  _delay_ms(DOT_LENGTH);
  makeOutput(0);
}

/*
  dash() - Makes the output LED blink a "dash".
*/
void dash()
{
  makeOutput(1);
  _delay_ms(3 * DOT_LENGTH);
  makeOutput(0);
}

/*
  space() - Provides a buffer between dots/dashes
*/
void space()
{
  _delay_ms(DOT_LENGTH);
}

/*
  makeOutput() - Changes the output bit to either a zero
  or one, based on the input argument "value".

  If the argument is zero, turn the output OFF,
  otherwise turn the output ON.
*/
void makeOutput(char value)
{
  if (value == 1)
  {
    PORTD |= 0x02;
  }
  else
  {
    PORTD &= ~(0x02);
  }
}

/*
  checkInput(bit) - Checks the state of the input bit specified by the
  "bit" argument (0-7), and returns either 0 or 1 depending on its state.

  Pressed means Logic 0 because of the ground connection.
  Unpressed means Logic 1 as Vdd is able to supply the LED with power.
 */
char checkInput(char bit)
{
  // switch (bit)
  // {
  // case 0:
  //   return PINB & 0x01;
  // case 1:
  //   return PINB & 0x02;
  // case 2:
  //   return PINB & 0x04;
  // case 3:
  //   return PINB & 0x08;
  // case 4:
  //   return PINB & 0x10;
  // case 5:
  //   return PINB & 0x20;
  // case 6:
  //   return PINB & 0x40;
  // case 7:
  //   return PINB & 0x80;
  // }

  return PINB & (1 << bit);
}
// (1 < PD3) PINB & 0x03
/*
  Morse Code U: dot dot dash
*/
void U()
{
  dot();
  space();
  dot();
  space();
  dash();
}

/*
  Morse Code S: dot dot dot
*/
void S()
{
  dot();
  space();
  dot();
  space();
  dot();
}

/*
  Morse Code C: dash dot dash dot
*/
void C()
{
  dash();
  space();
  dot();
  space();
  dash();
  space();
  dot();
}
