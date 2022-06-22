/**
 * Martin Egli
 * 2022-06-06
 *
 * Timer1 as variable PWM output
 */

#ifndef _PWM_H_
#define _PWM_H_
/* - includes --------------------------------------------------------------- */

#include <stdint.h>

// - public MACROS -------------------------------------------------------------
void pwm_init(void);

#define PWM_MAX_FREQUENCY 10000                              // max. 10 kHz
#define PWM_MAX_PULSE_WIDTH 1000                             // in 0.1 % steps
void pwm_start(uint16_t set_freq, uint16_t set_pulse_witdh); // pwm_on()
void pwm_stop(void);                                         // pwm_off()

#endif /* _MACROS_H_ */
