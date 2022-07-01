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

ISR(INT0_vect)
{
  button_on_cnt++;
  SEND_EVENT(EV_BUTTON);
  SEND_BUTTON_EVENT(EV_BUTTON_BTN_ON);
}

ISR(INT1_vect)
{
  button_wheel_cnt++;
  SEND_EVENT(EV_BUTTON);
  SEND_BUTTON_EVENT(EV_BUTTON_BTN_WHEEL);
}

// PCINT8, 9
ISR(PCINT1_vect)
{
  uint8_t wheel_in = PINC & (_BV(PIN_WHEEL_A) | _BV(PIN_WHEEL_B));
  cli();

  if ((wheel_in & _BV(PIN_WHEEL_A)) == 0)
  {
    if ((wheel_in & _BV(PIN_WHEEL_B)) == 0)
    {
      wheel_cnt++;
    }
    else
    {
      wheel_cnt--;
    }
  }
  else
  {
    if ((wheel_in & _BV(PIN_WHEEL_B)) == 0)
    {
      wheel_cnt--;
    }
    else
    {
      wheel_cnt++;
    }
  }
  SEND_EVENT(EV_BUTTON);
  SEND_BUTTON_EVENT(EV_BUTTON_WHEEL);

  sei();
}

void buttons_init(void)
{
  // vars
  button_on_cnt = 0;
  button_wheel_cnt = 0;
  wheel_cnt = 0;

  // gpios
  DDRD &= ~(_BV(PIN_BUTTON_ON_OFF) | _BV(PIN_BUTTON_WHEEL));
  EICRA = (_BV(ISC11) | _BV(ISC01)); // INT0, INT1 on falling edge = button pressed
  EIFR = (_BV(INTF0) | _BV(INTF1));
  EIMSK = (_BV(INT0) | _BV(INT1));

  // wheel (rotary encoder)
  DDRC &= ~(_BV(PIN_WHEEL_A) | _BV(PIN_WHEEL_B));
  PCIFR |= _BV(PCIF1); // clear
  PCICR |= _BV(PCIE1);
  PCMSK1 |= (_BV(PCINT8));
}