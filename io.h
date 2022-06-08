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
#define PIN_OLED_SDA         (4)
#define PIN_OLED_SCL         (5)

// Buttons, rotary encoder PORTC
#define PIN_BUTTON_ON_OFF    (0)  // PCINT8,  on/off button
#define PIN_BUTTON_WHEEL     (1)  // PCINT9,  button of rotary encoder
#define PIN_WHEEL_A          (2)  // PCINT10, rotary encoder signal A
#define PIN_WHEEL_B          (3)  // PCINT11, rotary encoder signal B

// PWM, F_CPU out, PORTB
#define PIN_CLKO             (0)   // F_CPU/1 output
#define PIN_OC1A             (1)   // variable pwm signal, (on/off)
#define PIN_OC1B             (2)   // variable pwm signal, (on/off)

// fixed PWM, PORTD
#define PIN_OC0A             (6)   // fixed 1 MHz 50% pwm signal, also (on/off)

// UART PORTD
#define PIN_RXD              (0)
#define PIN_TXD              (1)

// XTAL, PORTB
#define PIN_XTAL1            (6)
#define PIN_XTAL2            (7)

#endif /* _IO_H_ */