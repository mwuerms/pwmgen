/**
 * Martin Egli
 * 2022-06-06
 *
 * define all ios for this project
 */

#ifndef _IO_H_
#define _IO_H_
/* - includes --------------------------------------------------------------- */

// PORTB
#define BV_BUTTON_ON_OFF    _BV(1)
#define BV_BUTTON_WHEEL     _BV(2)  // button of rotary encoder
#define BV_WHEEL_A          _BV(3)  // rotary encoder signal A
#define BV_WHEEL_B          _BV(4)  // rotary encoder signal B

// PORTC
#define BV_FCPU_OUT        _BV(0)   // F_CPU/1 output
#define BV_FIXED_PWM_OUT   _BV(1)   // fixed 1 MHz 50% pwm signal, also (on/off)
#define BV_VAR_PWM_OUT     _BV(2)   // variable pwm signal, (on/off)

#define BV_OLED_SPI   _BV(1)

// Timings, Timer1, 16-Bit

#endif /* _IO_H_ */