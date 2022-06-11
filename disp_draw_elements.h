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
#define TEXT_ON     "ON"
#define TEXT_OFF     "OFF"
#define TEXT_3_3V   "3.3V"
#define TEXT_5_0V   "5.0V"
#define TEXT_SWEEP_OFF   "SWEEP_OFF"
#define TEXT_SWEEP_DUTY   "SWEEP_FREQ"
#define TEXT_SWEEP_DUTY   "SWEEP_DUTY"

struct coord {
    int8_t x, y;
};

#define TEXT_LABEL_OPT_FOCUS    _BV(0)
#define TEXT_LABEL_OPT_INVERSE  _BV(1)
struct text_label {
    char *text;
    struct coord text_pos;
    struct coord frame_offset;
    struct coord frame_size;
    uint8_t options;
};

// - public functions ----------------------------------------------------------
void disp_draw_init(void);
void disp_draw_clear(void);

void disp_draw_info(void);
uint8_t disp_draw_update_info(void);
void disp_draw_pwm_setup(void);

#endif /* _DISP_DRAW_H_ */
