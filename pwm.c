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
 * outputs(same frequency, different duty cycle):
 *  OC1A
 *  OC1B
 *
 * f_{pwm} = f_{clkI/O}/(N*(1+TOP))
 * f_{clkI/O} = F_CPU = 8000000
 * N: prescalser = 1 (1, 8, 64, 256, or 1024)
 *
 */
#warning _TODO frequency sweep
#warning _TODO duty cycle sweep

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "io.h"
#include "pwm.h"

// - private variables ---------------------------------------------------------
static volatile uint32_t timer_top;

#define SWEEP_DUTY_DIR_UP 0
#define SWEEP_DUTY_DIR_DOWN 1

#define SWEEP_MODE_OFF 0
#define SWEEP_MODE_DUTY 1
static volatile struct
{
    struct
    {
        uint16_t ocr_start, ocr_stop, ocr_step, next_ocr;
        uint8_t dir;
    } duty;
    uint8_t mode;
} sweep_ctrl;

// - private functions ---------------------------------------------------------
ISR(TIMER1_COMPA_vect)
{
    if (sweep_ctrl.mode == SWEEP_MODE_DUTY)
    {
        // set previously calculated next OCR
        OCR1A = sweep_ctrl.duty.next_ocr;
        // calc next OCR
        if (sweep_ctrl.duty.dir == SWEEP_DUTY_DIR_UP)
        {
            sweep_ctrl.duty.next_ocr = OCR1A + sweep_ctrl.duty.ocr_step;
        }
        else
        {
            sweep_ctrl.duty.next_ocr = OCR1A - sweep_ctrl.duty.ocr_step;
        }
        if (sweep_ctrl.duty.next_ocr > sweep_ctrl.duty.ocr_stop)
        {
            sweep_ctrl.duty.next_ocr = sweep_ctrl.duty.ocr_start;
        }
    }
}

// - public functions ----------------------------------------------------------
void pwm_init(void)
{
    DDRB |= (1 << PIN_OC1A);
}

void pwm_start(uint16_t set_freq, uint16_t set_duty_cycle)
{

    uint16_t prescaler = 1;
    uint8_t cs_value = _BV(CS12);
    TIMSK1 = 0; // disable all interrupts

    // TOP = f_{clkI/O}(N*f_{pwm}-1
    // check set_duty_cycle
    if (set_duty_cycle > PWM_MAX_DUTY_CYCLE)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB |= _BV(PIN_OC1A);
        return;
    }
    if (set_duty_cycle == 0)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB &= ~_BV(PIN_OC1A);
        return;
    }
    // get prescaler N from range
    if (set_freq > PWM_MAX_FREQUENCY)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB |= _BV(PIN_OC1A);
        return;
    }
    if (set_freq == 0)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB &= ~_BV(PIN_OC1A);
        return;
    }
    else if (set_freq >= 250)
    {
        prescaler = 1;
        cs_value = _BV(CS10);
    }
    else if (set_freq >= 31)
    {
        prescaler = 8;
        cs_value = _BV(CS11);
    }
    else if (set_freq >= 4)
    {
        prescaler = 64;
        cs_value = _BV(CS11) | _BV(CS10);
    }
    else
    {
        prescaler = 256;
        cs_value = _BV(CS12);
    }
    // TOP in ICR1
    timer_top = ((uint32_t)(F_CPU) / (prescaler * (uint32_t)(set_freq))) - 1;
    ICR1 = (uint16_t)timer_top;
    // duty cycle in OCR1A
    OCR1A = (uint16_t)((timer_top * set_duty_cycle) / PWM_MAX_DUTY_CYCLE);

    // use mode 14: fast pwm, TOP: ICR1
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | cs_value;
}

void pwm_start_sweep_duty_cycle(uint16_t set_freq, uint16_t duty_cycle_start, uint16_t duty_cycle_stop)
{
    uint16_t prescaler = 1;
    uint16_t swap;
    uint8_t cs_value = _BV(CS12);
    TIMSK1 = 0; // disable all interrupts

    sweep_ctrl.mode = SWEEP_MODE_OFF;
    // check duty_cycle_start
    if (duty_cycle_start == duty_cycle_stop)
    {
        // no need for sweeping
        pwm_start(set_freq, duty_cycle_start);
        return;
    }

    if (duty_cycle_start > duty_cycle_stop)
    {
        // direction: count down
        sweep_ctrl.duty.dir = SWEEP_DUTY_DIR_DOWN;
        // swap start and stop, so always get from start to stop
        swap = duty_cycle_start;
        duty_cycle_start = duty_cycle_stop;
        duty_cycle_stop = swap;
    }
    else
    {
        // direction: count up
        sweep_ctrl.duty.dir = SWEEP_DUTY_DIR_UP;
        // no need to swap start and stop
    }
    if (duty_cycle_stop > PWM_MAX_DUTY_CYCLE)
    {
        duty_cycle_stop = PWM_MAX_DUTY_CYCLE;
    }
    // get prescaler N from range
    if (set_freq > PWM_MAX_FREQUENCY)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB |= _BV(PIN_OC1A);
        return;
    }
    if (set_freq == 0)
    {
        // error, out of range, do not start pwm
        pwm_stop();
        DDRB |= _BV(PIN_OC1A);
        PORTB &= ~_BV(PIN_OC1A);
        return;
    }
    else if (set_freq >= 250)
    {
        prescaler = 1;
        cs_value = _BV(CS10);
    }
    else if (set_freq >= 31)
    {
        prescaler = 8;
        cs_value = _BV(CS11);
    }
    else if (set_freq >= 4)
    {
        prescaler = 64;
        cs_value = _BV(CS11) | _BV(CS10);
    }
    else
    {
        prescaler = 256;
        cs_value = _BV(CS12);
    }
    sweep_ctrl.mode = SWEEP_MODE_DUTY;
    // TOP in ICR1
    timer_top = ((uint32_t)(F_CPU) / (prescaler * (uint32_t)(set_freq))) - 1;
    ICR1 = (uint16_t)timer_top;
    // duty cycle in OCR1A
    sweep_ctrl.duty.ocr_start = (uint16_t)((timer_top * duty_cycle_start) / PWM_MAX_DUTY_CYCLE);
    sweep_ctrl.duty.ocr_stop = (uint16_t)((timer_top * duty_cycle_stop) / PWM_MAX_DUTY_CYCLE);
    sweep_ctrl.duty.ocr_step = (uint16_t)((timer_top * 1) / PWM_MAX_DUTY_CYCLE);
    // set OCR at start
    OCR1A = sweep_ctrl.duty.ocr_start;
    // calc next OCR
    if (sweep_ctrl.duty.dir == SWEEP_DUTY_DIR_UP)
    {
        sweep_ctrl.duty.next_ocr = OCR1A + sweep_ctrl.duty.ocr_step;
    }
    else
    {
        sweep_ctrl.duty.next_ocr = OCR1A - sweep_ctrl.duty.ocr_step;
    }
    if (sweep_ctrl.duty.next_ocr > sweep_ctrl.duty.ocr_stop)
    {
        sweep_ctrl.duty.next_ocr = sweep_ctrl.duty.ocr_start;
    }

    // enable interrupt for sweeping
    TIFR1 |= (1 << OCF1A);
    TIMSK1 |= (1 << OCIE1A);

    // use mode 14: fast pwm, TOP: ICR1
    TCCR1A = _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | cs_value;
}
void pwm_stop(void)
{
    TIMSK1 = 0; // disable all interrupts
    TCCR1B = 0; // stop clock/counter
    TCCR1C = 0; // force outputs low
    TCCR1A = 0;
}
