/**
 * Martin Egli
 * 2022-06-06
 *
 * generate variable pwm signal:
 * 1 ... 10 kHz, 0.1 ... 99.9 %
 * 
 * Timer1 as variable PWM output
 * periode (TOP, frequency):
 *  ICR1
 * outputs(same frequency, different pulse width):
 *  OC1A 
 *  OC1B
 * 
 * f_{pwm} = f_{clkI/O}/(N*(1+TOP))
 * f_{clkI/O} = F_CPU = 8000000
 * N: prescalser = 1 (1, 8, 64, 256, or 1024)
 * 
 */
#warning _TODO frequency sweep
#warning _TODO pulse width sweep

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "pwm.h"

// - public MACROS -------------------------------------------------------------
void pwm_init(void) {
    DDRB |= (1<<BV_OC1A);
}

void pwm_start(uint16_t set_freq, uint16_t set_pulse_witdh) {
    uint16_t prescaler = 1;
    uint8_t cs_value;
    // TOP = f_{clkI/O}(N*f_{pwm}-1
    // check set_pulse_witdh
    if((set_pulse_witdh == 0) || (set_pulse_witdh >= PWM_MAX_PULSE_WIDTH)){
        // error, out of range, do not start pwm
        pwm_stop();
    }
    // get prescaler N from range
    if(set_freq > PWM_MAX_FREQUENCY) {
        // error, out of range, do not start pwm
        pwm_stop();
    }
    else if(set_freq >= 130) {
        prescaler = 1;
        cs_value = _BV(CS10);
    }
    else if(set_freq >= 16) {
        prescaler = 8;
        cs_value = _BV(CS11);
    }
    else if(set_freq >= 2) {
        prescaler = 64;
        cs_value = _BV(CS11)|_BV(CS10);
    }
    else {
        prescaler = 256;
        cs_value = _BV(CS12);
    }
    ICR1 = F_CPU / (prescaler * set_freq) -1;

    // use mode 14: fast pwm, TOP: ICR1
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | cs_value;
}

void pwm_stop(void) {
    TCCR1B = 0; // stop clock/counter
    TCCR1C = 0; // force outputs low
    TCCR1A = 0;
}
