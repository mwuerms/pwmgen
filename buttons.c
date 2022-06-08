/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

/*#ifdef __AVR_ATmega328P__
#error ??__AVR_ATmega328P__
#endif*/

//PCINT8, 9, 10, 11
ISR(PCINT1_vect) {
  cli();
  if( (PINC & (1 << PIN_BUTTON_ON_OFF)) == 1 ) {

  }
  else
  {
  }
  sei();
}

void buttons_init(void) {
    // gpios
    // vars
}