/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _DISP_DRAW_H_
#define _DISP_DRAW_H_
/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "macrolib.h"

// - definitions ---------------------------------------------------------------

// - public functions ----------------------------------------------------------
void disp_draw_init(void);
void disp_draw_clear(void);

void disp_draw_info(void);
uint8_t disp_draw_update_info(void);

typedef enum
{
    ITEM_FREQ = 0,
    ITEM_DUTY,
    ITEM_SWEEP,
} pwm_setup_items_t;
typedef struct
{
    uint16_t freq;    // 0 ... 10000  (0 ... 10000 Hz)
    uint8_t freq_pos; // 0 ... 3 (1, 10, 100, 1000)
    uint16_t duty;    // 0 ... 1000 (0.0 ... 100.0 %)
    uint8_t duty_pos; // 0, 2, 3 (0.1, 1.0, 10.0)
    uint8_t status;
    pwm_setup_items_t item;
} pwm_settings_t;
#define FREQ_POS_0 (6)
#define DUTY_POS_0 (6)
#define ITEM_POS (255)

#define PWM_STATUS_ON _BV(0) // =1: ON, =0: OFF

void disp_draw_pwm_setup(pwm_settings_t *ps);

#endif /* _DISP_DRAW_H_ */
