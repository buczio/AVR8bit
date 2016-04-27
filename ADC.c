
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

#define SAMPLE_DELAY  20


static inline void initFreerunningADC(void) {
  ADMUX |= (1 << REFS0);                
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    /* ADC clock prescaler /8 */

  ADMUX |= (1 << ADLAR);     /* left-adjust result, return only 8 bits */

  ADCSRA |= (1 << ADEN);                                 /* enable ADC */
  ADCSRA |= (1 << ADATE);                       /* auto-trigger enable */
  ADCSRA |= (1 << ADSC);                     /* start first conversion */
}

int main(void) {

  initUSART();
  initFreerunningADC();

  while (1) {
    transmitByte(ADCH);       
    _delay_ms(SAMPLE_DELAY);
  }                                                
  return (0);                          
}
