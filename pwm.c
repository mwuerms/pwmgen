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

// - private functions ---------------------------------------------------------
ISR(TIMER1_COMPA_vect) {
    uint16_t calc_reg = OCR1A + 100;

    if(calc_reg > ICR1) {
        OCR1A = 0;
    }
    else {
        OCR1A = calc_reg;
    }
}
/*    // pulse width sweep
    OCR1A += 10;
    if(OCR1A == ICR1) {
        OCR1A = 0;
    }
}
*/

// - public functions ----------------------------------------------------------
void pwm_init(void) {
    DDRB |= (1<<PIN_OC1A);
}

void pwm_start(uint16_t set_freq, uint16_t set_pulse_witdh) {
    uint32_t calc_timer_regs;
    uint16_t prescaler = 1;
    uint8_t cs_value  = _BV(CS12);
    // TOP = f_{clkI/O}(N*f_{pwm}-1
    // check set_pulse_witdh
    if((set_pulse_witdh == 0) || (set_pulse_witdh >= PWM_MAX_PULSE_WIDTH)){
        // error, out of range, do not start pwm
        pwm_stop();
        return;
    }
    // get prescaler N from range
    if(set_freq > PWM_MAX_FREQUENCY) {
        // error, out of range, do not start pwm
        pwm_stop();
        return;
    }
    else if(set_freq >= 250) {  // 16 MHz: 250/ 8 MHz: 130
        prescaler = 1;
        cs_value = _BV(CS10);
    }
    else if(set_freq >= 31) {   // 16 MHz: 31 / 8 MHz: 16
        prescaler = 8;
        cs_value = _BV(CS11);
    }
    else if(set_freq >= 4) {   // 16 MHz: 4 / 8 MHz: 2
        prescaler = 64;
        cs_value = _BV(CS11)|_BV(CS10);
    }
    else {
        prescaler = 256;
        cs_value = _BV(CS12);
    }
    // TOP in ICR1
    calc_timer_regs = ((uint32_t)(F_CPU) / (prescaler * (uint32_t)(set_freq))) -1;
    ICR1 = calc_timer_regs;
    // pulse width in OCR1A
    OCR1A = (calc_timer_regs * set_pulse_witdh) / PWM_MAX_PULSE_WIDTH;

    // enable interrupt for sweeping
    TIFR1  |= (1<<OCF1A);
    TIMSK1 |= (1<<OCIE1A);

    // use mode 14: fast pwm, TOP: ICR1
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | cs_value;
}

// mode = 0: up / 1: down / 2: up-down
void pwm_start_sweep_pulse_width(uint16_t set_freq, uint16_t set_pulse_witdh, uint16_t sweep_start, uint16_t sweep_stop, uint16_t sweep_step, uint8_t mode) {
    return;
}
void pwm_stop(void) {
    TIMSK1 = 0; // disable all interrupts
    TCCR1B = 0; // stop clock/counter
    TCCR1C = 0; // force outputs low
    TCCR1A = 0;
}
