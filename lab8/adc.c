#include <avr/io.h>

#include "adc.h"

/**
 * Initialize the ADC
 */
void adc_init(void)
{

    ADMUX |= (1 << REFS0);                                  // bit 6; set voltage to 5v
    ADMUX |= (1 << ADLAR);                                  // bit 5; use 8-bit readings
    ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); // bits 0, 1, 2; set prescalar to 128
    ADCSRA |= (1 << ADEN);                                  // Turn on the ADC
}

unsigned char adc_sample(unsigned char channel)
{
    // Set ADC input mux bits to 'channel' value
    ADMUX &= ~(0x0f);
    ADMUX |= channel;

    // Convert an analog input and return the 8-bit result
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA & (1 << ADSC)) != 0)
    {
        // busy looping
    }

    return ADCH;
}
