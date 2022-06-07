/**
 * Martin Egli
 * 2022-06-06
 *
 * define all ios for this project
 */

#ifndef _IO_H_
#define _IO_H_
/* - includes --------------------------------------------------------------- */

// OLED I2C, PORTC
#define BV_OLED_SDA         _BV(4)
#define BV_OLED_SCL         _BV(5)

// Buttons, rotary encoder PORTC
#define BV_BUTTON_ON_OFF    _BV(0)  // PCINT8,  on/off button
#define BV_BUTTON_WHEEL     _BV(1)  // PCINT9,  button of rotary encoder
#define BV_WHEEL_A          _BV(2)  // PCINT10, rotary encoder signal A
#define BV_WHEEL_B          _BV(3)  // PCINT11, rotary encoder signal B

// PWM, F_CPU out, PORTB
#define BV_CLKO             _BV(0)   // F_CPU/1 output
#define BV_OC1A             _BV(1)   // variable pwm signal, (on/off)
#define BV_OC1B             _BV(2)   // variable pwm signal, (on/off)

// fixed PWM, PORTD
#define BV_OC0A             _BV(6)   // fixed 1 MHz 50% pwm signal, also (on/off)

// UART PORTD
#define BV_RXD              _BV(0)
#define BV_TXD              _BV(1)

// XTAL, PORTB
#define BV_XTAL1            _BV(6)
#define BV_XTAL2            _BV(7)

#endif /* _IO_H_ */