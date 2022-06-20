/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "project.h"
#include "io.h"

/*#ifdef __AVR_ATmega328P__
#error ??__AVR_ATmega328P__
#endif*/

volatile uint8_t button_on_cnt;
volatile uint8_t button_wheel_cnt;
volatile int8_t wheel_cnt;

// PCINT8, 9, 10, 11
ISR(PCINT1_vect)
{
  uint8_t change = 0;
  cli();
  if ((PINC & _BV(PIN_BUTTON_ON_OFF)) == 0)
  {
    // pressed
    button_on_cnt++;
    change = 1;
  }
  if ((PINC & _BV(PIN_BUTTON_WHEEL)) == 0)
  {
    // pressed
    button_wheel_cnt++;
    change = 1;
  }
  if ((PINC & _BV(PIN_WHEEL_A)) == 0)
  {
    if ((PINC & _BV(PIN_WHEEL_B)) == 0)
    {
      wheel_cnt++;
      change = 1;
    }
    else
    {
      wheel_cnt--;
      change = 1;
    }
  }
  else
  {
    if ((PINC & _BV(PIN_WHEEL_B)) == 0)
    {
      wheel_cnt--;
      change = 1;
    }
    else
    {
      wheel_cnt++;
      change = 1;
    }
  }
  if (change)
  {
    SEND_EVENT(EV_BUTTON);
    SEND_BUTTON_EVENT(EV_BUTTON_UPDATE);
  }
  sei();
}

void buttons_init(void)
{
  // gpios
  DDRC &= ~(_BV(PIN_BUTTON_ON_OFF) | _BV(PIN_BUTTON_WHEEL) | _BV(PIN_WHEEL_A) | _BV(PIN_WHEEL_B));
  PCIFR |= _BV(PCIF1); // clear
  PCICR |= _BV(PCIE1);
  PCMSK1 |= (_BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10));
  // vars
  button_on_cnt = 0;
  button_wheel_cnt = 0;
  wheel_cnt = 0;
}