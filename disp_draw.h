/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _DISP_DRAW_H_
#define _DISP_DRAW_H_
// - includes ------------------------------------------------------------------
#include <stdint.h>
#include "macrolib.h"

// - definitions ---------------------------------------------------------------

// - public variables ----------------------------------------------------------
extern const uint16_t decimal_place[];

// - public functions ----------------------------------------------------------
void disp_draw_init(void);
void disp_draw_clear(void);

void disp_draw_info(void);
uint8_t disp_draw_update_info(void);

typedef enum
{
    MENU_ITEM_FREQ = 0,
    MENU_ITEM_DUTY,
    MENU_ITEM_SWEEP_MODE,
    MENU_ITEM_SWEEP_DUTY_START,
    MENU_ITEM_SWEEP_DUTY_STOP,
    MENU_ITEM_SWEEP_FREQ_START,
    MENU_ITEM_SWEEP_FREQ_STOP,
} pwm_setup_menus_t;

typedef enum
{
    SWEEP_MODE_OFF = 0,
    SWEEP_MODE_DUTY,
    SWEEP_MODE_FREQ,
} pwm_sweep_modes_t;

typedef struct
{
    uint16_t freq; // 0 ... 10000  (0 ... 10000 Hz)
    uint16_t duty; // 0 ... 1000 (0.0 ... 100.0 %)
    uint8_t status;
    pwm_setup_menus_t menu;
    uint8_t menu_pos;
    struct
    {
        uint16_t duty_start, duty_stop;
        uint16_t freq_start, freq_stop;
        pwm_sweep_modes_t mode;
    } sweep;
} pwm_settings_t;
#define FREQ_POS_0 (6)
#define DUTY_POS_0 (6)
#define MENU_POS (255)

#define PWM_STATUS_ON _BV(0) // =1: ON, =0: OFF

void disp_draw_pwm_setup(pwm_settings_t *ps);

#endif /* _DISP_DRAW_H_ */
